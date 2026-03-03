#include "vga.h"   // after the defines so it can use them

void printchar(int x, int y, char c) {
    unsigned short* vga = (unsigned short*)VGA_ADDRESS;
    vga[x + (y * width)] = (unsigned short)c | (0x07 << 8);
}

void print(int x, int y, char* str) {
    int i = 0;
    while (str[i]) {
        printchar(x + i, y, str[i]);
        i++;
    }
}

int streq(char* a, char* b) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

void split(char* str, char** out, int* count) {
    *count = 0;
    int i = 0;
    int len = 0;
    while (str[len]) len++;

    while (i < len) {
        while (i < len && str[i] == ' ') i++;
        if (i >= len) break;

        out[*count] = &str[i];
        (*count)++;

        while (i < len && str[i] != ' ') i++;
        str[i] = '\0';
        i++;
    }
}

void clearLine(int y) {
    for (int x = 0; x < width; x++) {
        printchar(x, y, ' ');
    }
}

void shiftTextUp() {
    unsigned short* vga = (unsigned short*)VGA_ADDRESS;
    for (int i = 0; i+1 < height; i++) {
        for (int x = 0; x < width; x++) {
            vga[(i*width)+x] = vga[((i+1)*width)+x];
        }
    }
}