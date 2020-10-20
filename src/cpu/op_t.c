#include "mort/cpu/op_t.h"

uint16_t nop(Gb_t *gb) {
  /* Nowt happens */
  return 0;
}

Op_t ops[] = {
  {
    .cycles = {
      .next = 4,
    },
    .flags = {
      .z = '-',
      .n = '-',
      .h = '-',
      .c = '-',
    },
    .method = nop,
    .length = 1,
  },
};

inline Op_t *GetOp(uint16_t opcode) {
  return &ops[opcode];
}
