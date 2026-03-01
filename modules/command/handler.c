#include "handler.h"
#include "../vga/vga.h"

int currentLine = 0;

unsigned char *commands[] = {
    "echo",
    "exit",
    0  // null terminator so we know when to stop
};

void handle_command(char* command) {
    int i = 0;
    while (commands[i]) {
        char* currentCommand = (char*)commands[i];
        if (streq(command, currentCommand)) {
            // handle each command
            if (streq(currentCommand, "echo")) {
                print(0, currentLine, "this is text this is text this is text this is text");
            } else if (streq(currentCommand, "exit")) {
                print(0, currentLine, "this is exit");
            }
            currentLine++;
            return;
        }
        i++;
    }
    print(0, currentLine, "Unknown command");
    currentLine++;
}