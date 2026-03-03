#include "../modules.h"
#include "handler.h"

// FUNCTION HANDLING

int currentLine = 0;

char* args[16];
int argc;

void handle_command(char* command) {
    split(command, args, &argc);
    fn_ptr_t fn = lookupCommand(args[0]);
    clearLine(currentLine);
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

// FUNCTION REGISTERING

FnEntry registry[MAX_FUNCTIONS];
int registry_count = 0;

void addCommandToRegistry(fn_ptr_t func, const char* name) {
    registry[registry_count++] = (FnEntry){ name, func };
}

fn_ptr_t lookupCommand(const char *name) {
    for (int i = 0; i < registry_count; i++) {
        if (streq(registry[i].name, name) != 0)
            return registry[i].fn;
    }
    return nullptr;
}