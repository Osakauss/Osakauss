#ifndef __MM_HEADER__
#define __MM_HEADER__
#include <stivale2.h>
#include <types.h>
extern void mmInit(struct stivale2_struct_tag_memmap *);

typedef struct bitmap{
    u64 physmem_addr;
    bool end;
    struct bitmap *next;
}bitmap;



#endif