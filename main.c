#include <stdio.h>

#include "mort/mmu/mmu_t.h"

int main(int argc, char *argv[]) {
    Mmu_t *mmu = NewMmu("../pulp/pulp.gb");
    mmu = DestroyMmu(mmu);

    return 0;
}
