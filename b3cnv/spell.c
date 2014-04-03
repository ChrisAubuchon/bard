#include <b3lib.h>
#include <spell.h>
#include <cnv_spell.h>

#define DEBUG
#include <debug.h>

/********************************/
/*				*/
/* Local function prototypes	*/
/*				*/
/********************************/

static btAction_t	*spf_batbonus(uint32_t);
static btAction_t	*spf_disbelieve(uint32_t);
static btAction_t	*spf_dmgspell(uint32_t);
static btAction_t	*spf_generic(uint32_t);
static btAction_t	*spf_geomancer(uint32_t);
static btAction_t	*spf_healspell(uint32_t);
static btAction_t	*spf_passiveeffect(uint32_t);
static btAction_t	*spf_rangespell(uint32_t);
static btAction_t	*spf_summon(uint32_t);
static btAction_t	*spf_batchspell(uint32_t);

static btFunction_t	*getFunction(uint32_t type);

/********************************/
/*				*/
/* Local data			*/
/*				*/
/********************************/

static spellFunction_t *spellFunctions[] = {
/* 0*/ spf_generic, spf_generic, NULL, NULL, NULL,
/* 5*/ spf_generic, spf_batbonus, spf_batbonus, spf_passiveeffect, spf_batchspell,
/*10*/ spf_generic, spf_passiveeffect, spf_dmgspell, spf_disbelieve, spf_generic,
/*15*/ spf_generic, spf_rangespell, spf_batbonus, spf_geomancer, spf_generic,
/*20*/ spf_healspell, spf_generic, spf_passiveeffect, spf_passiveeffect, spf_batbonus,
/*25*/ spf_rangespell, spf_generic, spf_generic, spf_generic, spf_passiveeffect,
/*30*/ spf_generic, spf_batbonus, spf_summon, spf_generic, spf_generic,
/*35*/ spf_batbonus, spf_batbonus, spf_generic, spf_generic, spf_generic,
/*40*/ spf_generic
};

/********************************/
/*				*/
/* Local function prototypes	*/
/*				*/
/********************************/

/*
 * getFunction()
 */
static btFunction_t *getFunction(uint32_t type)
{
	btFunction_t	*rval;

	rval = btFunction_new(FUNC_STRING, bts_strcpy(spellStrings[type]));

	return rval;
}

/*
 * spf_batbonus()
 */
static btAction_t *spf_batbonus(uint32_t index)
{
	btAction_t	*rval;
	uint8_t		type;

	rval		= btAction_new(FUNC_NONE, EFFECT_NONE);
	rval->pl	= paramList_new();

	type = spellType[index];

	param_add(rval->pl, PARAM_NUMBER, "amount", spellDuration[index]);

	switch (type) {
	case sp_acBonus:
	case sp_antiMagic:
	case sp_vorpalPlating:
	case sp_wordOfFear:
		rval->function = getFunction(type);
		break;
	case sp_freezeFoes:
	case sp_luckSpell:
	case sp_strengthBonus:
		rval->function = getFunction(type);

		break;
	}

	return rval;
}


/*
 * spf_disbelieve()
 */
static btAction_t *spf_disbelieve(uint32_t index)
{
	btAction_t	*rval;
	uint8_t		dur;

	rval		= btAction_new(FUNC_NONE, EFFECT_NONE);
	rval->function	= getFunction(sp_disbelieve);
	rval->pl	= paramList_new();

	dur = spellDuration[index];

	param_add(rval->pl, PARAM_BOOL, "disbelieve", IFBIT(dur, 0x81, 1, 0));
	param_add(rval->pl, PARAM_BOOL, "allBattle", IFBIT(dur, 0xc0, 1, 0));
	param_add(rval->pl, PARAM_BOOL, "reveal", IFBIT(dur, 0x80, 1, 0));
	param_add(rval->pl, PARAM_BOOL, "noSummon", IFBIT(dur, 0x40, 1, 0));

	return rval;
}

/*
 * spf_dmgspell()
 */
static btAction_t *spf_dmgspell(uint32_t i)
{
	breathAtt_t *b;

	b = &dmgspells[(spellDuration[i] / 7)];

	return cnvBreathAttack(b, spellRange[i]);
}

/*
 * cnvBreathAttack()
 * Convert a breathAtt_t structure to a spell effect
 */
btAction_t *cnvBreathAttack(breathAtt_t *b, uint8_t range)
{
	btAction_t	*rval;
	bteAttack_t	a;

	memset(&a, 0, sizeof(bteAttack_t));

	rval = btAction_new(FUNC_NONE, EFFECT_NONE);
	rval->function = getFunction(sp_damageSpell);

	a.allFoes	= IFBIT(b->targetFlags, 0x80, 1, 0);
	a.group		= IFBIT(b->targetFlags, 0x40, 1, 0);
	a.levelMult	= (b->levelMult == 0);
	a.isSpell	= IFBIT(b->targetFlags, 2, 1, 0);
	a.isBreath	= IFBIT(b->targetFlags, 1, 1, 0);
	a.spAttack	= b->effect;
	
	a.outOfRange	= IFBIT(range, 0x80, 1, 0);
	a.range		= range & 0x7f;
	a.attype	= (b->breathFlag & 0xfe) >> 1;
	if (b->damage) {
		a.ndice = NDICE(b->damage) * ((b->levelMult != 0) ? b->levelMult : 1);
		a.dieval = DIEVAL(b->damage);
	}

	a.rflags.evil		= IFBIT(b->repelFlags, 0x80, 1, 0);
	a.rflags.demon		= IFBIT(b->repelFlags, 0x40, 1, 0);
	a.rflags.spellcaster	= IFBIT(b->repelFlags, 0x20, 1, 0);
	a.rflags.unk1		= IFBIT(b->repelFlags, 0x10, 1, 0);
	a.rflags.unk2		= IFBIT(b->repelFlags, 0x08, 1, 0);
	a.rflags.unk3		= IFBIT(b->repelFlags, 0x04, 1, 0);
	a.rflags.unk4		= IFBIT(b->repelFlags, 0x02, 1, 0);
	a.rflags.unk5		= IFBIT(b->repelFlags, 0x01, 1, 0);

	a.elem.fire		= IFBIT(b->elements, 0x80, 1, 0);
	a.elem.unk1		= IFBIT(b->elements, 0x40, 1, 0);
	a.elem.holy		= IFBIT(b->elements, 0x20, 1, 0);
	a.elem.ice		= IFBIT(b->elements, 0x10, 1, 0);
	a.elem.lightning	= IFBIT(b->elements, 0x08, 1, 0);
	a.elem.spell		= IFBIT(b->elements, 0x04, 1, 0);
	a.elem.unk2		= IFBIT(b->elements, 0x02, 1, 0);
	a.elem.thrown		= IFBIT(b->elements, 0x01, 1, 0);

	rval->pl = bteAttack_toParam(&a);

	return rval;
}

/*
 * spf_generic()
 */
static btAction_t *spf_generic(uint32_t index)
{
	uint8_t		type;
	btAction_t	*rval;

	type = spellType[index];

	if (spellStrings[type] == NULL)
		return NULL;

	rval = btAction_new(FUNC_NONE, EFFECT_NONE);

	switch (type) {
	case sp_phaseDoor:
		rval->pl = paramList_new();

		param_add(rval->pl, PARAM_BOOL, "isPermanent",
			spellDuration[index] > 0);
		break;
	}

	rval->function = getFunction(type);

	return rval;
}

/*
 * spf_geomancer()
 */
static btAction_t *spf_geomancer(uint32_t i)
{
	btAction_t	*rval;

	rval = btAction_new(FUNC_NONE, EFFECT_NONE);
	rval->function = btFunction_new(FUNC_STRING, bts_strcpy("--xxx_geomancer_spell"));

	return rval;
#if 0
	btAction_t *rsp;

	rsp = spellEffect_new(S_GEOMANCER);

	GEOPTR(rsp, type) = spellDuration[i];

	return rsp;
#endif
}

/*
 * spf_healspell()
 */
static btAction_t *spf_healspell(uint32_t index)
{
	btAction_t	*rval;
	bteHeal_t	bh;

	memset(&bh, 0, sizeof(bh));

	rval = btAction_new(FUNC_NONE, EFFECT_NONE);
	rval->function = getFunction(sp_healSpell);

	bh.groupHeal		= IFBIT(spellRange[index], 0x80, 1, 0);

	switch (spellDuration[index]) {
	case 0xfe:
		bh.randomHeal	= 0;
		bh.ndice	= 8;
		break;
	case 0xfd:
		bh.randomHeal	= 0;
		bh.fullHeal	= 1;
		break;
	case 0xff:
		bh.randomHeal	= 1;
		bh.levelMultiply	= 1;
		bh.dieval	= 4;
		break;
	default:
		bh.randomHeal	= 1;
		bh.ndice	= spellDuration[index];
		bh.dieval	= 4;
		break;
	}

	switch (spellRange[index] & 0x7f) {
	case 1:
		bh.poison	= 1;
		bh.paralysis	= 1;
		bh.insanity	= 1;
		break;
	case 2:
		bh.stoned	= 1;
		break;
	case 3:
		bh.dispossess	= 1;
		break;
	case 4:
		bh.resurrect	= 1;
		break;
	case 5:
		bh.old		= 1;
		break;
	case 6:
		bh.poison	= 1;
		bh.resurrect	= 1;
		bh.paralysis	= 1;
		bh.insanity	= 1;
		break;
	}

	rval->pl = bteHeal_toParam(&bh);

	return rval;
}

/*
 * spf_passiveeffect()
 */
static btAction_t *spf_passiveeffect(uint32_t index)
{
	btAction_t	*rval;
	uint8_t		type, flags;
	int8_t		duration;
	btePassive_t	bp;

	memset(&bp, 0, sizeof(btePassive_t));

	type		= spellType[index];
	duration	= spellDuration[index];
	flags		= spellRange[index];

	rval = btAction_new(FUNC_NONE, EFFECT_NONE);

	switch (spellType[index]) {
	case sp_areaEnchant:
		bp.duration		= (duration == 0xff) ? -1 : duration;
		bp.detectStairs		= (flags != 0);
		bp.detectTraps		= (flags != 1);
		bp.detectSpecial	= (flags == 2);
		break;
	case sp_compassSpell:
	case sp_levitation:
		bp.duration		= (duration == 0xff) ? -1 : duration;
		break;
	case sp_lightSpell:
		bp.duration		= lightDur[duration];
		bp.lightDistance	= lightDist[duration];
		bp.detectSecret		= lightSDFlag[duration];
		break;
	case sp_shieldSpell:
		bp.duration		= (duration == 0xff) ? -1 : duration;
		bp.acBonus		= flags;
		break;
	}

	rval->function	= getFunction(type); 
	rval->pl	= btePassive_toParam(&bp);

	return rval;
}

/*
 * spf_rangespell()
 */
static btAction_t *spf_rangespell(uint32_t index)
{
	btAction_t	*rval;

	rval = btAction_new(FUNC_NONE, EFFECT_NONE);
	rval->function = getFunction(spellType[index]);

	if (spellType[index] == sp_farFoes) {
		rval->pl = paramList_new();

		param_add(rval->pl, PARAM_NUMBER, "distance", 
			spellDuration[index]);
	}

	return rval;
}

/*
 * spf_summon()
 */
static btAction_t *spf_summon(uint32_t index)
{
	btAction_t	*rval;

	rval = btAction_new(FUNC_NONE, EFFECT_NONE);
	rval->function = getFunction(spellType[index]);

	rval->pl	= paramList_new();

	param_add(rval->pl, PARAM_BOOL, "isIllusion",
			IFBIT(spellDuration[index], 0x80, 1, 0));
	param_add(rval->pl, PARAM_BTSTRING, "type",
			getSummonMacro(spellDuration[index] & 0x7f));

	if (index == 77) {
		param_add(rval->pl, PARAM_BOOL, "fillParty", 1);
	}

	return rval;
}

/*
 * spf_batchspell()
 */
static btAction_t *spf_batchspell(uint32_t index)
{
	uint32_t	i;
	uint32_t	count;
	btAction_t	*rval;
	paramList_t	*pl;

	uint8_t		indexString[2];

	rval		= btAction_new(FUNC_NONE, EFFECT_NONE);
	rval->function	= getFunction(sp_batchspell);
	rval->pl	= paramList_new();
	pl		= paramList_new();

	count = 1;
	for (i = spellDuration[index]; batchSpellList[i]; i++) {
		param_add(pl, PARAM_STRING, "",
			spName[batchSpellList[i] & 0x7f].abbr);
	}

	param_add(rval->pl, PARAM_ARRAY, "spellList", pl);

	return rval;
	
}


/********************************/
/*				*/
/* Public Functions		*/
/*				*/
/********************************/

/*
 * getTargetting()
 */
void getTargetting(uint32_t index, btTargetting_t *bt)
{
	if ((spellAttr[index] & 7) >= 4) {
		bt->targetted = 0;
	} else {
		bt->targetted = 1;

		switch (spellAttr[index] & 7) {
		case 3:
			bt->targetMonster = 1;
			/* Fall through */
		case 0:
		case 1:
			bt->targetParty = 1;
			break;
		case 2:
			bt->targetMonster = 1;
			break;
		}
	}
}

/*
 * getFigurineAction()
 * Public interface to spf_summon
 */
btAction_t *getFigurineAction(uint32_t i)
{
	btAction_t	*rval;

	rval = btAction_new(FUNC_NONE, EFFECT_NONE);
	rval->function = getFunction(sp_summonSpell);

	rval->pl	= paramList_new();

	param_add(rval->pl, PARAM_BTSTRING, "type",
			getSummonMacro(i));

	return rval;
}

/*
 * getWeaponAction()
 * Convert a weapon attack structure to a damage spell
 */
btAction_t *getWeaponAction(uint8_t i)
{
	breathAtt_t	b;
	weapAtt_t	*w;

	if (i == 0xff) {
		printf("Nope");
		return;
	}

	w = &weapeff[i];

	b.breathFlag = 0x10;
	b.repelFlags = 0;
	b.effect = w->effect;
	b.elements = w->elements;
	b.damage = w->damage;
	b.targetFlags = w->targetFlags;
	b.levelMult = w->levelMult;

	return cnvBreathAttack(&b, w->range);
}

#if 0
/*
 * getSpellName()
 * Public interface to spell full name
 */
uint8_t *getSpellName(uint8_t spno)
{
	return spName[spno].full;
}

/*
 * getSpellAbbr()
 * Public interface to spell abbreviation
 */
btstring_t *getSpellAbbr(uint8_t spno)
{
	return bts_strcpy(spName[spno].abbr);
}
#endif

/*
 * getSpellAction()
 * Public interface to the spell functions
 */
btAction_t *getSpellAction(uint32_t spell)
{
	uint8_t sptype;

	sptype = spellType[spell];

	if (spellFunctions[sptype] == NULL)
		return 0;

	return spellFunctions[sptype] (spell);
}

/*
 * convertSpells()
 */
void convertSpells(void)
{
	int		i;
	mageSpell_t	*m;
	cnvList_t	*spells;

	spells = spellList_new();

	for (i = 0; i < 125; i++) {
		m = mageSpell_new(0);

		m->abbr = bts_strcpy(spName[i].abbr);
		if (spName[i].full[0])
			m->full = bts_strcpy(spName[i].full);
		else
			m->full = bts_strdup(m->abbr);
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
