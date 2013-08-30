#include <b2lib.h>

static uint8_t *spattackTypes[] = {
  "SPAT_NONE", "SPAT_POISON", "SPAT_LEVDRAIN", "SPAT_INSANITY", "SPAT_OLD",
  "SPAT_POSSESS", "SPAT_STONE", "SPAT_CRITHIT", "SPAT_UNEQUIP", "SPAT_SPPTDRAIN"
};

static uint8_t *breathAttack[] = {
  "ATTEFF_FRIES", "ATTEFF_FROZE", "ATTEFF_SHOCK", "ATTEFF_DRAIN",
  "ATTEFF_BURN", "ATTEFF_CHOKE", "ATTEFF_STEAM"
};

void printSpecialAttack(uint8_t sp)
{
  printf("%s", spattackTypes[sp]);
}

uint8_t *breathAttackEffect(uint8_t br)
{
  return breathAttack[br];
}
