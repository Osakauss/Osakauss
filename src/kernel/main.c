#include <stdint.h>
#include <stivale2.h>
#include <x86.h>
#include <types.h>
#include <libs/klibc.h>
#include <libs/dynlist.h>

#include <kernel/drivers/graphics/framebuffer.h>
#include <kernel/log.h>

#include <kernel/cpu/GDT.h>
#include <kernel/cpu/IDT.h>
#include <kernel/cpu/ISR.h>

#include <kernel/input.h>

#include <kernel/mem/pmm.h>

#include <kernel/fs/vfs.h>

#include <kernel/drivers/disk/initrd.h>

#include <kernel/fs/devfs.h>

#include <kernel/fs/fd.h>


u8 stack[16000];



static struct stivale2_header_tag_framebuffer framebuffer_header_tag =
{
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = 0
    },
    .framebuffer_width  = 800,
    .framebuffer_height = 600,
    .framebuffer_bpp    = 32
};

void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
    struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
    for (;;) {

        if (current_tag == NULL) {
            return NULL;
        }

        if (current_tag->identifier == id) {
            return current_tag;
        }


        current_tag = (void *)current_tag->next;
    }
}



bool init(struct stivale2_struct *stivale2_info){

    struct stivale2_struct_tag_framebuffer *framebuffer_tag;
    struct stivale2_struct_tag_memmap *memmap;
    struct stivale2_struct_tag_modules *modules;

    modules = stivale2_get_tag(stivale2_info, STIVALE2_STRUCT_TAG_MODULES_ID);


    framebuffer_tag = stivale2_get_tag(stivale2_info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
    memmap = stivale2_get_tag(stivale2_info, STIVALE2_STRUCT_TAG_MEMMAP_ID);



    framebufferInit(framebuffer_tag->framebuffer_addr,(struct framebuffer_pixel*)framebuffer_tag->framebuffer_addr,
                    framebuffer_tag->framebuffer_width,framebuffer_tag->framebuffer_height,
                    framebuffer_tag->framebuffer_bpp,framebuffer_tag->framebuffer_pitch);


    require_log(LOG_BOTH);

    trace = true;

    logf("   ...:::   Osakauss v0.0.1   :::...\n\n");

    logf("bootloader->[\04%s\01]\nbootloader-version->[\04%s\01]\n\n",stivale2_info->bootloader_brand,stivale2_info->bootloader_version);


    gdtInit();
    IDTInit();
    ISRInit();
    enable_interrupts();

    pmmInit(memmap);

    require_input(INPUT_BOTH);

    vfs_init();

    vfs_node *root = vfs_get_node("/");
    if (root == NULL){
        KERNEL_ERROR("CANNOT FIND ROOT");
    }

    vfs_node *new_node = vfs_mkfile(root, "file.txt");
    if(new_node == NULL){
        logf("Not enough ram\n");
        hang();
    }



    if (!devfs_init())
        KERNEL_ERROR("Failed to init dev fs!");



    logf("root->name->[%s]\nroot->children_count->[%d]\n\n", root->name, root->children_count);

    for (u64 v = 0; v <= root->children_count-1; v++){
        logf("filename->[\"%s\"]\n\n",root->children[v]->name);
    }


    logf("module_amount->[%d]\n\n", modules->module_count);
    logf("Module 1 is = [%s]\n\n", modules->modules[0].string);

    int file = fd_open("/dev/crap");
    if (file < 0){
        logf("Failed to get file or folder\n");
    }
    logf("\nopened file fd = %d\n", file);

    char *data = pmm_calloc(100);

    fd_write(file, 3 ,0 , "ls\0");
    fd_read(file, 3, 0, data);

    logf("adfadf = %s\n", data);

    //initrd_init(modules);

    return true;
}

void main(struct stivale2_struct * stivale2_info) {
    init(stivale2_info);
    logf("[DONE]\n");
    for (;;){
        asm volatile("hlt");
    }
}


__attribute__((section(".stivale2hdr"), used))
static struct stivale2_header stivale_hdr = {

    .entry_point = 0,

    .stack = (uintptr_t)stack + sizeof(stack),

    .flags = (1 << 1),

    .tags = (uintptr_t)&framebuffer_header_tag
};
