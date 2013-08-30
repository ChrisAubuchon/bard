#include <b2lib.h>
#include <trap.h>
#include <cnv_trap.h>

/*#define DEBUG*/
#include <debug.h>

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

btstring_t *getDunTrap(uint8_t level, uint8_t index)
{
  if ((level > 7) || (index > 7))
    return NULL;

  return bts_sprintf("%s_%d", dun_trap_macros[index], level);
}

btstring_t *getChestTrap(uint8_t level, uint8_t index)
{
  if ((level > 7) || (index > 6))
    return NULL;

  return bts_sprintf("%s_%d", chest_trap_macros[index], level);
}

void convertTraps(void)
{
  uint32_t i;
  trapList_t *tl;
  uint8_t tindex = 0;

  printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  printf("<b2_traps>\n");

  tl = trapList_new(DUN_NTRAPS + CHEST_NTRAPS);

  for (i = 0; i < DUN_NTRAPS; i++) {
    trap_t *tr;

    tl->traps[tindex] = trap_new();
    tr = tl->traps[tindex++];

    tr->string = bts_strcpy(dun_trap_strings[i % 8]);
    tr->macro = getDunTrap((i / 8) + 1, i % 8);
    tr->partyFlag = dun_trap_partyFlag[i % 8];
    tr->ndice = dun_trap_dice[i % 8] * ((i / 8) + 1);
    tr->dieval = 4;

    tr->sv_chance = 50;
    tr->sv_half = 50;

    tr->spAttack = dun_trap_spAttack[i % 8];
  }

  for (i = 0; i < CHEST_NTRAPS; i++) {
    trap_t *tr;

    tl->traps[tindex] = trap_new();
    tr = tl->traps[tindex++];

    tr->string = bts_strcpy(chest_trap_strings[i % 7]);
    tr->macro = getChestTrap((i/7) + 1, i % 7);
    tr->partyFlag = ((i % 7) < 3) ? 1 : 0;
    tr->spAttack = chest_trap_spAttack[i % 7];
    tr->ndice = chest_trap_dice[i % 7] * ((i / 7) + 1);
    tr->dieval = 4;

    /* 50% chance of saving throw on chest traps */
    tr->sv_chance = 50;
    tr->sv_half = 50;
  }

  outputTrapListXML(2, tl);

  printf("</b2_traps>\n");
  trapList_free(tl);
}

