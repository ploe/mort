#include <stdlib.h>

struct _Gb_t;

typedef struct _Gb_t Gb_t;

uint16_t MemoryGet(void *, uint16_t);

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

/* OpMethod_t is the signature of the callback function method in Op_t */
typedef uint16_t (*OpMethod_t)(Gb_t *);

typedef struct {
  /* Op_t is the type that describes what a CPU Op (instruction) looks like */
  struct {
    /* cycles is the amount of cycles the operation takes to execute */
    int jump, next;
  } cycles;

  struct {
    /* flags describe how the F register changes when the op is executed */
    char z, n, h, c;
  } flags;

  /* method is the callback function we call to execute the op */
  OpMethod_t method;

  /* length is the size of the instruction in binary, we need this to increment
  PC after executing the op */
  uint16_t length;
} Op_t;

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

static inline Op_t *getOp(uint16_t opcode) {
  return &ops[opcode];
}

void *GbRun(Gb_t *gb, uint16_t entrypoint) {
  uint16_t *pc = &gb->cpu->pc;
  *pc = entrypoint;

  void *error = NULL;

  while (!error) {
    uint16_t opcode = MemoryGet(gb->memory, *pc);
    Op_t *op = getOp(opcode);

    if ((error = gb->hooks.pre(gb, op))) continue;

    uint16_t jump = op->method(gb);
    *pc = jump ? jump : *pc + op->length;

    error = gb->hooks.post(gb, op);
  }

  return error;
}
