#include <stdlib.h>

#include "mort/cpu/cpu_t.h"
#include "mort/cpu/op_t.h"

uint16_t MemoryGet(void *, uint16_t);

typedef void *(HookMethod)(Gb_t *, Op_t *);

struct _Gb_t {
  /* Gb_t is a container that holds references to submodules that describe
  hardware features of the Game Boy */
  Cpu_t *cpu;
  struct {
    void *data;
    HookMethod *post, *pre;
  } hooks;
  void *memory;
};

void *GbRun(Gb_t *gb, uint16_t entrypoint) {
  uint16_t *pc = &gb->cpu->pc;
  *pc = entrypoint;

  void *breakpoint = NULL;

  while (!breakpoint) {
    uint16_t opcode = MemoryGet(gb->memory, *pc);
    Op_t *op = GetOp(opcode);

    if ((breakpoint = gb->hooks.pre(gb, op))) continue;

    uint16_t jump = op->method(gb);
    *pc = jump ? jump : *pc + op->length;

    breakpoint = gb->hooks.post(gb, op);
  }

  return breakpoint;
}
