#include <types.h>
#include <kernel/drivers/graphics/framebuffer.h>
#include <libs/stdlib.h>


struct Sframebuffer framebuffer;

extern void framebufferInit(u64* addr,u32 width, u32 height, u32 ppsl){
    framebuffer.width = width;
    framebuffer.height = height;
    framebuffer.ppsl = ppsl;
    framebuffer.addr = addr;
    memset(framebuffer.backBuffer, 0, sizeof(u8));
}

extern void PutPixel(u32 x, u32 y, struct framebuffer_pixel pixel){
    *(x + (y * framebuffer.ppsl) + (u32*)(framebuffer.addr)) = *(u32*)&pixel;
}

extern struct framebuffer_pixel 
GeneratePixelFG(u32 color)// means: generate pixel forground
{
    struct framebuffer_pixel pixel;
    pixel.alpha    = color >> 24;
    pixel.red      = color >> 16;
    pixel.green    = color >> 8;
    pixel.blue     = color;
    return pixel;
}


extern void
SetFramebufferColor(struct framebuffer_pixel pixel){
    framebuffer.color = pixel;
}