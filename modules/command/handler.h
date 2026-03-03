#ifndef COMMAND_H
#define COMMAND_H

// HANDLING
#include <string.h>

extern int currentLine;

void handle_command(char* command);

// REGISTERING
#define MAX_FUNCTIONS 64

typedef void (*fn_ptr_t)(void);

typedef struct {
    const char *name;
    fn_ptr_t fn;
} FnEntry;

extern FnEntry registry[MAX_FUNCTIONS];
extern int registry_count;
void addCommandToRegistry(fn_ptr_t func, const char* name);
fn_ptr_t lookupCommand(const char *name);

#endif