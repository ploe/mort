#ifndef _MORT_CPU_CPU_T_H
#define _MORT_CPU_CPU_T_H

#include <stdlib.h>

typedef struct {
  union {
    struct {
      uint16_t af, bc, de, hl;
    };

    struct {
      uint8_t f, a, c, b, e, d, l, h;
    };
  };

  uint16_t pc, sp;
} Cpu_t;

#endif
