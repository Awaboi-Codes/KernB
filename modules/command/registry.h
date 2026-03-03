#pragma once
// registry.h
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

#define REGISTER(_cmd) \
__attribute__((constructor)) \
static void __register_##_cmd##__(void) { \
    if (registry_count < MAX_FUNCTIONS) { \
        fn_ptr_t __fn_ptr__ = (fn_ptr_t)(_cmd); \
        registry[registry_count].name = #_cmd; \
        registry[registry_count].fn = __fn_ptr__; \
        registry_count++; \
    } \
} 