#ifndef __BITMAP__LIB__HEADER__
#define __BITMAP__LIB__HEADER__

#include <types.h>


typedef int (*funcBitMap)(int num,u64 n);



typedef struct {
    funcBitMap getBit;
    funcBitMap setBit;
    funcBitMap clrBit;
}BitMapfunctions;

struct BitMapS{
    BitMapfunctions BMF; // bitmap functions
    int (*BitMapGetBit)(struct BitMapS* self,u64 k);
    int (*BitMapSetBit)(struct BitMapS *self,u64 k);
    int (*BitMapClrBit)(struct BitMapS* self,u64 k);
    int (*BitMapNewSector)(struct BitMapS* self, u64 sector);
    u32 *bitmap;
    bool bitmap_setup;
    u64 entries;
};

extern void BitMap(struct BitMapS *Bmp);

#endif
