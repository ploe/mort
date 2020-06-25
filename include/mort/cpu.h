#ifndef _MORT_OPCODES_H
#define _MORT_OPCODES_H

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

typedef bool (*Instruction)(LR35902_t *, uint8_t []);

typedef struct {
    char *mnemonic;
    Instruction instruction;
    uint8_t bytes;
    struct {
      int jumped, ignore;
    } cycles;
} Opcode;

inline Opcode Opcode_Get(uint8_t opcode);

#endif
