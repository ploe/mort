#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "mort/cpu.h"

typedef struct {
  uint16_t address;
  uint8_t bank;
} Label;

/* LINE_LENGTH: Max symbol length in rgbasm is set to 256, so this is
overshooting it a tad. */
#define LINE_LENGTH 512

void Symbols_ParseFile(char *path) {
  char buffer[LINE_LENGTH];

  FILE *stream = fopen(path, "r");

  while(fgets(buffer, sizeof(buffer), stream)) {
    char key[LINE_LENGTH];
    Label tmp;
    int count = sscanf(buffer, "%02hhu:%04hX %512s\n", &tmp.bank, &(tmp.address), key);

    if (count != 3) continue;
    printf("bank: %02hhu, address: 0x%04hX, label: %s\n", tmp.bank, tmp.address, key);
  }

  fclose(stream);
}

int main(int argc, char *argv[]) {
    LR35902_t cpu;

    cpu.registers.af = 0x3604;

    printf("af: 0x%04X\n", cpu.registers.af);
    printf("a: 0x%02X\n", cpu.registers.a);
    printf("f: 0x%02X\n", cpu.registers.f);

    cpu.registers.bc = 0x4477;

    printf("af: 0x%04X\n", cpu.registers.bc);
    printf("a: 0x%02X\n", cpu.registers.b);
    printf("f: 0x%02X\n", cpu.registers.c);

    // for op at cpu.pc
    Opcode op = GetOpcode(0x33);
    int cycles;
    bool jumped; // =

    if (jumped) {
        cycles += op.cycles.jumped;
        // continue;
    }

    cpu.registers.pc += op.bytes;
    cycles += op.cycles.ignore;

    Symbols_ParseFile("../pulp/pulp.sym");

    op.instruction(&cpu, NULL);

    return 0;
}
