#ifndef __BITMAP__LIB__HEADER__
#define __BITMAP__LIB__HEADER__

#include <types.h>


typedef int (*funcBitMap)(int num,int n);



typedef struct {
    funcBitMap getBit;
    funcBitMap setBit;
    funcBitMap clrBit;
}BitMapfunctions;

struct BitMapS{
    BitMapfunctions BMF; // bitmap functions
    int (*BitMapGetBit)(struct BitMapS* self,int k);
    int (*BitMapSetBit)(struct BitMapS *self,int k);
    int (*BitMapClrBit)(struct BitMapS* self,int k);
    int (*BitMapNewSector)(struct BitMapS* self, int sector);
    u32 *bitmap;
};

extern void BitMap(struct BitMapS *Bmp, u32 *bmp);

#endif
