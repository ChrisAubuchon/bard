#ifndef _B3_TRAP_H
#define _B3_TRAP_H

#include <cnv_trap.h>

#define DUN_NTRAPS	24
#define CHEST_NTRAPS	23

extern uint8_t *dun_trap_strings[];
extern uint8_t *dun_trap_macros[];
extern uint8_t dun_trap_spAttack[];
extern uint8_t dun_trap_ndice[];
extern uint8_t dun_trap_save[];
extern uint8_t dun_trap_map[];

extern uint8_t *chest_trap_strings[];
extern uint8_t *chest_trap_macros[];
extern uint8_t chest_trap_dice[];
extern uint8_t chest_trap_flags[];
extern uint8_t chest_trap_save[];
extern uint8_t chest_trap_map[];

#endif
