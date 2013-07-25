#include <b1lib.h>
#include <mon.h>
#include <ctype.h>
#include <spell.h>
#include <cnv_mon.h>

#define DEBUG
#include <debug.h>

/********************************/
/*				*/
/* Local function prototypes	*/
/*				*/
/********************************/

static btstring_t *prepName(uint8_t * name, int pflag);
static void getSummonData(cnvList_t *mlist);

/********************************/
/*				*/
/* Local functions		*/
/*				*/
/********************************/

static void getSummonData(cnvList_t *mlist)
{
	monster_t *m;

	int i, j;

	for (i = 0; i < 3; i++) {
		m = monster_new();
		m->macro = bts_strcpy(monMacro[i + 127]);
		m->singular = bts_strcpy(sumlist[i].name);
		m->plural = bts_strcpy(sumlist[i].name);
		m->picture = bts_strcpy("PIC_NONE");

		m->hpRndNdice = 1;
		m->hpRndDie = sumlist[i].hpdice + 1;
		m->hpBase = sumlist[i].hpbase;
		m->reward = 0;
		m->baseAC = 10 - sumlist[i].ac;

		uint8_t strength = sumlist[i].strength & 0x1f;
#define CAP(x,y)	((x) > (y)) ? (y) : (x)
/*
    m->breathSaveLo = (sumlist[i].race >> 3) + 1;
    m->breathSaveHi = (sumlist[i].race >> 3) + 8;
*/
		getSavingThrow(sumlist[i].race, &m->breathSaveLo, &m->breathSaveHi);
		m->spellSaveLo = m->breathSaveLo;
		m->spellSaveHi = m->breathSaveHi;
		m->toHitLo = sumlist[i].ac;
		m->toHitHi = sumlist[i].ac;
		m->priorityLo = CAP(strength << 2, 0xff);
		m->priorityHi = CAP((strength << 2) + 31, 0xff);
#undef CAP
		m->rndGroupSize = 0;
		m->groupSize = 0;
		m->numAttacks = 1;
		m->pronoun = 0;
		m->willAdvance = 0;
		m->distance = 0;
		m->advSpeed = 0;
		m->noRandom = 1;
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

		for (j = 0; j < 4; j++) {
			monsterAttack_t *ma;

			if (sumlist[i].att[j] != 0xff) {
				bteAttack_t	*ba;

				ma = monsterAttack_new(ACT_MELEE);
				ma->action = btAction_new(FUNC_ATTACK, 
							EFFECT_ATTACK);
				ba = btEffect_attack(ma->action->effect);

				ba->meleeString = monAttType[sumlist[i].race] + 1;
				ba->dieval = DIEVAL((sumlist[i].strength & 0x1f) | 0x20);
				ba->ndice = NDICE((sumlist[i].strength & 0x1f) | 0x20);
				ba->spAttack = sumlist[i].strength >> 5;
				ba->attype = 8;

				cnvList_add(m->attacks, ma);
			}
		}

		cnvList_add(mlist, m);
	}
}

static btstring_t *prepName(uint8_t *inName, int pflag)
{
	uint8_t length = 0;
	uint8_t *name = inName;
	btstring_t *rval;

	if (*name == '\0')
		return;

	rval = bts_new(strlen(name) + 1);

	do {
		if (*name == '^') {
			name++;
			if (*name == '\0')
				break;

			if (pflag) {
				while (*name++ != '^');
				while (*name != '^')
					rval->buf[length++] = *name++;
			} else {
				while (*name != '^')
					rval->buf[length++] = *name++;
				name++;
				while (*name++ != '^');
			}
		} else {
/*			rval->buf[length++] = *name;*/
			rval->buf[length++] = *name++;
		}
/*	} while (*name++ != '\0');*/
	} while (*name != '\0');
	rval->buf[length++] = '\0';

	return bts_resize(rval, length);
}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

btstring_t *getMonsterName(uint32_t index)
{
	return bts_strcpy(monMacro[index]);
}

void convertMonsters(void)
{
	int i;
	int j;
	uint8_t str[16];
	cnvList_t *mlist;
	monster_t *m;

/*	cnv_printMonHeader();*/

	mlist = monList_new();

/*	for (i = 0; i < 127; i++) {*/
	for (i = 0; i < 126; i++) {
		m = monster_new();

		m->singular = prepName(monStr[i], 0);
		m->plural = prepName(monStr[i], 1);
		m->macro = bts_strcpy(monMacro[i]);
		m->picture = getPicMacro(monPicIndex[i]);

		m->hpRndNdice = 1;
		m->hpRndDie = diceMask[monMask[i] >> 4];
		m->hpBase = monAttrib[i] >> 4;
		m->reward = (monReward[i] >= 0x10) ? monReward[i] : monReward[i] << 8;
		m->baseAC = 10 - (monAttrib[i] & 0x1f);

		getSavingThrow(i + 1, &m->breathSaveLo, &m->breathSaveHi);
		getSavingThrow(i + 1, &m->spellSaveLo, &m->spellSaveHi);

#define CAP(x,y)	((x) > (y)) ? (y) : (x)
		m->toHitLo = (monPri[i] & 0x1f) >> 1;
		m->toHitHi = ((monPri[i] & 0x1f) >> 1) + 14;
		m->priorityLo = CAP(monPri[i] << 2, 0xff);
		m->priorityHi = CAP((monPri[i] << 2) + 31, 0xff);

		m->rndGroupSize = (i < 123) ? 1 : 0;
		if (monMask[i] & 7) {
			m->groupSize = CAP(diceMask[monMask[i] & 7], 99);
		} else {
			m->groupSize = 1;
		}
#undef CAP

		m->numAttacks = 1;
		m->pronoun = (monPronoun[monPicIndex[i]] != 0);
		m->willAdvance = 1;
		m->distance = 1;
		m->advSpeed = monPri[i] & 0x1f;
		m->noRandom = (i < 123) ? 0 : 1;
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

		switch (i) {
		case 9:
		case 19:
		case 29:
		case 50:
		case 59:
		case 64:
		case 106:
		case 109:
		case 123:
			m->repel.evil = 1;
			break;
		case 77:
		case 94:
		case 110:
		case 125:
			m->repel.demon = 1;
			break;
		}

		for (j = 0; j < 4; j++) {
			monsterAttack_t *ma = NULL;
			uint8_t att;

			att = monAttacks[(i * 4) + j];

			if (att == 0) {
				bteAttack_t *ba;

				ma = monsterAttack_new(ACT_MELEE);
				ma->action = btAction_new(FUNC_NONE,
							EFFECT_ATTACK);
				ba = btEffect_attack(ma->action->effect);

				ba->ndice = (monPri[i] & 0x1f) + 1;
				ba->dieval = 4;
				ba->spAttack = monPri[i] >> 5;
				ba->attype = 8;
				ba->meleeString = monAttType[i] + 1;
			} else if (att < 79) {
				ma = monsterAttack_new(ACT_SPELL);
				ma->action = getSpellAction(monSpellTrans[att]);
			} else if (att == 0xfe) {
				bteAttack_t *ba;
				ma = monsterAttack_new(ACT_DOPPEL);
				ma->action = btAction_new(FUNC_NONE,
							EFFECT_ATTACK);
				ba = btEffect_attack(ma->action->effect);
				ba->ndice = (monPri[i] & 0x1f) + 1;
				ba->dieval = 4;
				ba->spAttack = monPri[i] >> 5;
				ba->attype = 8;
				ba->meleeString = monAttType[i] + 1;
			} else {
				bteAttack_t *ba;

				ma = monsterAttack_new(ACT_BREATH);
				ma->action = btAction_new(FUNC_NONE,
							EFFECT_ATTACK);
				ba = btEffect_attack(ma->action->effect);

				ba->isBreath = 1;
				ba->ndice = spellDuration[att];
				ba->dieval = 4;
				ba->group = 1;
				ba->attype = spellAttr[att] >> 5;
			}

			cnvList_add(m->attacks, ma);
		}

		cnvList_add(mlist, m);
	}

	getSummonData(mlist);

	monList_to_json(mlist, mkJsonPath("monsters.json"));
	cnvList_free(mlist);
}
