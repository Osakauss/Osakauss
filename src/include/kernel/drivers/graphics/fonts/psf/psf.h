#pragma once
#include <types.h>
#define PSF_FONT_MAGIC 0x864ab572
 
typedef struct {
    u32 magic;
    u32 version;
    u32 headersize;
    u32 flags;
    u32 numglyph;
    u32 bytesperglyph;
    u32 height;
    u32 width;
} PSF_font;