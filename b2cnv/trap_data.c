#include <b2lib.h>

uint8_t *dun_trap_strings[] = {
  "pit!",
  "spiked pit!",
  "timed Warstrike spell!",
  "poison gas cloud!",
  "tripwire, which caused poison darts to shoot!",
  "tripwire, which caused a stone block to fall!",
  "tripwire, which caused several crossbows to fire at you!",
  "Basilisk snare!"
};

uint8_t *dun_trap_macros[] = {
  "DT_PIT",
  "DT_SPIKEDPIT",
  "DT_WARSTRIKE",
  "DT_POISONGAS",
  "DT_DARTTRIP",
  "DT_BLOCKTRIP",
  "DT_CROSSBOXTRIP",
  "DT_BASILISK_SNARE"
};

uint8_t dun_trap_spAttack[] = {
  0,0,0,1,1,0,0,6
};

uint8_t dun_trap_partyFlag[] = {
  1,1,1,1,0,0,1,0
};

uint8_t dun_trap_dice[] = {
  1,2,2,1,2,4,3,1
};

uint8_t *chest_trap_strings[] = {
  "POISON NEEDLE",
  "BLADES",
  "DARTS",
  "GAS CLOUD",
  "SHOCKER",
  "CRAZYCLOUD",
  "MINDTRAP"
};

uint8_t *chest_trap_macros[] = {
  "CT_POISON_NEEDLE",
  "CT_BLADES",
  "CT_DARTS",
  "CT_GAS_CLOUD",
  "CT_SHOCKER",
  "CT_CRAZYCLOUD",
  "CT_MINDTRAP"
};

uint8_t chest_trap_dice[] = {
  1,2,4,3,7,1,1            
};

uint8_t chest_trap_spAttack[] = {
/*   */ 1,0,0,1,
/*   */ 0,3,5
};

uint8_t chest_trap_save[] = {
/*  0*/ 0x0F, 0x0F, 0x0F, 0x0F,
/*  4*/ 0x13, 0x13, 0x13, 0x13,
/*  8*/ 0x15, 0x15, 0x15, 0x15,
/* 12*/ 0x18, 0x18, 0x18,
/* 15*/ 0x26, 0x26, 0x26, 0x26,
/* 19*/ 0x27, 0x28, 0x27, 0x28
};

