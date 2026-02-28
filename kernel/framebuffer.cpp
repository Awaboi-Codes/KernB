#include "framebuffer.h"
#include "../font/8x8.h"

Framebuffer fb = {0, 0, 0, 0, 0};

// ============================================================
//  8x8 bitmap font (ASCII 32-127)
//  Each char = 8 bytes, one byte per row, MSB = leftmost pixel
// ============================================================

// ============================================================
//  Drawing functions
// ============================================================

namespace fbsys {
    void put_pixel(int x, int y, unsigned int color) {
        if ((unsigned int)x >= fb.width || (unsigned int)y >= fb.height) return;
        unsigned int* row = (unsigned int*)((unsigned char*)fb.addr + y * fb.pitch);
        row[x] = color;
    }

    void fill(unsigned int color) {
        for (unsigned int y = 0; y < fb.height; y++) {
            unsigned int* row = (unsigned int*)((unsigned char*)fb.addr + y * fb.pitch);
            for (unsigned int x = 0; x < fb.width; x++) {
                row[x] = color;
            }
        }
    }

    void draw_rect(int x, int y, int w, int h, unsigned int color) {
        for (int row = y; row < y + h; row++)
            for (int col = x; col < x + w; col++)
                put_pixel(col, row, color);
    }

    void draw_rect_outline(int x, int y, int w, int h, unsigned int color) {
        for (int i = x; i < x + w; i++) {
            put_pixel(i, y,         color);
            put_pixel(i, y + h - 1, color);
        }
        for (int i = y; i < y + h; i++) {
            put_pixel(x,         i, color);
            put_pixel(x + w - 1, i, color);
        }
    }

    void draw_line(int x0, int y0, int x1, int y1, unsigned int color) {
        int dx = x1 - x0; if (dx < 0) dx = -dx;
        int dy = y1 - y0; if (dy < 0) dy = -dy;
        int sx = x0 < x1 ? 1 : -1;
        int sy = y0 < y1 ? 1 : -1;
        int err = dx - dy;
        while (true) {
            put_pixel(x0, y0, color);
            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x0 += sx; }
            if (e2 <  dx) { err += dx; y0 += sy; }
        }
    }

    void draw_circle(int cx, int cy, int r, unsigned int color) {
        int x = 0, y = r, d = 3 - 2 * r;
        while (x <= y) {
            put_pixel(cx+x, cy+y, color); put_pixel(cx-x, cy+y, color);
            put_pixel(cx+x, cy-y, color); put_pixel(cx-x, cy-y, color);
            put_pixel(cx+y, cy+x, color); put_pixel(cx-y, cy+x, color);
            put_pixel(cx+y, cy-x, color); put_pixel(cx-y, cy-x, color);
            if (d < 0) d += 4*x + 6; else { d += 4*(x-y) + 10; y--; }
            x++;
        }
    }

    // ============================================================
    //  Text rendering
    // ============================================================

    void draw_char(int x, int y, char c, unsigned int color) {
        if (c < 32 || c > 127) c = '?';
        const unsigned char* glyph = font8x8[c - 32];
        for (int row = 0; row < 8; row++) {
            unsigned char bits = glyph[row];
            for (int col = 0; col < 8; col++) {
                if (bits & (1 << col)) {
                    put_pixel(x + col, y + row, color);
                }
            }
        }
    }

    void draw_string(int x, int y, const char* str, unsigned int color) {
        int cx = x;
        while (*str) {
            if (*str == '\n') { cx = x; y += 10; }
            else { draw_char(cx, y, *str, color); cx += 9; }
            str++;
        }
    }

    void draw_int(int x, int y, int n, unsigned int color) {
        char buf[12];
        int i = 0;
        if (n < 0) { buf[i++] = '-'; n = -n; }
        if (n == 0) { buf[i++] = '0'; }
        else {
            char tmp[12]; int j = 0;
            while (n > 0) { tmp[j++] = '0' + (n % 10); n /= 10; }
            for (int k = j-1; k >= 0; k--) buf[i++] = tmp[k];
        }
        buf[i] = '\0';
        draw_string(x, y, buf, color);
    }

    void draw_hex(int x, int y, unsigned int n, unsigned int color) {
        const char* hex = "0123456789ABCDEF";
        char buf[11];
        buf[0] = '0'; buf[1] = 'x';
        for (int i = 0; i < 8; i++)
            buf[2+i] = hex[(n >> (28 - i*4)) & 0xF];
        buf[10] = '\0';
        draw_string(x, y, buf, color);
    }
}