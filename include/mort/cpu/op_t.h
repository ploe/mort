#ifndef _MORT_CPU_OP_T_H
#define _MORT_CPU_OP_T_H

#include <stdlib.h>

/* prototypes */

/* So that we do not have a circular dependency we forward declare Gb_t so we
can use it here */

struct _Gb_t;
typedef struct _Gb_t Gb_t;

/* types */

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

  /* mnemonic is the assembly mnemonic for the op */
  const char *mnemonic;

  /* method is the callback function we call to execute the op */
  OpMethod_t method;

  /* The actual opcode of the the instruction, this should be the index of the
  op in the ops array */
  uint8_t code;

  /* length is the size of the instruction in binary, we need this to increment
  PC after executing the op */
  uint16_t length;
} Op_t;

/* methods */

Op_t *GetOp(uint16_t);

#endif
