#ifndef __PHYS_HEADER__
#define __PHYS_HEADER__
#include <types.h>


typedef struct{
    i32 amount;
    i32 start;
    i32 end;
    i32 realSize;
    i32 flag;
}block;


struct frame{
    u8 *frames;
    i32 frameCount;
    block *blocks;
};


extern void physmem_init();
extern u32 physmem_alloc(u32);
extern void physmem_free(u32);
#endif
