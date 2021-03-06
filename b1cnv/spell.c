#include <b1lib.h>
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

static btAction_t	*spell_attack(uint32_t index);
static btAction_t	*spell_passive(uint32_t index);
static btAction_t	*spell_disbelieve(uint32_t index);
static btAction_t	*spell_bonus(uint32_t index);
static btAction_t	*spell_heal(uint32_t index);
static btAction_t	*spell_summon(uint32_t index);
static btAction_t	*spell_generic(uint32_t index);

static void song_lightSpell(btSong_t *bs);
static void song_generic(btSong_t *bs);

/********************************/
/*				*/
/* Static data			*/
/*				*/
/********************************/

typedef btAction_t *(spellFunction_t)(uint32_t index);

static spellFunction_t *newSpellFunctions[] = {
	spell_passive, spell_attack, spell_bonus, spell_generic,
	spell_bonus, spell_passive, spell_bonus, spell_heal,
	spell_generic, spell_passive, spell_attack, spell_summon,
	spell_attack, spell_generic, spell_bonus, spell_disbelieve,
	spell_bonus, spell_passive, spell_attack, spell_bonus,
	spell_generic, spell_attack, spell_passive, spell_attack,
	spell_bonus, spell_generic, spell_generic
};

static uint8_t genSpell[] = {
	FUNC_NONE, FUNC_NONE, FUNC_NONE, FUNC_TRAPZAP,
	FUNC_NONE, FUNC_NONE, FUNC_NONE, FUNC_NONE,
	FUNC_MAGESTAR, FUNC_NONE, FUNC_NONE, FUNC_NONE,
	FUNC_NONE, FUNC_TELEPORT, FUNC_NONE, FUNC_NONE,
	FUNC_NONE, FUNC_NONE, FUNC_NONE, FUNC_NONE,
	FUNC_SCRYSITE, FUNC_NONE, FUNC_NONE, FUNC_NONE,
	FUNC_NONE, FUNC_PHASEDOOR, FUNC_SPELLBIND
};

/********************************/
/*				*/
/* Local functions		*/
/*				*/
/********************************/

static btAction_t *spell_generic(uint32_t index)
{
	btAction_t *rval;
	bteGeneric_t *bg;

	rval = btAction_new(genSpell[spellType[index]], EFFECT_GENERIC);
	bg = btEffect_generic(rval->effect);

	bg->type = genSpell[spellType[index]];

	return rval;
}

static btAction_t *spell_passive(uint32_t index)
{
	btAction_t	*rval;
	btePassive_t	*bp;

	rval = btAction_new(FUNC_NONE, EFFECT_PASSIVE);
	bp = btEffect_passive(rval->effect);

	bp->duration = spellDuration[index];

	switch (spellType[index]) {
	case sp_LightSpell:
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
	case sp_LocateTraps:
	{
		uint8_t effect = spellAttr[index] >> 6;

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
	case sp_GroupShield:
		rval->function->type = FUNC_SHIELD;
		bp->type = PASS_SHIELD;
		bp->acBonus = 2;
		break;
	}

	return rval;
}

static btAction_t *spell_bonus(uint32_t index)
{
	btAction_t	*rval;
	bteBonus_t	*bb;

	rval = btAction_new(FUNC_BONUS, EFFECT_BONUS);
	bb = btEffect_bonus(rval->effect);

	switch (spellType[index]) {
	case sp_AntiMagic:
		bb->group = 1;
		bb->antiMagic = 2;
		break;
	case sp_BattleSkill:
		bb->damageBonus = 1;
		bb->damageStack = 1;
		bb->damageAmount = battleSkill[spellDuration[index]];
		break;
	case sp_FreezeFoes:
		bb->group = 1;
		bb->acPenalty = 1;
		bb->acStack = 1;
		bb->acAmount = 1;
		break;
	case sp_PhaseBlur:
		bb->group = 1;
		bb->acBonus = 1;
		bb->acStack = 1;
		bb->acAmount = phaseBlur[spellDuration[index]];
		break;
	case sp_ShieldSelf:
		bb->acBonus = 1;
		bb->acStack = 1;
		bb->acAmount = shieldSelf[spellDuration[index]];
		break;
	case sp_WordOfFear:
		bb->group = 1;
		bb->toHitPenalty = 1;
		bb->toHitStack = 1;
		bb->toHitAmount = wordoffear[spellDuration[index]];
		bb->damagePenalty = 1;
		bb->damageStack = 1;
		bb->damageAmount = wordoffear[spellDuration[index]];
		break;
	case sp_VorpalPlating:
#if 0
		bb->toHitBonus = 1;
		bb->toHitStack = 1;
		bb->toHitRandom = 1;
		bb->toHitAmount = spellDuration[index];
		bb->toHitDice = 8;
#endif
		bb->damageBonus = 1;
		bb->damageStack = 1;
		bb->damageRandom = 1;
		bb->damageAmount = spellDuration[index];
		bb->damageDice = 8;
		break;
	}

	return rval;
}

static btAction_t *spell_attack(uint32_t index)
{
	btAction_t	*rval;
	bteAttack_t	*ba;

	rval = btAction_new(FUNC_ATTACK, EFFECT_ATTACK);
	ba = btEffect_attack(rval->effect);

	ba->dieval	= 4;
	ba->attype	= 10;

	if (spellType[index] == sp_PoisonStrike) {
		ba->spAttack = spellDuration[index];
	} else {
		ba->ndice = spellDuration[index];
		ba->attype = spellAttr[index] >> 5;

		switch (spellType[index]) {
		case sp_ArcFire:
			ba->levelMult = 1;
			break;
		case sp_WarStrike:
			ba->group = 1;
			break;
		case sp_HolyWater:
			if ((index == 75) || (index == 67))
				ba->group = 1;
			if (index < 69)
				ba->rflags.evil = 1;
			else
				ba->rflags.demon = 1;
			break;
		case sp_MindBlade:
			ba->allFoes = 1;
			break;
		case sp_SpectreTouch:
			ba->ndice++;
			break;
		}
	}

	return rval;
}

static btAction_t *spell_heal(uint32_t index)
{
	uint8_t		mask;
	btAction_t	*rval;
	bteHeal_t	*bh;

	rval = btAction_new(FUNC_HEAL, EFFECT_HEAL);
	bh = btEffect_heal(rval->effect);

	bh->randomHeal = 1;
	bh->groupHeal = IFBIT(spellDuration[index], 0x80, 1, 0);
	bh->ndice = spellDuration[index] & 0x0f;
	if (bh->ndice == 0x0f) {
		bh->ndice = 0;
		bh->fullHeal = 1;
	}
	if (bh->ndice == 0) {
		bh->randomHeal = 0;
	}

	mask = spellAttr[index] & 0xe0;

	if (mask == 0xc0)
		bh->dispossess = 1;
	if (mask == 0xe0)
		bh->resurrect = 1;
	if (mask == 0x80) {
		bh->poison = 1;
		bh->paralysis = 1;
		bh->insanity = 1;
	}
	bh->dieval = 4;

	return rval;
}
	

static btAction_t *spell_summon(uint32_t index)
{
	uint32_t	isRandom;
	uint8_t		sum;
	btAction_t	*rval;
	bteSummon_t	*bs;

	rval = btAction_new(FUNC_SUMMON, EFFECT_SUMMON);
	bs = btEffect_summon(rval->effect);

	isRandom = IFBIT(spellDuration[index], 0x40, 1, 0);
	bs->isIllusion = IFBIT(spellDuration[index], 0x80, 1, 0);

	sum = summonIndex[spellDuration[index] & 0x3f];
	sum = (sum > 4) ? sum - 1 : sum;
	sum = (sum == 0) ? 127 : sum;
	sum = (sum == 2) ? 128 : sum;
	sum = (sum == 4) ? 129 : sum;

	cnvList_add(bs->monsters, getMonsterName(sum));
	if (isRandom) {
		sum = summonIndex[(spellDuration[index] & 0x3f) + 1];
		sum = (sum > 4) ? sum - 1 : sum;
		sum = (sum == 0) ? 127 : sum;
		sum = (sum == 2) ? 128 : sum;
		sum = (sum == 4) ? 129 : sum;
		cnvList_add(bs->monsters, getMonsterName(sum));
	}

	return rval;
}

static btAction_t *spell_disbelieve(uint32_t index)
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

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

btAction_t *getSpellAction(uint32_t spell)
{
	uint8_t		type;

	type = spellType[spell];

	if (type == 255)
		return NULL;

	if (newSpellFunctions[type] != NULL)
		return newSpellFunctions[type](spell);

	return NULL;
}

uint8_t *getSpellName(uint8_t spno)
{
	return spellFull[spno];
}

void getTargetting(uint32_t index, btTargetting_t *bt)
{
	if ((spellAttr[index] & 7) >= 4) {
		bt->targetted = 0;
	} else {
		bt->targetted = 1;
		debug("Target value: %d\n", spellAttr[index] & 7);
		switch (spellAttr[index] & 7) {
		case 3:
			bt->targetMonster = 1;
			/* Fall through */
		case 1:
			bt->targetSummon = 1;
			/* Fall through */
		case 0:
			bt->targetParty = 1;
			break;
		case 4:
			bt->targetMelee = 1;
			/* Fall through */
		case 2:
			bt->targetMonster = 1;
			break;
		}
	}
}

void convertSpells(void)
{
	int i;
	mageSpell_t *m;
	cnvList_t *spells;

	spells = spellList_new();

	for (i = 0; i < 79; i++) {
		m = mageSpell_new(0);

		m->class = bts_strcpy(spellClass[i]);
		m->level = spellLevel[i];
		m->abbr = bts_strcpy(spellAbbr[i]);
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
}
