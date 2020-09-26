#include <stdio.h>

#include "mort/mmu/mmu_t.h"

int main(int argc, char *argv[]) {
    Mmu_t *mmu = NewMmu("../pulp/pulp.gb", "../pulp/pulp.sym");

    Symbol_t *symbol = GetSymbol(mmu->symfile, "Kernel_Version");

    if (symbol)
      printf("%.4s\n", &mmu->rom->bank0[symbol->address]);

    //RomGet(0xFFFF);

    uint8_t p = RomGet(mmu->rom, symbol->address);
    RomGet(mmu->rom, 0x0000);
    putchar(p);

    mmu = DestroyMmu(mmu);

    return 0;
}
