#include "commandDefs.h"

void registry_init(void) {
    ext4_init();
    addCommandToRegistry((fn_ptr_t)echo, "echo");
}