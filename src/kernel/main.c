#include <stdint.h>
#include <stivale2.h>
#include <x86.h>
#include <types.h>

#include <kernel/drivers/graphics/framebuffer.h>
#include <kernel/log.h>

#include <kernel/cpu/GDT.h>
#include <kernel/cpu/IDT.h>
#include <kernel/cpu/ISR.h>

#include <kernel/input.h>

#include <kernel/mem/phys.h>
#include <kernel/mem/mm.h>

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

    framebuffer_tag = stivale2_get_tag(stivale2_info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
    memmap = stivale2_get_tag(stivale2_info, STIVALE2_STRUCT_TAG_MEMMAP_ID);



    framebufferInit(framebuffer_tag->framebuffer_addr,(struct framebuffer_pixel*)framebuffer_tag->framebuffer_addr,
                    framebuffer_tag->framebuffer_width,framebuffer_tag->framebuffer_height,
                    framebuffer_tag->framebuffer_bpp,framebuffer_tag->framebuffer_pitch);


    require_log(LOG_BOTH);

    trace = true;

    logf("   ...:::   osakauss v0.0.0  :::...\n\n");


    gdtInit();
    IDTInit();
    ISRInit();
    enable_interrupts();
    require_input(INPUT_BOTH);
    physmem_init();
    mmInit(memmap);
    return true;
}


__attribute__((section(".stivale2hdr"), used))
static struct stivale2_header stivale_hdr = {

    .entry_point = 0,

    .stack = (uintptr_t)stack + sizeof(stack),

    .flags = (1 << 1),

    .tags = (uintptr_t)&framebuffer_header_tag
};





void main(struct stivale2_struct * stivale2_info) {
    init(stivale2_info);
    u64 stuff = physmem_alloc(1);
    u64 l;
    l = stuff;
    stuff = l;
    for (;;){
        asm volatile("hlt");
    }
}
