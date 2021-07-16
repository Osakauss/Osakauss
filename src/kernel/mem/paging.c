#include <kernel/mem/paging.h>
#include <kernel/mem/phys.h>
#include <kernel/log.h>
#include <libs/stdlib.h>

PageBitmap * PBM; // stands for Page Bitmap

extern void 
setBit(u64 idx){  
    u8 bit = idx % 8;
    u64 byte = idx / 8;
    PBM->base[byte] |= (bit << 1);
}
extern void 
clrBit(u64 idx){
    u8 bit = idx % 8;
    u64 byte = idx / 8;
    PBM->base[byte] &= ~(1 << bit);
}
extern u64 
getBit(u64 idx){
    u8 bit = idx % 8;
    u64 byte = idx / 8;
    return (PBM->base[byte] >> bit) & 0x1;
}
extern void
printBmp(u64 start, u64 length){
    for (u64 i = start; i < start + length; i++) {
        logf("\nBitmap entry->[%s]\n",itob(getBit(i), 2));
  }
}

bool initPBM(){ // stands for init Page bitmap
    PBM = physmem_alloc(sizeof(PageBitmap));
    PBM->base = physmem_alloc(sizeof(char));
}


static u64 firstFree() {
  for (u64 i = 0; i < PBM->size; i++) {
    if (!getBit(i)) {
      return i;
    }
  }
  return -1;
}

static u64 firstFreeAfter(u64 low_limit) {
  for (u64 i = low_limit; i < PBM->size; i++) {
    if (!getBit(i)) {
      return i;
    }
  }
  return -1;
}


void freePages(u64 *addr, u64 number_pages) {
  u64 start_page = (u64)addr / PAGE_SIZE;
  for (u64 i = start_page; i < start_page + number_pages; i++) {
    clrBit(i);
  }
}

extern u64 
locateAlloc(u64 number_pages) {
  u64 base_addr = allocPages(number_pages);
  memset((u64 *)base_addr + KERNEL_VIRT_ADDR_BASE, (u64)0,
             number_pages * 0x1000);
  return base_addr;
}



extern u64 
allocPages(u64 number_pages) {
  u64 first_free_pg = firstFree();
  u64 base_addr = first_free_pg * PAGE_SIZE;
  u64 cur_free = 0;

  if (first_free_pg == -1) {
    tracef("[PMM] Error: \003No free pages!\001\n",NULL);
    return -1;
  }

  for (u64 i = first_free_pg; i < PBM->size; i++) {
    if (getBit(i)) {
      base_addr += (cur_free + 1) * PAGE_SIZE;
      cur_free = 0;
      continue;
    }

    if (++cur_free == number_pages) {
      for (u64 j = 0; j < number_pages; j++) {
        setBit(j + base_addr / PAGE_SIZE);
      }
      return base_addr;
    }
  }

  return 0;
}



