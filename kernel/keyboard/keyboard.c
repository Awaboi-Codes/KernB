#include "keyboard.h"

int shift = 0;

unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

unsigned char read_key() {
    while (!(inb(0x64) & 1));
    return inb(0x60);
}

char lowerScancode[] = {
    0, 0, '1','2','3','4','5','6','7','8','9','0','-','=', 0,
    0, 'q','w','e','r','t','y','u','i','o','p','[',']', 0,
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/', 0,
    0, 0, ' '
};

char upperScancode[] = {
    0, 0, '!','@','#','$','%','^','&','*','(',')','_','+', 0,
    0, 'Q','W','E','R','T','Y','U','I','O','P','{','}', 0,
    0, 'A','S','D','F','G','H','J','K','L',':','"','~',
    0,'|','Z','X','C','V','B','N','M','<','>','?', 0,
    0, 0, ' '
};

char key_to_char(unsigned char scancode) {
    char* table = shift ? upperScancode : lowerScancode;
    if (scancode < sizeof(lowerScancode))
        return table[scancode];
    return 0;
}