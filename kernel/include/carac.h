
#ifndef CARAC_H
#define CARAC_H

#include "out.h"
#include "hardware/inout.h"
#include "graphics/button.h"
#include "string.h"
#include "graphics/display.h"
#include "hardware/timer.h"
#define INPUT_BUF_SIZE 128
// do: ring 3 if possible, handlers for errors like division error, kernel panic, etc.
static inline void func(void)
{
    while (1){ 
    1/0;
    }
}
static int sandwich = 0;
static button_t buttons[] = {
    {150, 30, 12, 9, "pendeho", 0xF2, func},
};

static inline void floppa_ldnin(void)
{
    int cursor_x = 160;
    int cursor_y = 100;

    vga_mode13_enable();

    gfx_set_color(0,  (rgb_t){ 32, 32, 32 });
    gfx_set_color(1,  (rgb_t){ 255, 255, 255 });
    gfx_set_color(2,  (rgb_t){ 180, 40, 50 });
    gfx_set_color(3,  (rgb_t){ 40, 180, 90 });
    gfx_set_color(4,  (rgb_t){ 40, 90, 220 });
    gfx_set_color(5,  (rgb_t){ 230, 180, 60 });
    gfx_set_color(6,  (rgb_t){ 120, 70, 35 });
    gfx_set_color(7,  (rgb_t){ 140, 80, 180 });

    gfx_set_color(8,  (rgb_t){ 20, 120, 130 });
    gfx_set_color(9,  (rgb_t){ 255, 100, 120 });
    gfx_set_color(10, (rgb_t){ 100, 220, 180 });
    gfx_set_color(11, (rgb_t){ 80, 100, 160 });
    gfx_set_color(12, (rgb_t){ 240, 140, 40 });
    gfx_set_color(13, (rgb_t){ 190, 150, 120 });
    gfx_set_color(14, (rgb_t){ 70, 45, 100 });
    gfx_set_color(15, (rgb_t){ 220, 220, 200 });

    gfx_set_color(16, (rgb_t){ 15, 10, 5 });
    gfx_set_color(17, (rgb_t){ 50, 30, 20 });
    gfx_set_color(18, (rgb_t){ 90, 50, 30 });
    gfx_set_color(19, (rgb_t){ 32, 32, 124 });
    gfx_set_color(20, (rgb_t){ 60, 120, 220 });
    gfx_set_color(21, (rgb_t){ 100, 180, 255 });
    gfx_set_color(22, (rgb_t){ 255, 220, 180 });
    gfx_set_color(23, (rgb_t){ 180, 90, 60 });

    gfx_set_color(24, (rgb_t){ 80, 160, 60 });
    gfx_set_color(25, (rgb_t){ 160, 220, 70 });
    gfx_set_color(26, (rgb_t){ 200, 60, 160 });
    gfx_set_color(27, (rgb_t){ 120, 30, 90 });
    gfx_set_color(28, (rgb_t){ 40, 200, 220 });
    gfx_set_color(29, (rgb_t){ 20, 70, 100 });
    gfx_set_color(30, (rgb_t){ 255, 80, 20 });
    gfx_set_color(31, (rgb_t){ 100, 100, 100 });

    while (1)
    {
        char key = keyboard_getchar();

        if (key == '^' || key == 'w') cursor_y -= 3;
        if (key == 'v' || key == 's') cursor_y += 3;
        if (key == '<' || key == 'a') cursor_x -= 3;
        if (key == '>' || key == 'd') cursor_x += 3;

        if (cursor_x < 0) cursor_x = 0;
        if (cursor_x > 312) cursor_x = 312;

        if (cursor_y < 0) cursor_y = 0;
        if (cursor_y > 194) cursor_y = 194;

        clear_screen(31);
        draw_gui_button(&buttons[0], 0, 0x3A, 8);
        draw_rect(0, 190, 320, 10, 13);

        draw_rect(cursor_x, cursor_y, 5, 6, 1);
        if (key == '\n' || key == '\r') {
            int i;

            for (i = 0; i < 1; i++) {
                button_activate(&buttons[i], cursor_x, cursor_y);
            }   
        }
        gfx_update();
    }
}
static inline int shell_ld(const char* input)
{ 
    
    if (strcmp(input, "clear") == 0)
    {
        clean_screen();
        return 1;
    }

    if (strcmp(input, "floppa") == 0)
    {
        floppa_ldnin();
        return 0;
    } if (strcmp(input, "user") == 0){
        enter_user_mode();
        return 1;
    } if (strcmp(input, "crash") == 0){
        asm volatile (
            "movl $1, %%eax\n"
            "xorl %%edx, %%edx\n"
            "xorl %%ecx, %%ecx\n"
            "divl %%ecx\n"
            ::: "eax", "ecx", "edx"
        );
        return 1; // remember to use return 1 !
    } if (strcmp(input, "credits") == 0){
        print(0, 5, "Credits to Mangykirb for creating it,");
        print(0, 6, "realxylo and nace! All of their accounts are in the desc");
        wait(300000);
        print(0, 5, "                                                      ");
        print(0, 6, "                                                                       ");
        return 1;
    } if (strcmp(input, "sandwich") == 0){
        print_color(30, 5, "xxxxxxxxxxxxxxxxxxxxxx", 0x06, 1);
        print_color(30, 6, "xxxxxxxxxxxxxxxxxxxxxx", 0x0C, 1);
        print_color(30, 7, "xxxxxxxxxxxxxxxxxxxxxx", 0x0A, 1);
        print_color(30, 8, "xxxxxxxxxxxxxxxxxxxxxx", 0x0C, 1);
        print_color(30, 9, "xxxxxxxxxxxxxxxxxxxxxx", 0x06, 1);
        sandwich = 1;
        return 1;
    } if (strcmp(input, "eat sandwich") == 0){
        if (!sandwich){
            print(0, 2, "Theres no sandwich!");
            return 1;
        }
        static int eaten = 0;
        for (int i = 0; i < 15 && eaten < 110; i++, eaten++){print_color(30 + eaten / 5, 5 + eaten % 5, " ", 0x00, 0);}
        if (eaten >= 110){
            print(0, 2, "Congrats for finding the sandwich secret!");   
            sandwich = 0;
        }
        eaten = 0;
        return 1;
    }

    print(0, 2, "Unknown command: ");
    print(17, 2, input);

    return 1;
}

static inline void terminal_load(void)
{
    char input[INPUT_BUF_SIZE] = {0};

    int len = 0;
    int x = 2;
    int y = 3;

    print(0, y, "> ");

    while (1)
    {
        char c = keyboard_getchar();

        if (!c)
            continue;

        if (c == '\n')
        {
            input[len] = '\0';

            if (!shell_ld(input))
                return;

            len = 0;
            input[0] = '\0';
            x = 2;
            y++;

            if (y >= MAX_Y)
            {
                clean_screen();
                y = 0;
            }

            print(0, y, "> ");
            continue;
        }

        if (c == '\b')
        {
            if (len > 0)
            {
                len--;
                x--;
                input[len] = '\0';
                print(x, y, " ");
            }

            continue;
        }

        if (c >= 32 && c <= 126 &&
            len < INPUT_BUF_SIZE - 1 && x < MAX_X)
        {
            input[len++] = c;

            char out[2] = {c, '\0'};
            print(x, y, out);
            x++;
        }
    }
}

#endif
