
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../hardware/inout.h"
#include "fontmap.h"
#include "../stdint.h"

#define WIDTH 320
#define HEIGHT 200

#define VGA_FB ((volatile unsigned char*)0xA0000)
#define VGA_FB_SIZE (WIDTH * HEIGHT)

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_t;

/* Backbuffer */
static unsigned char backbuffer[VGA_FB_SIZE];

/* Forward declaration */
static inline void putpixel(int x, int y, uint8_t color);

/* Set VGA palette color */
static inline void gfx_set_color(uint8_t index, rgb_t color)
{
    outb(0x3C8, index);
    outb(0x3C9, color.r >> 2);
    outb(0x3C9, color.g >> 2);
    outb(0x3C9, color.b >> 2);
}

/* Clear backbuffer */
static inline void clear_screen(uint8_t color)
{
    for (unsigned int i = 0; i < VGA_FB_SIZE; i++)
        backbuffer[i] = color;
}

/* Copy backbuffer to VGA */
static inline void gfx_update(void)
{
    for (unsigned int i = 0; i < VGA_FB_SIZE; i++)
        VGA_FB[i] = backbuffer[i];
}

/* Draw pixel */
static inline void putpixel(int x, int y, uint8_t color)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;

    backbuffer[y * WIDTH + x] = color;
}

/* Draw rectangle */
static inline void draw_rect(int x, int y, int w, int h, uint8_t color)
{
    if (w <= 0 || h <= 0)
        return;

    int x_end = x + w;
    int y_end = y + h;

    if (x < 0) x = 0;
    if (y < 0) y = 0;

    if (x_end > WIDTH) x_end = WIDTH;
    if (y_end > HEIGHT) y_end = HEIGHT;

    if (x >= x_end || y >= y_end)
        return;

    for (int py = y; py < y_end; py++) {
        for (int px = x; px < x_end; px++) {
            putpixel(px, py, color);
        }
    }
}

/*
 * Draw character with exact pixel dimensions.
 *
 * size = 1  -> 1x1
 * size = 4  -> 4x4
 * size = 8  -> 8x8
 * size = 16 -> 16x16
 * size = 50 -> 50x50
 */
static inline void draw_char(
    int x,
    int y,
    char c,
    uint8_t color,
    int size
)
{
    unsigned char ch = (unsigned char)c;

    if (ch < 32 || ch > 126 || size <= 0)
        return;

    const unsigned char *glyph = font[ch - 32];

    for (int py = 0; py < size; py++) {
        for (int px = 0; px < size; px++) {

            /* Map destination pixels to original 8x8 font */
            int src_x = px * 8 / size;
            int src_y = py * 8 / size;

            if (glyph[src_y] & (1 << src_x)) {
                putpixel(
                    x + px,
                    y + py,
                    color
                );
            }
        }
    }
}

/* Draw string with exact character dimensions */
static inline void draw_string(
    int x,
    int y,
    const char *str,
    uint8_t color,
    int size
)
{
    if (!str || size <= 0)
        return;

    int start_x = x;

    while (*str) {

        if (*str == '\n') {
            x = start_x;
            y += size;
        }
        else if (*str == '\t') {
            x += size * 4;
        }
        else {
            draw_char(x, y, *str, color, size);
            x += size;
        }

        str++;
    }
}

#endif
