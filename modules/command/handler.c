#include "handler.h"
#include "../vga/vga.h"

unsigned char *commands[] = {
    "ECHO",
    0  // null terminator so we know when to stop
};

void handle_command(char* command) {
    int i = 0;
    while (commands[i]) {
        if (streq(command, (char*)commands[i])) {
            // handle each command
            if (streq(command, "ECHO")) {
                print(0, 3, "ECHO!");
            }
            return;
        }
        i++;
    }
    print(0, 3, "Unknown command");
}