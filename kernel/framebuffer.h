// framebuffer.h
#pragma once

struct Framebuffer {
    unsigned int* addr;
    unsigned int  width;
    unsigned int  height;
    unsigned int  pitch;
    unsigned char bpp;
};

extern Framebuffer fb;

inline unsigned int rgb(unsigned char r, unsigned char g, unsigned char b) {
    return ((unsigned int)r << 16) | ((unsigned int)g << 8) | b;
}

#define COLOR_BLACK   0x000000
#define COLOR_WHITE   0xFFFFFF
#define COLOR_RED     0xFF0000
#define COLOR_GREEN   0x00FF00
#define COLOR_BLUE    0x0000FF
#define COLOR_CYAN    0x00FFFF
#define COLOR_MAGENTA 0xFF00FF
#define COLOR_YELLOW  0xFFFF00
#define COLOR_ORANGE  0xFF8800
#define COLOR_GRAY    0x888888
#define COLOR_DKGRAY  0x333333

namespace fbsys {
    void put_pixel(int x, int y, unsigned int color);
    void fill(unsigned int color);
    void draw_rect(int x, int y, int w, int h, unsigned int color);
    void draw_rect_outline(int x, int y, int w, int h, unsigned int color);
    void draw_line(int x0, int y0, int x1, int y1, unsigned int color);
    void draw_circle(int cx, int cy, int r, unsigned int color);
    void draw_char(int x, int y, char c, unsigned int color);
    void draw_string(int x, int y, const char* str, unsigned int color);
    void draw_int(int x, int y, int n, unsigned int color);
    void draw_hex(int x, int y, unsigned int n, unsigned int color);
}