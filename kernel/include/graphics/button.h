#ifndef BUTTON_H
#define BUTTON_H
#define BUTTON_COUNT 3
#include "graphics.h"
#include "../stdint.h"
#include "../carac.h"
typedef void (*button_callback_t)(void);

typedef struct {
    int x;
    int y;
    int w;
    int h;
    const char *label;
    uint8_t color;
    button_callback_t callback;
} button_t;

static inline void button_activate(button_t *b, int cursor_x, int cursor_y)
{
    if (cursor_x >= b->x &&
        cursor_x + 5 <= b->x + b->w &&
        cursor_y >= b->y &&
        cursor_y + 6 <= b->y + b->h)
    {
        if (b->callback)
            b->callback();
    }
}
    
static inline void draw_gui_button(button_t *b, int selected, uint8_t colorer, int fontsize)
{
    uint8_t color = selected ? 5 : b->color;

    draw_rect(b->x, b->y, b->w, b->h, color);
    draw_rect(b->x, b->y, b->w, 1, selected ? 0 : 1);
    draw_rect(b->x, b->y, 1, b->h, selected ? 0 : 1);
    draw_rect(b->x, b->y + b->h - 1, b->w, 1, 0);
    draw_rect(b->x + b->w - 1, b->y, 1, b->h, 0);

    if (b->label) {
        int text_width = 0;
        const char *p = b->label;
        while (*p++){text_width += 8;}
        draw_string(b->x + (b->w - text_width) / 2,b->y + (b->h - 8) / 2,b->label, colorer, fontsize);
    }
}
static inline int button_contains(button_t *b, int x, int y)
{
    return (x < b->x + b->w &&x + 5 > b->x &&y < b->y + b->h &&y + 6 > b->y);
}
#endif