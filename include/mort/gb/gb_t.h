#ifndef _MORT_GB_GB_T_H
#define _MORT_GB_GB_T_H

#include "mort/cpu/cpu_t.h"
#include "mort/cpu/op_t.h"
#include "mort/memory/memory_t.h"

struct _Gb_t;

typedef struct _Gb_t Gb_t;

typedef void *(HookMethod)(Gb_t *, Op_t *);

struct _Gb_t {
  /* Gb_t is a container that holds references to submodules that describe
  hardware features of the Game Boy */
  Cpu_t *cpu;
  struct {
    void *data;
    HookMethod *post, *pre;
  } hooks;
  Memory_t *memory;
};

#endif
