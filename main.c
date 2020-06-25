#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct LR35902_t {
  struct registers {
    union {
      struct {
        uint16_t af, bc, de, hl;
      };

      struct {
        uint8_t f, a, c, b, e, d, l, h;
      };
    };

    uint16_t pc, sp;
  } registers;

} LR35902_t;

typedef bool (*Opcode_Callback)(LR35902_t *, uint8_t []);

typedef struct {
    char *mnemonic;
    Opcode_Callback callback;
    uint8_t bytes;
    struct {
      int jumped, ignore;
    } cycles;
} Opcode;


bool cpu_nop(LR35902_t *cpu, uint8_t *bytes) {
    /* todo 0x00 */
    abort();

    return true;
}

Opcode opcodes[] = {
    {
      .bytes = 1,
      .callback = cpu_nop,
      .cycles = {
        .jumped = 0,
        .ignore = 4
      },
      .mnemonic = "NOP",
    }
};

void Symfile_Parse(char *path) {
  uint8_t bank;
  uint16_t address;
  char buffer[128], label[128];

  FILE *filehandle;
  filehandle = fopen(path, "r");

  while(fgets(buffer, sizeof(buffer), filehandle)) {
    int count = sscanf(buffer, "%02hhu:%04hX %128s\n", &bank, &address, label);

    if (count != 3) continue;
    printf("bank: %02hhu, address: 0x%04hX, label: %s\n", bank, address, label);
  }

  fclose(filehandle);
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
    Opcode *op = &(opcodes[0]);
    int cycles;
    bool jumped; // = op->callback(&cpu, NULL);

    if (jumped) {
        cycles += op->cycles.jumped;
        // continue;
    }

    cpu.registers.pc += op->bytes;
    cycles += op->cycles.ignore;

    return 0;
}
