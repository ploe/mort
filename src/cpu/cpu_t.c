
void *GbRun(void *gb, uint16_t entrypoint) {
  uint16_t *pc = &gb->cpu->pc;
  *pc = entrypoint;

  void *error = NULL;

  while (!error) {
    op = GetOp(*pc);

    if (error = prehook(gb, op)) continue;

    jump = RunOp(gb, op);
    *pc = jump ? jump : *pc + op->length;

    error = posthook(gb, op);
  }

  return error;
}
