#include <stdio.h>

#include "mort/memory/memory_t.h"

int main(int argc, char *argv[]) {
    Memory_t *memory = NewMemory("../pulp/pulp.gb");
    memory = DestroyMemory(memory);

    return 0;
}
