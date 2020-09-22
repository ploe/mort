#include <stdio.h>

#include "mort/mmu/mmu_t.h"

int main(int argc, char *argv[]) {
    Mmu_t *mmu = NewMmu("../pulp/pulp.gb");

    GetSymbol(mmu->symfile, "Sprite_Bank_1");
    GetSymbol(mmu->symfile, "Sprite_Bank_0");

    putchar(mmu->rom->data[0x113A]);
    putchar(mmu->rom->data[0x113B]);
    putchar(mmu->rom->data[0x113C]);
    putchar(mmu->rom->data[0x113D]);

    mmu = DestroyMmu(mmu);

    return 0;
}
