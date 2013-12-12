#include <b2lib.h>
#include <cnv_mon.h>
#include <mon.h>
#include <fileio.h>
#include <endian.h>
#include <spell.h>

#define DEBUG
#include <debug.h>

/********************************/
/*				*/
/* Local function prototypes	*/
/*				*/
/********************************/

#if 0
static void printMonster(b2mon_t *mon, uint32_t fmon, int index);
#endif

static void		unmaskString(uint8_t *str, uint32_t size);
static uint8_t		willAdvance(b2mon_t *b);
static btstring_t	*prepName(uint8_t *name, int pflag);
static monster_t	*convertMonster(b2mon_t *inMonster);

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

static monster_t *convertMonster(b2mon_t *inMonster)
{
	monster_t	*m;
	int		i;

	for (i = 0; i < 16; i++) {
		if (inMonster->name[i] & 0x80)
			inMonster->name[i] ^= 0x80;
		if ((inMonster->name[i] == '/') || (inMonster->name[i] == '\\'))
			inMonster->name[i] = '^';
		if (inMonster->name[i] == 0x7f)
			inMonster->name[i] = '\0';
	}

	if (inMonster->name[0] == '\0')
		return NULL;
	
	m = monster_new();

	m->singular	= prepName(inMonster->name, 0);
	m->plural	= prepName(inMonster->name, 1);
	m->picture	= getPicMacro(inMonster->picindex);
	m->reward	= ((inMonster->numattacks + 1) * inMonster->basemelee) << 7;

	m->hpRndNdice	= 1;
	m->hpRndDie	= inMonster->hpRnd;
	m->hpBase	= inMonster->hpBase;

	m->groupSize	= (inMonster->maxgrp << 1) + 1;
	m->distance	= (inMonster->flags >> 4) * 10;
	m->advSpeed	= inMonster->advancespeed * 10;
	m->willAdvance	= willAdvance(inMonster);
	m->baseAC	= 10 - inMonster->baseac2hit;

	m->breathSaveLo = inMonster->basemelee >> 3;
	m->breathSaveHi = (inMonster->basemelee >> 3) + 8;
	m->spellSaveLo	= m->breathSaveLo;
	m->spellSaveHi	= m->breathSaveHi;
	m->toHitLo	= inMonster->baseac2hit;
	m->toHitHi	= inMonster->baseac2hit + 3;
	m->pronoun	= (inMonster->flags & 1) ? 0 : 1;

#define CAP(x,y)  ((x) > (y)) ? (y) : (x)
	m->priorityLo	= CAP(((inMonster->basemelee << 2) | 1), 0xff);
	m->priorityLo	= CAP((((inMonster->basemelee << 2) | 1) + 31), 0xff);
#undef CAP

	switch (inMonster->picindex) {
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

	for (i = 0; i < 4; i++) {
		monsterAttack_t	*ma = NULL;
		uint8_t		att;

		att = inMonster->attTypes[i];

		if (att == 0) {
			bteAttack_t	*ba;

			ma = monsterAttack_new(ACT_MELEE);
			ma->action = btAction_new(FUNC_NONE, EFFECT_ATTACK);
			ba = btEffect_attack(ma->action->effect);

			ba->dieval	= 4;
			ba->ndice	= inMonster->basemelee;
			ba->spAttack	= inMonster->meleeatttype & 0x0f;
			if (ba->spAttack > 9)
				ba->spAttack = 0;
			ba->attype	= 8;
			ba->meleeString	= ((inMonster->meleeatttype & 0xf0) >> 4) + 1;
		} else if (att < 79) {
			ma = monsterAttack_new(ACT_SPELL);
			ma->action = getSpellAction(att);
		} else if ((att > 79) && (att < 96)) {
			bteAttack_t	*ba;

			ma = monsterAttack_new(ACT_BREATH);
			ma->action = btAction_new(FUNC_NONE, EFFECT_ATTACK);

			ba = btEffect_attack(ma->action->effect);

			ba->isBreath	= 1;
			ba->dieval	= 4;
			ba->group	= 1;
			ba->ndice	= spellDuration[att];
			ba->attype	= spellAttr[att] >> 5;
			ba->range	= spellRange[att] * 10;
		} else if ((att > 97) && (att < 112)) {
			bteAttack_t	*ba;

			ma = monsterAttack_new(ACT_FIRE);
			ma->action = btAction_new(FUNC_NONE, EFFECT_ATTACK);
			ba = btEffect_attack(ma->action->effect);

			ba->dieval	= 4;
			ba->range	= spellRange[att] * 10;
			ba->ndice	= spellDuration[att];
			ba->attype	= 8;
			ba->fireString	= fireConv[att - 98];
		} else if (att == 0xfe) {
			bteAttack_t	*ba;

			ma = monsterAttack_new(ACT_DOPPEL);
			ma->action = btAction_new(FUNC_NONE, EFFECT_ATTACK);
			ba = btEffect_attack(ma->action->effect);

			ba->dieval	= 4;
			ba->ndice	= inMonster->basemelee;
			ba->spAttack	= inMonster->meleeatttype & 0x0f;
			if (ba->spAttack > 9)
				ba->spAttack = 0;
			ba->attype	= 8;
			ba->meleeString	= ((inMonster->meleeatttype & 0xf0) >> 4) + 1;
		} else if (att == 0xff) {
			debug("No attack\n");
		} else {
			debug("Attack not covered: 0x%02x\n", att);
		}

		if (ma != NULL)
			cnvList_add(m->attacks, ma);
	}
	
	return m;
}

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
	uint8_t		length = 0;
	btstring_t	*from;
	btstring_t	*rval;
	uint8_t		*bufp;

	if (*name == '\0')
		return;

	from = bts_sprintf("%.16s", name);
	rval = bts_new(17);
	bufp = from->buf;

	do {
		if (*bufp == '^') {
			bufp++;
			if (*bufp == '\0')
				break;

			if (pflag) {
				while (*bufp++ != '^');
				while ((*bufp != '^') && (*bufp != '\0'))
					rval->buf[length++] = *bufp++;
			} else {
				while (*bufp != '^')
					rval->buf[length++] = *bufp++;
				bufp++;
				while ((*bufp != '^') && (*bufp != '\0'))
					bufp++;
			}
		} else {
			rval->buf[length++] = *bufp++;
		}
	} while (*bufp != '\0');
	rval->buf[length++] = '\0';

	bts_free(from);

	return bts_resize(rval, length);
}

static void unmaskString(uint8_t *str, uint32_t size)
{
	uint32_t	i;

	for (i = 0; i < size; i++) {
		if (str[i] & 0x80)
			str[i] ^= 0x80;
		if ((str[i] == '/') || (str[i] == '\\'))
			str[i] = '^';
		if (str[i] == 0x7f)
			str[i] = '\0';
	}
}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

btstring_t *getSummonMacro(uint32_t index)
{
	return bts_sprintf("%s_%d", sumMons[index].name, M_SUMMON);
}

#if 0
void monsterHeader(void)
{
	int i;

	for (i = 0; i < 466; i++)
		printf("#define %s\t%d\n", monMacro[i], i);
}

static void printMonster(b2mon_t * mon, uint32_t fmon, int index)
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
	m->priorityLo = CAP(((mon->basemelee << 2) | 1), 0xff);
	m->priorityLo = CAP((((mon->basemelee << 2) | 1) + 31), 0xff);
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

		if (type == 55)
			type = 61;

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

#endif

void getMonsterNameList(cnvList_t *list, btstring_t *monList, uint8_t level)
{
	uint32_t	numMonsters;
	uint32_t	i, j;
	b2mon_t		*bm;
	btstring_t	*singular;

	numMonsters = monList->size / sizeof(b2mon_t);
	for (i = 0; i < numMonsters; i++) {
		bm = (b2mon_t *)(&(monList->buf[i * sizeof(b2mon_t)]));
		if (bm->name[0] == '\0')
			break;
		for (j = 0; j < 16; j++) {
			if (bm->name[j] & 0x80)
				bm->name[j] ^= 0x80;
			if ((bm->name[j] == '/') || (bm->name[j] == '\\'))
				bm->name[j] = '^';
			if (bm->name[j] == 0x7f)
				bm->name[j] = '\0';
		}
		singular = prepName(bm->name, 0);
		cnvList_add(list, bts_sprintf("%s_%d", singular->buf, level));
		bts_free(singular);
	}
}

btstring_t *getMonsterByIndex(btstring_t *monList, uint32_t index, uint8_t level)
{
	uint32_t	numMonsters;
	uint32_t	i, j;
	b2mon_t		*bm;
	btstring_t	*singular;
	btstring_t	*rval;

	numMonsters = monList->size / sizeof(b2mon_t);
	if (index > numMonsters)
		return NULL;

	bm = (b2mon_t *)(&(monList->buf[index * sizeof(b2mon_t)]));
	unmaskString(bm->name, 16);
	if (bm->name[0] == '\0')
		return NULL;
	singular = prepName(bm->name, 0);
	rval = bts_sprintf("%.*s_%d", singular->size, singular->buf, level);
	bts_free(singular);
	return rval;
}


void convertMonsters(void)
{
	int		i, j, k;
	uint32_t	numLevels;
	uint32_t	numMonsters;
	btstring_t	*monData;
	cnvList_t	*monsters;
	b2mon_t		*bm;
	monster_t	*m;

	monsters = monList_new();

	numLevels = getMaxLevels();
	debug("numLevels = %d\n", numLevels);

	for (i = 0; i < numLevels; i++) {
		monData = getMonsters(i);

		numMonsters = monData->size / sizeof(b2mon_t);
		for (j = 0; j < numMonsters; j++) {
			bm = (b2mon_t *)(&(monData->buf[j * sizeof(b2mon_t)]));

			m = convertMonster(bm);
			if (m == NULL)
				break;

			m->macro = bts_sprintf("%s_%d", m->singular->buf, i);
			m->rndGroupSize = (j < 16) ? 1 : 0;
			m->noRandom	= (j < 16) ? 0 : 1;

			cnvList_add(monsters, m);
		}

		bts_free(monData);
	}

	monData = getWildMonsters();
	numMonsters = monData->size / sizeof(b2mon_t);
	for (j = 0; j < numMonsters; j++) {
		bm = (b2mon_t *)(&(monData->buf[j * sizeof(b2mon_t)]));

		m = convertMonster(bm);
		if (m == NULL)
			break;

		m->macro = bts_sprintf("%s_%d", m->singular->buf, M_WILD);
		m->rndGroupSize = (j < 16) ? 1 : 0;
		m->noRandom	= (j < 16) ? 0 : 1;
		cnvList_add(monsters, m);
	}
	bts_free(monData);

	i = 0;
	while (sumMons[i].hpRnd) {
		m = convertMonster(&sumMons[i]);
		if (m == NULL)
			break;

		m->macro = bts_sprintf("%s_%d", m->singular->buf, M_SUMMON);
		cnvList_add(monsters, m);
		i++;
	}

	monList_to_json(monsters, mkJsonPath("monsters.json"));
	cnvList_free(monsters);
}
