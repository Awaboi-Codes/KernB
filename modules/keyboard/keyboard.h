#ifndef KEYBOARD_H
#define KEYBOARD_H

extern int shift;

unsigned char inb(unsigned short port);
unsigned char read_key();
char key_to_char(unsigned char scancode);

#endif