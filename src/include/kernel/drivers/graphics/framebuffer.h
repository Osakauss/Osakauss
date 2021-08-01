#ifndef __FRAMEBUFFER__HEADER__
#define __FRAMEBUFFER__HEADER__
#include <types.h>
struct framebuffer_pixel
{
    u8 blue;
    u8 green;
    u8 red;
    u8 alpha;
}__attribute__((packed));

struct Sframebuffer{
    u64* addr;
    u8* buffer;
    u32 width;
    u32 height;
    struct framebuffer_pixel color;
    u32 ppsl;
    u8* backBuffer;
};

extern void PutPixel(u32, u32, struct framebuffer_pixel);
extern void framebufferInit(u64 *, u32,u32,u32);
extern struct framebuffer_pixel GeneratePixelFG(u32);
extern void SetFramebufferColor(struct framebuffer_pixel pixel);

#endif