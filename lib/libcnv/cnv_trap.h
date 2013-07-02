#ifndef _CNV_TRAP_H
#define _CNV_TRAP_H

#include <bt_string.h>

typedef struct {
  btstring_t *macro;
  btstring_t *string;
  uint8_t ndice;
  uint8_t dieval;
  uint8_t spAttack;
  uint8_t sv_lo;
  uint8_t sv_hi;
  uint8_t partyFlag;
  uint8_t sv_chance;
  uint8_t sv_half;
} trap_t;

typedef struct {
  uint8_t ntraps;
  trap_t **traps;
} trapList_t;

trapList_t *trapList_new(uint8_t ntraps);
trap_t *trap_new(void);
void trapList_free(trapList_t *tl);
void trap_free(trap_t *t);
void outputTrapListXML(uint8_t indent, trapList_t *tl);

#endif
