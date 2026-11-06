
#ifndef VIDEO_H
#define VIDEO_H

#include "hardware/inout.h"

#define VIDEO_TEXT 0
#define VIDEO_GRAPHICS 1

static int video_mode = VIDEO_TEXT;

static inline void set_video_mode(int mode)
{
    if (mode == VIDEO_TEXT)
    {
        outb(0x3C4, 0x00);
        outb(0x3C5, 0x03);

        __asm__ volatile (
            "int $0x10"
            :
            : "a"(0x0003)
            : "memory"
        );

        video_mode = VIDEO_TEXT;
    }
    else if (mode == VIDEO_GRAPHICS)
    {
        __asm__ volatile (
            "int $0x10"
            :
            : "a"(0x0013)
            : "memory"
        );

        video_mode = VIDEO_GRAPHICS;
    }
}

#endif
