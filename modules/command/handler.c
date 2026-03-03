#include "../modules.h"
#include "registry.h"

int currentLine = 0;

void handle_command(char* command) {
    fn_ptr_t fn = lookupCommand(command);  // ✅ fn_ptr_t, lookupCommand
    if (fn) {
        fn();
    } else {
        print(0, currentLine, "Unknown command");
    }
    if (currentLine < height) {
        currentLine++;
    } else {
        shiftTextUp();
    }
}