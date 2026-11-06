#ifndef OUT_H
#define OUT_H
#define VGA ((volatile unsigned short*)0xB8000)
#define MAX_Y 25
#define MAX_X 80
static inline void print(int x, int y, const char* str)
{
    volatile unsigned short* vga = (unsigned short*)0xB8000;
    int pos = y * 80 + x;

    while (*str)
        vga[pos++] = (unsigned char)*str++ | (0x0F << 8);
}
static inline void print_color(int x, int y, const char* str, int color, int fullblock) {
    volatile unsigned short* vga = (unsigned short*)0xB8000;
    int pos = y * 80 + x;

    while (*str) {
        unsigned char c = fullblock ? 0xDB : (unsigned char)*str++;

        if (fullblock)
            str++;

        vga[pos++] = c | (color << 8);
    }
}
static inline void screen_print_fb(int x, int y, int color){
    for(int i = 0; i < x*y; i++){
        VGA[i] = color << 8;
    }
}
static inline void clean_screen(){
    for(int i = 0; i < 80*25; i++){
        VGA[i] = 0x0F20;
    }
}// this piss aint rly neccesary cuz theres graphics but i guess its good to have it for the terminal
#endif