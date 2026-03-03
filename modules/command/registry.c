#include "../modules.h"
#include "registry.h"

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