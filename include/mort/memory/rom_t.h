#ifndef _MORT_MMU_ROM_T_H
#define _MORT_MMU_ROM_T_H

/* prototypes */

struct _Memory_t;
typedef struct _Memory_t Memory_t;

/* types */

typedef struct Rom_t {
  /* Rom_t is a container for the ROM data */
  char *start, *bank;
  long count;
} Rom_t;

Rom_t *OpenRom(const char *);
Rom_t *CloseRom(Rom_t *);
uint8_t RomGet(Memory_t *, uint16_t);
void RomSet(uint16_t, uint8_t);

#endif
