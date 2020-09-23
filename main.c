#include <stdio.h>

#include "mort/mmu/mmu_t.h"

int main(int argc, char *argv[]) {
    Mmu_t *mmu = NewMmu("../pulp/pulp.gb", "../pulp/pulp.sym");

    Symbol_t *symbol = GetSymbol(mmu->symfile, "Kernel_Version");

    if (symbol)
      printf("%.4s\n", &mmu->rom->data[symbol->address]);

    symbol = GetSymbol(mmu->symfile, "Find this!");

    mmu = DestroyMmu(mmu);

    return 0;
}
