#include <kernel/mem/pmm.h>
#include <types.h>
#include <kernel/log.h>
#include <libs/bitmap.h>
#include <libs/klibc.h>
#define VIRTUALMEM_BASE 0
#define BLOCK_SIZE 0x1000

u64 start_address = 0, end_address = 0; // these are the starting and end addresses of the physical memory

static struct BitMapS bitmap_;

u32 *block_entries; // keep track of how many blocks a block is leaded by


enum MEMORY_STATS{
    FREE,
    UNUSABLE,
    IN_USE,

};

static bool pmm_alloc_ataddress(void*, u64, int);
static u64 address2blockoffset(void *);



static int SetBlock(u64 idx, int value){
    switch (value){
        case FREE:
            bitmap_.BitMapClrBit(&bitmap_,idx*2);
            bitmap_.BitMapClrBit(&bitmap_,(idx*2)+1);
            break;
        case IN_USE:
            bitmap_.BitMapSetBit(&bitmap_,idx*2);
            bitmap_.BitMapSetBit(&bitmap_,(idx*2)+1);
            break;
        case UNUSABLE:
            bitmap_.BitMapSetBit(&bitmap_,idx*2);
            bitmap_.BitMapClrBit(&bitmap_,(idx*2)+1);
            break;
        default:
            break;
    }
    return 0;
}
static int GetBlock(u64 idx){
    int first = 0, second = 0;
    first = bitmap_.BitMapGetBit(&bitmap_,idx*2);

    second = bitmap_.BitMapGetBit(&bitmap_,(idx*2)+1);
    int block = first+second;
    // if bit one and two equal 1 and 1, that makes 2.
    // so if bit one and two equal 1 and 0, that makes 1.
    // so if bit one and two equals 0 and 0, that makes 0.
    // if it is something else, well then we just say it is not usable, just incase.
    switch (block){
        case 0:
            return FREE;
        case 1:
            return UNUSABLE;
        case 2:
            return IN_USE;
        default:
            return UNUSABLE;
    }

    return 0;
}

extern void pmmInit(struct stivale2_struct_tag_memmap *memmap){

    u64 entries = 0, size = 0;
    start_address = memmap->memmap[0].base;
    end_address = memmap->memmap[(memmap->entries - 1)].base;
    for (u64 i = 0; i < memmap->entries; i++){ // we need to found out how many entries there will be in the bitmap
        size += memmap->memmap[i].length; // this is just to keep track of how much memory there is
        for (u64 j = 0; j < memmap->memmap[i].length/BLOCK_SIZE; j++)  {
            entries++;
        }
    }


    for (u64 i = 0; i < memmap->entries; i++){
        if ( ((entries * 2)+(entries*sizeof(u32)) < memmap->memmap[i].length) && (memmap->memmap[i].type == 1)){
            //tracef("> found space for bitmap; entry->[\04%d\01]\n",i);
            bitmap_.bitmap_setup = true; // set this to make sure that we now that we found enough space
            bitmap_.bitmap = (u32 *)(memmap->memmap[i].base + VIRTUALMEM_BASE); // we need to convert the physical address to virtual by adding VIRTUALMEM_BASE  to the physical address
            block_entries = (u32 *)(memmap->memmap[i].base + VIRTUALMEM_BASE + (entries*2));

            // we need to tell the bitmap that we are done
            break;
        }
    }
    if (bitmap_.bitmap_setup == false){ // we need to make sure that we have found enough space for our bitmap
        KERNEL_ERROR("Failed to find memory with large enough space!!");
    }

    bitmap_.entries = entries;
    entries = 0; // make sure we reset it before reusing it

    BitMap(&bitmap_); // init bitmap before use.

    for (u64 i = 0; i < memmap->entries; i++){

        for (u64 j = 0; j < memmap->memmap[i].length/BLOCK_SIZE; j++)  {

            if  (memmap->memmap[i].type == 1)
                SetBlock(entries,FREE);
            else
                SetBlock(entries,UNUSABLE);

            entries++;
        }
    }
    bool bitmap_allocation = pmm_alloc_ataddress(bitmap_.bitmap, entries*2, UNUSABLE);
    bool entries_bitmap_allocation = pmm_alloc_ataddress(block_entries,(entries*sizeof(u32)) , UNUSABLE);
    if (bitmap_allocation == false || entries_bitmap_allocation == false){ // we need to make sure this returns true, because if it doesn't then we are doooooomed, mainly because then we know that we wrote of memory that we should not of done
        KERNEL_ERROR("Failed to set the bitmap or entries bitmap location to unusable because it seems to be already set unusable or in use\n");
    }
}

static bool pmm_alloc_ataddress(void* addr, u64 amount, int STATE){ // this is only really needed for making sure that the bitmap is not written over. but could be used for more maybe later.
    u64 block = address2blockoffset(addr); // make sure to take off 1 because we start from 0 not 1. we count like computers here
    int block_status = 0;
    u64 entries = 0;
    for (u64 v = 0; v < amount/BLOCK_SIZE; v++){ // make sure that this space is actually usable, if not then we have to stop.
        block_status =  GetBlock((u32)(block)+v); // please don't break;
        switch (block_status){
            case FREE:
                break;
            default:
                return false;
        }
    }
    for (u64 v = 0; v < amount/BLOCK_SIZE; v++){
        SetBlock((u32)(block)+v,STATE); // set all the blocks to FREE so that we know that we are done with them
        entries++;
    }
    block_entries[(u32)block] = entries;
    return true;
}

extern void * pmm_alloc(u64 amount){
    int block_status = 0;
    u64 start = 0, end = 0, amnt = 0;
    bool f_free_section = false; // found free section
    for  (u64 i = 0; i <= bitmap_.entries; i++){
        if (amnt >= amount){
            break; // we found it enough space
        }
        block_status = GetBlock(i);
        switch (block_status){
            case FREE:
                if (f_free_section == false){
                    start = i;
                    f_free_section = true;
                }
                end = i;
                amnt+=BLOCK_SIZE;
                break;
            default:
                start = 0;
                f_free_section = false;
                amnt = 0;
                end=0;
        }
    }
    if (amnt == 0){
        return NULL; // if we can't find any space then we have to sadly return NULL so that it knows we can't find it a home.
    }
    u32 block_count = 0;
    // Now we need to set
    for (u64 v = start; v <= end; v++){
        SetBlock(v, IN_USE);
        block_count++;
    }

    block_entries[start] = block_count; // make sure that we keep track of how many blocks where used in this address
    //logf("Block amount->[%d]\nblock_offset>[%d]\n", block_count,(u32)start);
    u64 *address = (u64 *)(start_address + (start * BLOCK_SIZE)) + VIRTUALMEM_BASE; // calculate the memory address
    return (void *)address;
}



extern void pmm_free(void *addr){ // i just know one day this thing is going to give me issues, i can feel it.
    // This will get the starting block that we will set FREE and all its friends that it was using.
    u64 block = address2blockoffset(addr); // make sure to take off 1 because we start from 0 not 1. we count like computers here
    //logf("removing block->[%d]\naddress->[%d]\n", (u32)block,addr);
    // block_entries[(u32)block];. that will give us how many blocks that are used by that memory.
    for (u64 v = 0; v < (u64)block_entries[(u32)block]; v++){
        SetBlock(block+v,FREE); // set all the blocks to FREE so that we know that we are done with them
    }
}



void * pmm_realloc(void * ptr, u64 size){
    void *ptr1 = pmm_alloc(size);
    memcpy(ptr1, ptr, size);
    pmm_free(ptr);
    return ptr1;
}

void * pmm_calloc(u64 size){
    void * ptr = pmm_alloc(size);
    memset(ptr, 0, size);
    return ptr;
}


static u64 address2blockoffset(void * addr){
    u64 block = ( ( (u64)addr / BLOCK_SIZE ) - VIRTUALMEM_BASE);
    return block-1;
}