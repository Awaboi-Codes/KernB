#pragma once
#include "../modules.h"

inline static void foo() {
    print(0, currentLine, "foo!");
}

void registry_init(void);