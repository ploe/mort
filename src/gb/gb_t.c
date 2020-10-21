#include <stdlib.h>

#include "mort/gb/gb_t.h"

void *GbRun(Gb_t *gb, uint16_t entrypoint) {
  /* GbRun executes each instruction, until it hits a breakpoint in the pre or
  post hook. It returns the breakpoint. */
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
