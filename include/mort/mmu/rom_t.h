#ifndef _MORT_MMU_ROM_T_H
#define _MORT_MMU_ROM_T_H

/* interface */

typedef struct Rom_t {
  /* Rom_t is a container for the ROM data */
  char *data;
  long count;
} Rom_t;

/* methods */

Rom_t *OpenRom(const char *);
Rom_t *CloseRom(Rom_t *);

#endif
