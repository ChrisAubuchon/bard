#include <b3lib.h>
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
  if ((level > 7) || (index > 2))
    return NULL;

  return bts_strcpy(dun_trap_macros[(level * 3) + index]);
}

btstring_t *getChestTrap(uint8_t level, uint8_t index)
{
  if ((level > 7) || (index > 3))
    return NULL;

  return bts_strcpy(chest_trap_macros[chest_trap_map[(level * 4) + index]]);
}

void convertTraps(void)
{
  uint32_t i;
  trapList_t *tl;
  uint8_t tindex = 0;

  printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  printf("<b3_traps>\n");

  tl = trapList_new(DUN_NTRAPS + CHEST_NTRAPS);

  for (i = 0; i < DUN_NTRAPS; i++) {
    trap_t *tr;

    tl->traps[tindex] = trap_new();
    tr = tl->traps[tindex++];

    tr->string = bts_strcpy(dun_trap_strings[i]);
    tr->macro = bts_strcpy(dun_trap_macros[i]);
    tr->partyFlag = 1;
    tr->ndice = 0;
    tr->dieval = 0;

    tr->sv_lo = dun_trap_save[dun_trap_map[i]];
    tr->sv_hi = dun_trap_save[dun_trap_map[i]];

    tr->spAttack = dun_trap_spAttack[dun_trap_map[i]];
  }

  for (i = 0; i < CHEST_NTRAPS; i++) {
    trap_t *tr;

    tl->traps[tindex] = trap_new();
    tr = tl->traps[tindex++];

    tr->string = bts_strcpy(chest_trap_strings[i]);
    tr->macro = bts_strcpy(chest_trap_macros[i]);
    tr->partyFlag = (chest_trap_flags[i] & 0x80) ? 1 : 0;
    tr->spAttack = chest_trap_flags[i] & 0x7f;
    tr->ndice = NDICE(chest_trap_dice[i]);
    tr->dieval = DIEVAL(chest_trap_dice[i]);

    tr->sv_lo = chest_trap_save[i];
    tr->sv_hi = chest_trap_save[i];
  }

  outputTrapListXML(2, tl);

  printf("</b3_traps>\n");
  trapList_free(tl);
}

