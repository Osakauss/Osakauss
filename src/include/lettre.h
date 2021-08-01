#ifndef __LETTRE_HEADER__
#define __LETTRE_HEADER__
#include <types.h>

typedef struct
{
    u32                   Type;
    u64     PhysicalStart;
    u64      VirtualStart;
    u64                   NumberOfPages;
    u64                   Attribute;
} LETTRE_MEMORY_DESCRIPTOR;


typedef struct{
    unsigned long long*    BaseAddress;
    unsigned long long     BufferSize;
    unsigned int           ScreenWidth;
    unsigned int           ScreenHeight;
    unsigned int           PixelsPerScanLine;
}LETTRE_PROTOCAL_FRAMEBUFFER;

typedef struct{
    LETTRE_MEMORY_DESCRIPTOR* MMap;
	unsigned long long     MMapSize;
	unsigned long long     MMapDescriptorSize;
}LETTRE_PROTOCAL_MMAP;

typedef struct{
    unsigned long long*    kernelFileSize;
}LETTRE_PROTOCAL_KERNEL;


typedef struct LETTRE_PROTOCAL
{
    LETTRE_PROTOCAL_FRAMEBUFFER framebuffer;
	LETTRE_PROTOCAL_KERNEL kernelInfo;
    LETTRE_PROTOCAL_MMAP memmap;
	
} LETTRE_PROTOCAL;




#endif