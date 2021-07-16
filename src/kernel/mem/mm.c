#include <kernel/mem/mm.h>
#include <kernel/log.h>
#include <kernel/mem/paging.h>
#include <libs/stdlib.h>


extern PageBitmap *PBM;

extern void
mmInit(struct stivale2_struct_tag_memmap* mm){
    int mmIndex;
    u64 memSize = 0;
    tracef("entry count->[\004%d\001]\n",mm->entries);
    for (mmIndex = 0; mm->entries > mmIndex; mmIndex++){
        tracef("base->[\004%x\001]; length->[\004%d\001]; Next Address->[\004%x\001];\n", 
            mm->memmap[mmIndex].base,
            mm->memmap[mmIndex].length,
            mm->memmap[mmIndex].length+mm->memmap[mmIndex].base 
            );       

        memSize += mm->memmap[mmIndex].length;
    }

    u64 bmpSize = ((memSize / 0x1000) / 8);
    u64 bmpBase = 0;
    tracef("planned pages->[\004%d\001]\n",bmpSize);

    tracef("memSize->[\004%d\001]\n",memSize);

    for (mmIndex = 0; mm->entries > mmIndex; mmIndex++){
        if ((mm->memmap[mmIndex].type == 0x01) && (mm->memmap[mmIndex].length >= bmpSize)){
            PBM->base = (u8 *)(mm->memmap[mmIndex].base + KERNEL_VIRT_ADDR_BASE);
            PBM->size = bmpSize;
            bmpBase = mm->memmap[mmIndex].base;
            memset((u64*)PBM->base, 0xffffffffffffffff, bmpSize / 8);
            break;
        }   
    }
    for (mmIndex = 0; mm->entries > mmIndex; mmIndex++){
        if (mm->memmap[mmIndex].type == 0x01){
            freeMem(mm->memmap[mmIndex].base, mm->memmap[mmIndex].length);
        }   
    }
    reserveMem(bmpBase,bmpSize);
    reserveMem(0,0x100000);

    tracef("PMM initiation->[\004Done\001]\n",NULL);
}
extern void 
reserveMem(u64 base_addr, u64 size) {
    tracef("Reserving Memory->[\004%x\001]\n",base_addr);
    for (u64 i = base_addr / PAGE_SIZE; i < ((base_addr + size) / PAGE_SIZE + (size % PAGE_SIZE ? 1 : 0)); i++) {
        setBit(i);
    }
}
extern void 
freeMem(u64 base_addr, u64 size) {
    tracef("Freeing Memory->[\004%x\001]\n",base_addr);
    for (u64 i = base_addr / PAGE_SIZE; i < ((base_addr + size) / PAGE_SIZE + (size % PAGE_SIZE ? 1 : 0)); i++) {
        clrBit(i);
    }
}

