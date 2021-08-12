#include <kernel/mem/physmm.h>
#include <types.h>
#include <kernel/log.h>
#include <libs/BitMap.h>
#include <x86.h>

#define VIRTUALMEM_BASE 0xffff800000000000


u64 start_address = 0;

u64 end_address = 0;

static struct BitMapS bitmap_;

enum MEMORY_STATS{
    FREE,
    UNUSABLE,
    IN_USE,

};

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
        for (u64 j = 0; j < memmap->memmap[i].length/0x1000; j++)  {
            entries++;
        }
    }


    for (u64 i = 0; i < memmap->entries; i++){
        if (((entries * 2) < memmap->memmap[i].length) && (memmap->memmap[i].type == 1)){
            logf("Found big enough space for bitmap. Memmap entry->[%d]\n",i);
            bitmap_.bitmap_setup = true; // set this to make sure that we now that we found enough space
            bitmap_.bitmap = (u32 *)(memmap->memmap[i].base + VIRTUALMEM_BASE); // we need to convert the physical address to virtual by adding VIRTUALMEM_BASE  to the physical address
            break;
        }
    }
    if (bitmap_.bitmap_setup == false){ // we need to make sure that we have found enough space for our bitmap
        logf("\02Failed to find memory with large enough space!!\n");
        hang();
    }

    bitmap_.entries = entries;
    entries = 0; // make sure we reset it before reusing it

    BitMap(&bitmap_); // init bitmap before use.

    for (u64 i = 0; i < memmap->entries; i++){
        //logf("\nstuff: %d\ntype: %d\n",memmap->memmap[i].length/0x1000, memmap->memmap[i].type);

        for (u64 j = 0; j < memmap->memmap[i].length/0x1000; j++)  {

            if  (memmap->memmap[i].type == 1)
                SetBlock(entries,FREE);
            else
                SetBlock(entries,UNUSABLE);

            entries++;
        }
    }
}
extern void * pmm_alloc(u64 amount){
    int block_status = 0;
    u64 start = 0;
    bool f_free_section = false; // found free section
    u64 end = 0;
    u64 amnt = 0;
    for  (u64 i = 0; i <= bitmap_.entries; i++){
        if (amnt >= amount){
            logf("DONE\n");
            break;
        }
        block_status = GetBlock(i);
        switch (block_status){
            case FREE:
                if (f_free_section == false){
                    start = i;
                    f_free_section = true;
                }
                end = i;
                amnt+=0x1000;
                break;
            default:
                start = 0;
                f_free_section = false;
                amnt = 0;
                end=0;
        }
    }
    for (u64 v = start; v <= end; v++){
        tracef("Setting block->[%d]\n",v);
        SetBlock(v, IN_USE);
    }
    u64 *address = (u64 *)(start_address * 0x1000) + VIRTUALMEM_BASE;
    return (void *)address;
}



extern void pmm_free(){

}
