#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct GameBoy {
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

} GameBoy;

typedef bool (*Opcode_Callback)(GameBoy *, uint8_t []);

typedef struct {
    char *mnemonic;
    Opcode_Callback callback;
    uint8_t bytes;
    struct {
      int jumped, ignore;
    } cycles;
} Opcode;


bool cpu_nop(GameBoy *gb, uint8_t *bytes) {
    /* todo 0x00 */
    abort();

    return true;
}

Opcode opcodes[] = {
    {
      .bytes = 1,
      .callback = cpu_nop,
      .cycles = {
        .update = 0,
        .ignore = 4
      },
      .mnemonic = "NOP",
    }
};

int main(int argc, char *argv[]) {
    GameBoy gb;

    gb.registers.af = 0x3604;

    printf("af: 0x%04X\n", gb.registers.af);
    printf("a: 0x%02X\n", gb.registers.a);
    printf("f: 0x%02X\n", gb.registers.f);

    gb.registers.bc = 0x4477;

    printf("af: 0x%04X\n", gb.registers.bc);
    printf("a: 0x%02X\n", gb.registers.b);
    printf("f: 0x%02X\n", gb.registers.c);

    // for op at gb.pc
    Opcode *op = &(opcodes[0]);
    int cycles;
    bool jumped = op->callback(&gb, NULL);

    if (jumped) {
        cycles += op->cycles.jumped;
        // continue;
    }

    gb.pc += op->bytes;
    cycles += op.cycles.ignore;

    return 0;
}
