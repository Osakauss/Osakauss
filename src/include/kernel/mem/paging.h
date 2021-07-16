#ifndef __PAGING_HEADER__
#define __PAGING_HEADER__
#include <types.h>

typedef void (*bitFunc)(u64 idx);


typedef struct{
    u8 *base;
    u64 size;
}PageBitmap;

extern void clrBit(u64 idx);
extern void setBit(u64 idx);
extern void clrBit(u64 idx);
extern u64 getBit(u64 idx);
extern void printBmp(u64 start, u64 length);

extern u64 allocPages(u64);
extern u64 locateAlloc(u64);


#define PAGE_SIZE 0x1000

#define KERNEL_VIRT_ADDR_TOP 0xffffffff80000000
#define KERNEL_VIRT_ADDR_BASE 0xffff800000000000



#endif
