#ifndef _B3_TRAP_H
#define _B3_TRAP_H

#define DUN_NTRAPS	24
#define CHEST_NTRAPS	23

uint8_t *dun_trap_strings[];
uint8_t *dun_trap_macros[];
uint8_t dun_trap_spAttack[];
uint8_t dun_trap_ndice[];
uint8_t dun_trap_save[];
uint8_t dun_trap_map[];

uint8_t *chest_trap_strings[];
uint8_t *chest_trap_macros[];
uint8_t chest_trap_dice[];
uint8_t chest_trap_flags[];
uint8_t chest_trap_save[];
uint8_t chest_trap_map[];

#endif
