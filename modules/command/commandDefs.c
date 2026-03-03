#include "commandDefs.h"

void registry_init(void) {
    addCommandToRegistry((fn_ptr_t)foo, "foo");
}