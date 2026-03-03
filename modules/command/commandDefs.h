#pragma once
#include "../modules.h"

inline static void echo() {
    print(0, currentLine, args[1]);
}

void registry_init(void);