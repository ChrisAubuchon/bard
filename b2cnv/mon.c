#include <b2lib.h>
#include <cnv_mon.h>
#include <mon.h>
#include <fileio.h>
#include <endian.h>
#include <spell.h>

/********************************/
/*				*/
/* Local function prototypes	*/
/*				*/
/********************************/

static btstring_t *prepName(uint8_t *name, int pflag);
static void printMonster(b2mon_t *mon, uint32_t fmon, int index);
static uint8_t willAdvance(b2mon_t *b);

/********************************/
/*				*/
/* Static data			*/
/*				*/
/********************************/

/********************************/
/*				*/
/* Local function 		*/
/*				*/
/********************************/

static uint8_t willAdvance(b2mon_t *b)
{
  int i;

  if (b->advancespeed == 0)
    return 0;

  for (i = 0; i < 4; i++) {
    if (b->attTypes[i] == 0)
      return 1;
  }

  return 0;
}

static btstring_t *prepName(uint8_t *name, int pflag)
{
  uint8_t length = 0;
  btstring_t *rval;

  if (*name == '\0')
    return;

  rval = bts_new(strlen(name));

  do {
    if (*name == '^') {
      name++;
      if (*name == '\0')
	break;

      if (pflag) {
	while (*name++ != '^');
	while ((*name != '^') && (*name != '\0'))
	  rval->buf[length++] = *name++;
      } else {
	while (*name != '^')
	  rval->buf[length++] = *name++;
	name++;
	while ((*name != '^') && (*name != '\0'))
	  name++;
      }
    } else {
      rval->buf[length++] = *name;
    }
  } while (*name++ != '\0');
  rval->buf[length++] = '\0';

  return bts_resize(rval, length);
}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

btstring_t *getSummonMacro(uint32_t index)
{
  return bts_strcpy(sumMacro[index]);
}

void monsterHeader(void)
{
  int i;

  for (i = 0; i < 466; i++)
    printf("#define %s\t%d\n", monMacro[i], i);
}

static void printMonster(b2mon_t *mon, uint32_t fmon, int index)
{
  monster_t *m;

  int j;

  m = monster_new();

  m->singular = prepName(mon->name, 0);
  m->plural = prepName(mon->name, 1);
  m->macro = bts_strcpy(monMacro[index]);
  m->picture = bts_strcpy(getPicMacro(mon->picindex));

  m->hpRndNdice = 1;
  m->hpRndDie = cnv16le(mon->hpRnd);
  m->hpBase = cnv16le(mon->hpBase);
  m->reward = ((mon->numattacks + 1) * mon->basemelee) << 7;
  m->baseAC = mon->baseac2hit;

#define CAP(x,y)  ((x) > (y)) ? (y) : (x)
  m->breathSaveLo = mon->basemelee >> 3;
  m->breathSaveHi = (mon->basemelee >> 3) + 8;
  m->spellSaveLo = m->breathSaveLo;
  m->spellSaveHi = m->breathSaveHi;
  m->toHitLo = mon->baseac2hit;
  m->toHitHi = mon->baseac2hit + 3;
  m->priorityLo = CAP(((mon->basemelee  << 2) | 1), 0xff);
  m->priorityLo = CAP((((mon->basemelee  << 2) | 1) + 31), 0xff);
#undef CAP

  m->rndGroupSize = (fmon < 16) ? 1 : 0;
  m->groupSize = (mon->maxgrp << 1) + 1;
  m->numAttacks = mon->numattacks;
  m->pronoun = mon->flags & 1;
  m->willAdvance = willAdvance(mon);
  m->distance = mon->flags >> 4;
  m->advSpeed = mon->advancespeed;
  m->noRandom = (fmon < 16) ? 1 : 0;
  m->isIllusion = 0;

  m->strong.fire = 0;
  m->strong.unk1 = 0;
  m->strong.holy = 0;
  m->strong.ice = 0;
  m->strong.lightning = 0;
  m->strong.spell = 0;
  m->strong.unk2 = 0;
  m->strong.unk3 = 0;

  m->weak.fire = 0;
  m->weak.unk1 = 0;
  m->weak.holy = 0;
  m->weak.ice = 0;
  m->weak.lightning = 0;
  m->weak.spell = 0;
  m->weak.unk2 = 0;
  m->weak.unk3 = 0;

  m->repel.evil = 0;
  m->repel.demon = 0;
  m->repel.spellcaster = 0;
  m->repel.unk1 = 0;
  m->repel.unk2 = 0;
  m->repel.unk3 = 0;
  m->repel.unk4 = 0;
  m->repel.unk5 = 0;

  switch (mon->picindex) {
    case 33:
    case 53:
    case 57:
    case 60:
      m->repel.spellcaster = 1;
      break;
    case 34:
    case 35:
    case 37:
    case 40:
    case 41:
    case 54:
      m->repel.evil = 1;
      break;
  }

  for (j = 0; j < 4; j++) {
    uint8_t type = mon->attTypes[j];

    if (type == 0) {
      m->att[j] = spellEffect_new(S_DMGSPELL);

      DMGPTR(m->att[j], spAttack) = mon->meleeatttype & 0x0f;
      DMGPTR(m->att[j], dieval) = 4;
      DMGPTR(m->att[j], ndice) = mon->basemelee;
      DMGPTR(m->att[j], attype) = 8;

      m->att[j]->type = ACT_MELEE;
      m->att[j]->subtype = mon->meleeatttype >> 4;
    } else if (type < 79) {
      m->att[j] = getSpellEffect(type);
      m->att[j]->type = ACT_SPELL;
    } else if ((type > 79) && (type < 96)) {
      m->att[j] = spellEffect_new(S_DMGSPELL);
      m->att[j]->type = ACT_BREATH;

      DMGPTR(m->att[j], dieval) = 4;
      DMGPTR(m->att[j], ndice) = spellDuration[type];
      DMGPTR(m->att[j], range) = spellRange[type];
      DMGPTR(m->att[j], isBreath) = 1;
      DMGPTR(m->att[j], attype) = spellAttr[type] >> 5;
      DMGPTR(m->att[j], group) = 1;
    } else if ((type > 97) && (type < 112)) {
      m->att[j] = spellEffect_new(S_DMGSPELL);
      m->att[j]->type = ACT_FIRE;
      m->att[j]->subtype = type - 98;

      DMGPTR(m->att[j], range) = spellRange[type];
      DMGPTR(m->att[j], dieval) = 4;
      DMGPTR(m->att[j], ndice) = spellDuration[type];
    } else if (type == 0xfe) {
      m->att[j] = spellEffect_new(S_GENERIC);
      m->att[j]->type = ACT_DOPPEL;

      GENPTR(m->att[j], type) = GENS_DOPPEL;
    } else if (type == 0xff) {
      m->att[j] = noSpellEffect();
      m->att[j]->type = ACT_NONE;
    }
  }
  printMonXML(2, m);

  monster_free(m);

}



void convertMonsters(void)
{
  int i, j;
  int fd;
  uint8_t buf[512];
  b2mon_t m;
  int index = -1;
  uint32_t fmon;

  cnv_printMonHeader();

  for (i = 0; i < 25; i++) {
    sprintf(buf, "lev%d-1.decomp", i);
    fd = xopen(buf, O_RDONLY);

    xread(fd, &m, sizeof(b2mon_t));
    fmon = 0;
    do {
      index++;
      for (j = 0; j < 16; j++) {
	if (m.name[j] & 0x80)
	  m.name[j] ^= 0x80;
	if ((m.name[j] == '/') || (m.name[j] == '\\'))
	  m.name[j] = '^';
	if (m.name[j] == 0x7f)
	  m.name[j] = '\0';
      }

      printMonster(&m, fmon, index);

      xread(fd, &m, sizeof(b2mon_t));
      fmon++;
    } while (m.name[0] != '\0');

    close(fd);
  }

  for (i = 0; i < 19; i++) {
    printMonster(&sumMons[i], i + 447, i + 447);
  }

  cnv_printMonFooter();
}

