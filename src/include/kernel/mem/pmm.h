#ifndef __MM_HEADER__
#define __MM_HEADER__
#include <stivale2.h>
#include <types.h>
extern void pmmInit(struct stivale2_struct_tag_memmap *);

extern void * pmm_alloc(u64);

extern void * pmm_realloc(void *, u64);
extern void * pmm_calloc(u64);

extern void pmm_free(void *);

#endif
