#include <libs/BitMap.h>

int setBit_(int n, int k)
{
    return (n | (1 << (k - 1)));
}

int clrBit_(int n, int k)
{
    return (n & (~(1 << (k - 1))));
}


int getBit_(int n, int k)
{
    return ((n >> k) & 1);
}

int BitMapGetBit_(struct BitMapS* self, int k){
    int sector = k / 31;
    int bit = k;

    if (sector != 0){
        bit = k-(sector*31);
        if (bit == 0){
            bit = 31;
            sector--;
        }
    }
    //printf("sector number: %d\nBit number: %d\n",sector,bit);
    return self->BMF.getBit(self->bitmap[sector],k);
}

int BitMapSetBit_(struct BitMapS *self,int k){
    int sector = k / 31;
    int bit = k;

    if (sector != 0){
        bit = k-(sector*31);
        if (bit == 0){
            bit = 31;
            sector--;
        }
    }
    //printf("sector number: %d\nBit number: %d\n",sector,bit);

    // the reason why i add 1 is because when getting the bit
    // for some reason you start from 0 but when writing to the bit
    // it starts from 1, so i want it so that when i put in 0 it will default
    // to 1 so we can keep it all the same.
    self->bitmap[sector] = self->BMF.setBit(self->bitmap[sector],k+1);
    return self->bitmap[sector];
}
int BitMapClrBit_(struct BitMapS *self,int k){
    int sector = k / 31;
    int bit = k;

    if (sector != 0){
        bit = k-(sector*31);
        if (bit == 0){
            bit = 31;
            sector--;
        }
    }
    //printf("sector number: %d\nBit number: %d\n",sector,bit);

    // the reason why i add 1 is because when getting the bit
    // for some reason you start from 0 but when writing to the bit
    // it starts from 1, so i want it so that when i put in 0 it will default
    // to 1 so we can keep it all the same.
    self->bitmap[sector] = self->BMF.clrBit(self->bitmap[sector],k+1);
    return self->bitmap[sector];
}
int BitMapNewSector_(struct BitMapS *self, int sector){
    // k will be the sector numnber
    self->bitmap[sector] = self->BMF.setBit(self->bitmap[sector],31);
    self->bitmap[sector] = self->BMF.clrBit(self->bitmap[sector], 31);
    return 0;
}

extern void BitMap(struct BitMapS *Bmp, unsigned int *bmp){
    Bmp->BMF.getBit = getBit_;
    Bmp->BMF.setBit = setBit_;
    Bmp->BMF.clrBit = clrBit_;
    Bmp->BitMapGetBit = BitMapGetBit_;
    Bmp->BitMapSetBit = BitMapSetBit_;
    Bmp->BitMapClrBit = BitMapClrBit_;
    Bmp->BitMapNewSector = BitMapNewSector_;
    Bmp->bitmap = bmp;
}


/*
int main()
{
    BitMap(&Bmp, &bmp_);

    Bmp.bitmap[0] = 12;

    int n  = Bmp.BitMapSetBit(&Bmp, 31);
    int z=0;
    Bmp.BitMapSetBit(&Bmp, 11);
    Bmp.BitMapSetBit(&Bmp,31);
    Bmp.BitMapGetBit(&Bmp,31);


    for (int x = 0; x <= 31; x++){
        z  = Bmp.BitMapGetBit(&Bmp,x);
        printf("%d",z);
    }

    return 0;
}
*/
