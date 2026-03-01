#include "../modules/modules.h"

void kernel_main() {
    char command[256];
    int cmd_len = 0;

    print(1, 1, "[KernB Shell] > ");

    while (1) {
        unsigned char scancode = read_key();

        if (scancode == 0x2A || scancode == 0x36) { shift = 1; continue; }
        if (scancode == 0xAA || scancode == 0xB6) { shift = 0; continue; }
        if (scancode & 0x80) continue;

        if (scancode == 0x0E) {         // backspace
            if (cmd_len > 0) {
                cmd_len--;
                putchar(cmd_len, 1, ' ');
            }
            continue;
        }

        if (scancode == 0x1C) {         // enter
            command[cmd_len] = 0;       // null terminate
            handle_command(command);
            // clear input line
            for (int i = 0; i < width; i++)
                putchar(i, 13, ' ');
            cmd_len = 0;
            continue;
        }

        char c = key_to_char(scancode);
        if (c && cmd_len < 255) {
            command[cmd_len] = c;
            putchar(cmd_len, 13, c);
            cmd_len++;
        }
    }
}