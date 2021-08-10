#include <kernel/mem/mm.h>
#include <types.h>
#include <kernel/log.h>
#include <libs/BitMap.h>

u32 bmp_;

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
/*
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
*/


extern void mmInit(struct stivale2_struct_tag_memmap *memmap){

    BitMap(&bitmap_, &bmp_);
    u64 size = 0;
    int idx = 0;
    int entries = 0;
    for (u64 i = 0; i < memmap->entries; i++){
        //logf("number->[%x]\nsize->[%x]\nblocks->[%d]\n", memmap->memmap[i].type,
        //    memmap->memmap[i].length,
        //    memmap->memmap[i].length/0x1000);

        //logf("size->[%x]=[%x]\n",memmap->memmap[i].length, memmap->memmap[i].length/0x1000);
        for (u64 j = 0; j < memmap->memmap[i].length/0x1000; j++)  {

            //logf("address->[%x]\n", memmap->memmap[i].length+(0x1000*j));
            //logf("entry_num->[%d]\n",entries);
            logf("%d|",entries);


            if  (memmap->memmap[i].type == 1)
                SetBlock(entries,FREE);
            else
                SetBlock(entries,UNUSABLE);


            logf("[a]");
            entries++;
            idx++;
        }
        size+=memmap->memmap[i].length;

    }
    //logf("size of all->[%d]\n",size/0x1000);
    //int v = GetBlock(0);
    //logf("\t=> %d;",v);
    logf("all blocks amount->[%d]\n", entries);
}
