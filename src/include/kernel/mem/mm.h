#ifndef __MM_MM_HEADER__
#define __MM__M_HEADER__
#include <types.h>
#include <stivale2.h>

extern void mmInit(struct stivale2_struct_tag_memmap*);
extern void freeMem(u64 base_addr, u64 size);
extern void reserveMem(u64 base_addr, u64 size);
#endif
