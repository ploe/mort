#ifndef _MORT_MMU_WRAM_T_H
#define _MORT_MMU_WRAM_T_H

/* interface */

typedef struct Wram_t {
  /* Wram_t is a container for the WRAM data */
  uint8_t bank0[8192], *bankx;
} Wram_t;

/* methods */


#endif
