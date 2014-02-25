#include <b2lib.h>
#include <spell.h>
#include <cnv_spell.h>
#include <cnv_song.h>

#define DEBUG
#include <debug.h>

/********************************/
/*				*/
/* Local function prototypes	*/
/*				*/
/********************************/

static btAction_t	*spf_batbonus(uint32_t index);
static btAction_t	*spf_damagespell(uint32_t index);
static btAction_t	*spf_disbelieve(uint32_t index);
static btAction_t	*spf_generic(uint32_t index);
static btAction_t	*spf_passiveeffect(uint32_t index);
static btAction_t	*spf_rangespell(uint32_t index);
static btAction_t	*spf_wordofhealing(uint32_t index);
static btAction_t	*spf_summon(uint32_t index);
static btAction_t	*spf_quickfix(uint32_t index);

/********************************/
/*				*/
/* Static Data			*/
/*				*/
/********************************/

typedef btAction_t *(spellFunction_t) (uint32_t index);

static spellFunction_t *spellFunctions[] = {
	spf_passiveeffect, spf_damagespell, spf_batbonus, spf_generic,
	spf_batbonus, spf_passiveeffect, spf_batbonus, spf_wordofhealing,
	NULL, spf_passiveeffect, spf_damagespell, spf_summon,
	spf_damagespell, spf_generic, spf_batbonus, spf_disbelieve,
	spf_batbonus, spf_passiveeffect, spf_damagespell, spf_batbonus,
	spf_generic, spf_damagespell, spf_passiveeffect, spf_damagespell,
	spf_batbonus, spf_generic, spf_generic, spf_rangespell,
	spf_quickfix, spf_generic, spf_generic, spf_generic,
	spf_rangespell, spf_generic, spf_generic
};

static uint8_t genSpell[] = {
	FUNC_NONE, FUNC_NONE, FUNC_NONE, FUNC_TRAPZAP,
	FUNC_NONE, FUNC_NONE, FUNC_NONE, FUNC_NONE,
	FUNC_MAGESTAR, FUNC_NONE, FUNC_NONE, FUNC_NONE,
	FUNC_NONE, FUNC_TELEPORT, FUNC_NONE, FUNC_NONE,
	FUNC_NONE, FUNC_NONE, FUNC_NONE, FUNC_NONE,
	FUNC_SCRYSITE, FUNC_NONE, FUNC_NONE, FUNC_NONE,
	FUNC_NONE, FUNC_PHASEDOOR, FUNC_SPELLBIND, FUNC_NONE,
	FUNC_NONE, FUNC_SAFETYSPELL, FUNC_DREAMSPELL, FUNC_MAGESTAR,
	FUNC_NONE, FUNC_BATCHSPELL, FUNC_CAMARADERIE
};

/********************************/
/*				*/
/* Local functions		*/
/*				*/
/********************************/

/*
 * spf_batbonus()
 */
static btAction_t *spf_batbonus(uint32_t index)
{
	btAction_t	*rval;
	bteBonus_t	*bb;

	rval = btAction_new(FUNC_BONUS, EFFECT_BONUS);
	bb = btEffect_bonus(rval->effect);

	switch (spellType[index]) {
	case sp_AntiMagic:
		bb->group	= 1;
		bb->antiMagic	= 2;
		break;
	case sp_BattleSkill:
		bb->damageBonus	= 1;
		bb->damageStack	= 1;
		bb->damageAmount= battleskill[spellDuration[index]];
		break;
	case sp_FreezeFoes:
		bb->group	= 1;
		bb->acPenalty	= 1;
		bb->acStack	= 1;
		bb->acAmount	= 1;
		break;
	case sp_PhaseBlur:
		bb->group	= 1;
		bb->acBonus	= 1;
		bb->acStack	= 1;
		bb->acAmount	= phaseBlur[spellDuration[index]];
		break;
	case sp_VorpalPlating:
		bb->damageBonus	= 1;
		bb->damageStack	= 1;
		bb->damageRandom= 1;
		bb->damageAmount= spellDuration[index];
		bb->damageDice	= 8;
		break;
	case sp_WordOfFear:
		bb->group	= 1;

		bb->toHitPenalty= 1;
		bb->toHitStack	= 1;
		bb->toHitAmount	= wordoffear[spellDuration[index]];

		bb->damagePenalty= 1;
		bb->damageStack	= 1;
		bb->damageAmount= wordoffear[spellDuration[index]];

		break;
	}

	return rval;
}

/*
 * spf_disbelieve(index)
 */
static btAction_t *spf_disbelieve(uint32_t index)
{
	btAction_t	*rval;
	bteDisbelieve_t	*bd;

	rval = btAction_new(FUNC_DISBELIEVE, EFFECT_DISBELIEVE);
	bd = btEffect_disbelieve(rval->effect);

	bd->disbelieve = 1;

	if (spellDuration[index] == 0xff) {
		bd->allBattle = 1;
		bd->revealDoppelganger = 1;
	}

	return rval;
}

/*
 * spf_damagespell()
 */
static btAction_t *spf_damagespell(uint32_t index)
{
	btAction_t	*rval;
	bteAttack_t	*ba;

	rval = btAction_new(FUNC_ATTACK, EFFECT_ATTACK);
	ba = btEffect_attack(rval->effect);

	ba->dieval = 4;
	ba->attype = 10;

	if (spellType[index] == sp_StoneTouch) {
		ba->spAttack = spellDuration[index];
	} else {
		ba->ndice = spellDuration[index];
		if (index >= 96) {
			ba->attype = 8;
		} else {
			ba->attype = spellAttr[index] >> 5;
		}

		switch (spellType[index]) {
		case sp_ArcFire:
			ba->levelMult = 1;
			break;
		case sp_WarStrike:
			ba->group = 1;
			break;
		case sp_SpectreTouch:
			ba->ndice++;
			break;
		case sp_MindBlade:
			ba->allFoes = 1;
			break;
		case sp_HolyWater:
			if (index == 55) {
				ba->group = 1;
				ba->rflags.spellcaster = 1;
				btFunction_free(rval->function);
				rval->function = btFunction_new(
					FUNC_STRING,
					bts_strcpy(magmString)
					);
			} else {
				ba->rflags.evil = 1;
			}
			break;
		}
	}

	ba->outOfRange = IFBIT(spellRange[index], 0x80, 1, 0);
	ba->range = spellRange[index] & 0x7f;

	return rval;
}

/*
 * spf_generic()
 *
 */
static btAction_t *spf_generic(uint32_t index)
{
	btAction_t	*rval;
	bteGeneric_t	*bg;

	rval = btAction_new(genSpell[spellType[index]], EFFECT_GENERIC);
	bg = btEffect_generic(rval->effect);

	bg->type = genSpell[spellType[index]];

	return rval;
}

/*
 * spf_passiveeffect()
 */
static btAction_t *spf_passiveeffect(uint32_t index)
{
	btAction_t	*rval;
	btePassive_t	*bp;

	rval = btAction_new(FUNC_NONE, EFFECT_PASSIVE);
	bp = btEffect_passive(rval->effect);

	bp->duration = spellDuration[index];

	switch (spellType[index]) {
	case sp_Lightspell:
		rval->function->type = FUNC_LIGHT;
		bp->type = PASS_LIGHT;
		bp->duration = lightDur[spellDuration[index]];
		bp->lightDistance = lightDist[spellDuration[index]];
		bp->detectSecret = (lightSDFlag[spellDuration[index]] != 0);
		break;
	case sp_MagicCompass:
		rval->function->type = FUNC_COMPASS;
		bp->type = PASS_COMPASS;
		break;
	case sp_Levitation:
		rval->function->type = FUNC_LEVITATE;
		bp->duration = levitDur[spellDuration[index]];
		bp->type = PASS_LEVITATE;
		break;
	case sp_GroupShield:
		rval->function->type = FUNC_SHIELD;
		bp->type = PASS_SHIELD;
		bp->acBonus = 2;
		break;
	case sp_AreaEnchant:
	{
		uint8_t	effect = spellAttr[index] >> 6;

		rval->function->type = FUNC_DETECT;
		bp->type = PASS_DETECT;
		if (effect == 0) {
			bp->detectTraps = 1;
		} else if (effect == 1) {
			bp->detectStairs = 1;
		} else {
			bp->detectTraps = 1;
			bp->detectStairs = 1;
			bp->detectSpecial = 1;
		}
		break;
	}
	}

	return rval;
}

/*
 * spf_quickfix()
 */
static btAction_t *spf_quickfix(uint32_t index)
{
	btAction_t	*rval;
	bteHeal_t	*bh;

	rval	= btAction_new(FUNC_HEAL, EFFECT_HEAL);
	bh	= btEffect_heal(rval->effect);

	bh->ndice = 8;

	return rval;
}

/*
 * spf_rangespell()
 */
static btAction_t *spf_rangespell(uint32_t index)
{
	btAction_t	*rval;
	bteAttack_t	*ba;

	debug("spf_rangespell(%d)\n", index);

	rval	= btAction_new(FUNC_RANGE, EFFECT_ATTACK);
	ba	= btEffect_attack(rval->effect);

	switch (spellType[index]) {
	case sp_FarFoe:
		ba->addDistance = 1;
		ba->distance	= 40;
		break;
	case sp_MeleeMen:
		ba->distance	= 10;
		break;
	}

	return rval;
}

/* 
 * spf_wordofhealing(index)
 */
static btAction_t *spf_wordofhealing(uint32_t index)
{
	uint8_t		mask;
	btAction_t	*rval;
	bteHeal_t	*bh;

	rval	= btAction_new(FUNC_HEAL, EFFECT_HEAL);
	bh	= btEffect_heal(rval->effect);

	bh->randomHeal	= 1;
	bh->groupHeal	= IFBIT(spellDuration[index], 0x80, 1, 0);
	bh->ndice	= spellDuration[index] & 0x0f;
	if (bh->ndice == 0x0f) {
		bh->ndice	= 0;
		bh->fullHeal	= 1;
	}
	if (bh->ndice == 0) {
		bh->randomHeal == 0;
	}

	mask = spellAttr[index] & 0xe0;

	if (mask == 0xc0)
		bh->dispossess	= 1;
	if (mask == 0xe0)
		bh->resurrect	= 1;
	if (mask == 0xa0) {
		bh->resurrect	= 1;
		bh->old		= 1;
		bh->poison	= 1;
		bh->paralysis	= 1;
		bh->insanity	= 1;
		bh->dispossess	= 1;
		bh->stoned	= 1;
	}
	if (mask == 0x80) {
		bh->poison	= 1;
		bh->paralysis	= 1;
		bh->insanity	= 1;
	}
	bh->dieval = 4;

	return rval;
}

/*
 * spf_summon(index)
 * Parameters
 *   monIndex1
 *   monIndex2
 *   illusionFlag
 */
static btAction_t *spf_summon(uint32_t index)
{
	uint8_t		sum;
	btAction_t	*rval;
	bteSummon_t	*bs;

	rval	= btAction_new(FUNC_SUMMON, EFFECT_SUMMON);
	bs	= btEffect_summon(rval->effect);

	if (index == 77)
		bs->fillParty = 1;
	else
		bs->fillParty = 0;

	bs->isIllusion = IFBIT(spellDuration[index], 0x80, 1, 0);

	sum = spellSummonMap[spellDuration[index] & 0x7f];
	cnvList_add(bs->monsters, getSummonMacro(sum));

	return rval;
}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

btAction_t *getSpellAction(uint32_t spell)
{
	uint8_t		type;

	type = spellType[spell];

	if (type == sp_None)
		return NULL;

	if (spellFunctions[type] != NULL)
		return spellFunctions[type](spell);

	return NULL;
}

btAction_t *getSummonEffect(uint32_t spell)
{
	return spf_summon(spell);
}

#if 0
btAction_t *getSpellEffect(uint32_t spell)
{
	uint8_t sptype;

	sptype = spellType[spell];

	if (sptype == sp_None)
		return 0;

	return spFuncs[sptype] (spell);
}

uint8_t *getSpellName(uint8_t spno)
{
	return spellFull[spno];
}

btAction_t *noSpellEffect(void)
{
	btAction_t *rsp;

	rsp = (btAction_t *) xmalloc(sizeof(btAction_t));
	rsp->type = S_NONE;
	rsp->effect = NULL;

	return rsp;
}
#endif

void getTargetting(uint32_t index, btTargetting_t *bt)
{
	if ((spellAttr[index] & 7) >= 4) {
		bt->targetted = 0;
	} else {
		bt->targetted = 1;
		switch (spellAttr[index] & 7) {
		case 3:
			bt->targetMonster = 1;
		case 1:
		case 0:
			bt->targetParty = 1;
			break;
		case 4:
		case 2:
			bt->targetMonster = 1;
			break;
		}
	}
}

void convertSpells(void)
{
	int		i;
	mageSpell_t	*m;
	cnvList_t	*spells;

	spells = spellList_new();

	for (i = 0; i < 79; i++) {
		m = mageSpell_new(0);

		m->class= bts_strcpy(spellClass[i]);
		m->level= spellLevel[i];
		m->abbr	= bts_strcpy(spellAbbr[i]);
		m->full = bts_strcpy(spellFull[i]);
		m->sppt = spptReq[i];

		m->combat = IFBIT(spellAttr[i], 8, 1, 0);
		m->noncombat = IFBIT(spellAttr[i], 0x10, 1, 0);
		getTargetting(i, &m->targetting);

		m->action = getSpellAction(i);

		cnvList_add(spells, m);
	}

	spellList_to_json(spells, mkJsonPath("spells.json"));
	cnvList_free(spells);

#if 0
	printf("static main() {\n");
	printf("auto eid;\n");
	printf("eid = GetEnum(\"spellName\");\n");
	for (i = 0; i < 79; i++) {
		printf("AddConstEx(eid, \"spell_%s\", %d, -1);\n",
			spellAbbr[i], i);
	}
	printf("}");
#endif
}
