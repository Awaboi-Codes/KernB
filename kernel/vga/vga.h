#ifndef VGA_H
#define VGA_H

#define VGA_ADDRESS 0xB8000
#define width 80
#define height 25

void putchar(int x, int y, char c);
void print(int x, int y, char* str);
int streq(char* a, char* b);
void shiftTextUp();

#endif