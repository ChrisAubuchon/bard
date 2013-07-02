#include <b1lib.h>

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
  1,0,0,1,0,3,5
};


