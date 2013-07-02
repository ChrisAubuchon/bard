#ifndef _B2_TRAP_H
#define _B2_TRAP_H

#define DUN_NTRAPS	64
#define CHEST_NTRAPS	56

extern uint8_t *dun_trap_strings[];
extern uint8_t *dun_trap_macros[];
extern uint8_t dun_trap_spAttack[];
extern uint8_t dun_trap_partyFlag[];
extern uint8_t dun_trap_dice[];

extern uint8_t *chest_trap_strings[];
extern uint8_t *chest_trap_macros[];
extern uint8_t chest_trap_dice[];
extern uint8_t chest_trap_spAttack[];

#endif
