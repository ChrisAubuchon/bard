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

static btAction_t	*spf_batbonus(uint32_t i);
static btAction_t	*spf_disbelieve(uint32_t i);
static btAction_t	*spf_dmgspell(uint32_t i);
static btAction_t	*spf_generic(uint32_t i);
static btAction_t	*spf_geomancer(uint32_t i);
static btAction_t	*spf_healspell(uint32_t i);
static btAction_t	*spf_passiveeffect(uint32_t i);
static btAction_t	*spf_rangespell(uint32_t i);
static btAction_t	*spf_summon(uint32_t i);
static btAction_t	*spf_grro(uint32_t i);
static btAction_t	*spf_fota(uint32_t i);

static btFunction_t	*getFunction(uint32_t type);
btAction_t *cnvBreathAttack(breathAtt_t * b, uint8_t range);

/********************************/
/*				*/
/* Local data			*/
/*				*/
/********************************/

static spellFunction_t *spellFunctions[] = {
/* 0*/ spf_generic, spf_generic, NULL, NULL, NULL,
/* 5*/ spf_generic, spf_batbonus, spf_batbonus, spf_passiveeffect, spf_generic,
/*10*/ spf_generic, spf_passiveeffect, spf_dmgspell, spf_disbelieve, spf_generic,
/*15*/ spf_generic, spf_rangespell, spf_batbonus, spf_geomancer, spf_generic,
/*20*/ spf_healspell, spf_generic, spf_passiveeffect, spf_passiveeffect, spf_batbonus,
/*25*/ spf_rangespell, spf_generic, spf_generic, spf_generic, spf_passiveeffect,
/*30*/ spf_generic, spf_batbonus, spf_summon, spf_generic, spf_generic,
/*35*/ spf_batbonus, spf_batbonus, spf_generic, spf_generic, spf_generic,
/*40*/ spf_grro, spf_fota, spf_generic
};

#if 0
static uint8_t spTypeStr[] = {
/* 0*/ S_NONE, S_NONE, S_NONE, S_NONE,
/* 4*/ S_NONE, S_NONE, S_BATBONUS, S_BATBONUS,
/* 8*/ S_PASSIVEEFFECT, S_GENERIC, S_GENERIC, S_PASSIVEEFFECT,
/*12*/ S_DMGSPELL, S_DISBELIEVE, S_GENERIC, S_GENERIC,
/*16*/ S_DMGSPELL, S_BATBONUS, S_GEOMANCER, S_GENERIC,
/*20*/ S_HEALSPELL, S_GENERIC, S_PASSIVEEFFECT, S_PASSIVEEFFECT,
/*24*/ S_BATBONUS, S_DMGSPELL, S_GENERIC, S_GENERIC,
/*28*/ S_GENERIC, S_PASSIVEEFFECT, S_GENERIC, S_BATBONUS,
/*32*/ S_SUMMONSPELL, S_GENERIC, S_GENERIC, S_BATBONUS,
/*36*/ S_BATBONUS, S_GENERIC, S_GENERIC, S_NONE,
/*40*/ S_HEALSPELL, S_DMGSPELL, S_GENERIC
};
#endif

static uint8_t genSpell[] = {
/* 0*/ GENS_REENERGIZE, GENS_NONE, GENS_NONE, GENS_NONE, GENS_NONE,
/* 5*/ GENS_NONE, GENS_NONE, GENS_NONE, GENS_NONE, GENS_BATCHSPELL,
/*10*/ GENS_CAMARADERIE, GENS_NONE, GENS_NONE, GENS_NONE, GENS_DIVINE,
/*15*/ GENS_EARTHMAW, GENS_NONE, GENS_NONE, GENS_NONE, GENS_MAGESTAR,
/*20*/ GENS_NONE, GENS_IDENTIFY, GENS_NONE, GENS_NONE, GENS_NONE,
/*25*/ GENS_NONE, GENS_PHASEDOOR, GENS_POSSESS, GENS_SCRYSITE, GENS_NONE,
/*30*/ GENS_SPELLBIND, GENS_NONE, GENS_NONE, GENS_TELEPORT, GENS_TRAPZAP,
/*35*/ GENS_NONE, GENS_NONE, GENS_CHRONO, GENS_MAPSPELL, GENS_MAPSPELL,
/*40*/ GENS_NONE, GENS_NONE, GENS_MAPSPELL
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
	bteBonus_t	*bb;
	uint8_t		type;

	rval	= btAction_new(FUNC_NONE, EFFECT_NONE);

	type = spellType[index];

	btAction_addParam(rval, PARAM_NUMBER, "amount", spellDuration[index]);

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
static btAction_t *spf_disbelieve(uint32_t i)
{
	btAction_t	*rval;

	rval = btAction_new(FUNC_NONE, EFFECT_NONE);
	rval->function = btFunction_new(FUNC_STRING, bts_strcpy("--xxx_disbelieve_spell"));

	return rval;
#if 0
	btAction_t *rsp;

	rsp = spellEffect_new(S_DISBELIEVE);

	DISPTR(rsp, disbelieve) = IFBIT(spellDuration[i], 0x81, 1, 0);
	DISPTR(rsp, allBattle) = IFBIT(spellDuration[i], 0xc0, 1, 0);
	DISPTR(rsp, revealDoppl) = IFBIT(spellDuration[i], 0x80, 1, 0);
	DISPTR(rsp, noSummon) = IFBIT(spellDuration[i], 0x40, 1, 0);

	return rsp;
#endif
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
btAction_t *cnvBreathAttack(breathAtt_t * b, uint8_t range)
{
	btAction_t	*rval;

	rval = btAction_new(FUNC_NONE, EFFECT_NONE);
	rval->function = btFunction_new(FUNC_STRING, bts_strcpy("--xxx_damage_spell"));

	return rval;
#if 0
	uint16_t ndice;
	uint16_t dieval;
	uint8_t isBreath = 0;
	uint8_t isSpell = 0;
	btAction_t *rsp;

	rsp = spellEffect_new(S_DMGSPELL);

	DMGPTR(rsp, allFoes) = IFBIT(b->targetFlags, 0x80, 1, 0);
	DMGPTR(rsp, group) = IFBIT(b->targetFlags, 0x40, 1, 0);
	DMGPTR(rsp, levelMult) = (b->levelMult == 0);
	DMGPTR(rsp, isSpell) = IFBIT(b->targetFlags, 2, 1, 0);
	DMGPTR(rsp, isBreath) = IFBIT(b->targetFlags, 1, 1, 0);
	DMGPTR(rsp, spAttack) = b->effect;

	DMGPTR(rsp, outOfRange) = IFBIT(range, 0x80, 1, 0);
	DMGPTR(rsp, range) = range & 0x7f;
	DMGPTR(rsp, addFlag) = 0;
	DMGPTR(rsp, distance) = 0;
	DMGPTR(rsp, attype) = (b->breathFlag & 0xfe) >> 1;
	if (b->damage) {
		DMGPTR(rsp, ndice) = NDICE(b->damage) * ((b->levelMult != 0) ? b->levelMult : 1);
		DMGPTR(rsp, dieval) = DIEVAL(b->damage);
	}

	DMGPTR(rsp, rflags.evil) = IFBIT(b->repelFlags, 0x80, 1, 0);
	DMGPTR(rsp, rflags.demon) = IFBIT(b->repelFlags, 0x40, 1, 0);
	DMGPTR(rsp, rflags.spellcaster) = IFBIT(b->repelFlags, 0x20, 1, 0);
	DMGPTR(rsp, rflags.unk1) = IFBIT(b->repelFlags, 0x10, 1, 0);
	DMGPTR(rsp, rflags.unk2) = IFBIT(b->repelFlags, 0x08, 1, 0);
	DMGPTR(rsp, rflags.unk3) = IFBIT(b->repelFlags, 0x04, 1, 0);
	DMGPTR(rsp, rflags.unk4) = IFBIT(b->repelFlags, 0x02, 1, 0);
	DMGPTR(rsp, rflags.unk5) = IFBIT(b->repelFlags, 0x01, 1, 0);

	DMGPTR(rsp, elem.fire) = IFBIT(b->elements, 0x80, 1, 0);
	DMGPTR(rsp, elem.unk1) = IFBIT(b->elements, 0x40, 1, 0);
	DMGPTR(rsp, elem.holy) = IFBIT(b->elements, 0x20, 1, 0);
	DMGPTR(rsp, elem.ice) = IFBIT(b->elements, 0x10, 1, 0);
	DMGPTR(rsp, elem.lightning) = IFBIT(b->elements, 0x08, 1, 0);
	DMGPTR(rsp, elem.spell) = IFBIT(b->elements, 0x04, 1, 0);
	DMGPTR(rsp, elem.unk2) = IFBIT(b->elements, 0x02, 1, 0);
	DMGPTR(rsp, elem.unk3) = IFBIT(b->elements, 0x01, 1, 0);

	return rsp;
#endif
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
		btAction_addParam(rval, PARAM_BOOL, "isPermanent",
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
static btAction_t *spf_healspell(uint32_t i)
{
	btAction_t	*rval;

	rval = btAction_new(FUNC_NONE, EFFECT_NONE);
	rval->function = btFunction_new(FUNC_STRING, bts_strcpy("--xxx_heal_spell"));

	return rval;
#if 0
	btAction_t *rsp;

	rsp = spellEffect_new(S_HEALSPELL);

	HEAPTR(rsp, groupHeal) = IFBIT(spellRange[i], 0x80, 1, 0);

	switch (spellDuration[i]) {
	case 0xfe:
		HEAPTR(rsp, rndHeal) = 0;
		HEAPTR(rsp, dice) = 8;
		break;
	case 0xfd:
		HEAPTR(rsp, rndHeal) = 0;
		HEAPTR(rsp, fullHeal) = 1;
		break;
	case 0xff:
		HEAPTR(rsp, rndHeal) = 1;
		HEAPTR(rsp, levelMult) = 1;
		break;
	default:
		HEAPTR(rsp, rndHeal) = 1;
		HEAPTR(rsp, dice) = spellDuration[i];
		break;
	}

	switch (spellRange[i] & 0x7f) {
	case 1:
		HEAPTR(rsp, poison) = 1;
		HEAPTR(rsp, paralysis) = 1;
		HEAPTR(rsp, insanity) = 1;
		break;
	case 2:
		HEAPTR(rsp, stoned) = 1;
		break;
	case 3:
		HEAPTR(rsp, dispossess) = 1;
		break;
	case 4:
		HEAPTR(rsp, resurrect) = 1;
		break;
	case 5:
		HEAPTR(rsp, old) = 1;
		break;
	case 6:
		HEAPTR(rsp, poison) = 1;
		HEAPTR(rsp, resurrect) = 1;
		HEAPTR(rsp, paralysis) = 1;
		HEAPTR(rsp, insanity) = 1;
		break;
	default:
		break;
	}

	return rsp;
#endif
}

/*
 * spf_passiveeffect()
 */
static btAction_t *spf_passiveeffect(uint32_t index)
{
	btAction_t	*rval;
	uint8_t		type, flags;
	int8_t		duration;

	type		= spellType[index];
	duration	= spellDuration[index];
	flags		= spellRange[index];


	rval = btAction_new(FUNC_NONE, EFFECT_NONE);

	switch (spellType[index]) {
	case sp_areaEnchant:
		btAction_addParam(rval, PARAM_NUMBER, "duration", duration);
		btAction_addParam(rval, PARAM_BOOL, "detectStairs",
					(flags != 0));
		btAction_addParam(rval, PARAM_BOOL, "detectTraps",
					(flags != 1));
		btAction_addParam(rval, PARAM_BOOL, "detectSpecial",
					(flags == 2));
		
		break;
	case sp_compassSpell:
	case sp_levitation:
		btAction_addParam(rval, PARAM_NUMBER, "duration", duration);
		break;
	case sp_lightSpell:
		btAction_addParam(rval, PARAM_NUMBER, "duration", 
					lightDur[duration]);
		btAction_addParam(rval, PARAM_NUMBER, "distance", 
					lightDur[duration]);
		btAction_addParam(rval, PARAM_NUMBER, "seeSecret", 
					lightDur[duration]);
		break;
	case sp_shieldSpell:
		btAction_addParam(rval, PARAM_NUMBER, "duration", duration);
		btAction_addParam(rval, PARAM_NUMBER, "acBonus", flags);
		break;
	}

	rval->function = getFunction(type); 

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

	return rval;
#if 0
	btAction_t *rsp;

	rsp = spellEffect_new(S_DMGSPELL);

	switch (spellType[i]) {
	case sp_farFoes:
		DMGPTR(rsp, addFlag) = 1;
		DMGPTR(rsp, distance) = spellDuration[i];
		break;
	case sp_meleeMen:
		DMGPTR(rsp, distance) = 1;
		break;
	}

	DMGPTR(rsp, attype) = 10;

	return rsp;
#endif
}

/*
 * spf_summon()
 */
static btAction_t *spf_summon(uint32_t index)
{
	btAction_t	*rval;

	rval = btAction_new(FUNC_NONE, EFFECT_NONE);
	rval->function = getFunction(spellType[index]);

	return rval;
#if 0 
	btAction_t *rsp;
	uint8_t *mon;

	rsp = spellEffect_new(S_SUMMONSPELL);

	SUMPTR(rsp, illflag) = IFBIT(spellDuration[i], 0x80, 1, 0);
	SUMPTR(rsp, sum_zero) = getSummonMacro(spellDuration[i] & 0x7f);

	return rsp;
#endif
}

/*
 * spf_grro
 * Convert the Grave Robber batch spell to a healing spell
 */
static btAction_t *spf_grro(uint32_t i)
{
	btAction_t	*rval;

	rval = btAction_new(FUNC_NONE, EFFECT_NONE);
	rval->function = btFunction_new(FUNC_STRING, bts_strcpy("--xxx_grro_spell"));

	return rval;
#if 0
	btAction_t *rsp;

	rsp = spellEffect_new(S_HEALSPELL);

	HEAPTR(rsp, fullHeal) = 1;
	HEAPTR(rsp, resurrect) = 1;
	HEAPTR(rsp, poison) = 1;
	HEAPTR(rsp, paralysis) = 1;
	HEAPTR(rsp, insanity) = 1;

	return rsp;
#endif
}

/*
 * spf_fota
 * Convert the Force of Tarjan spell to a damage spell
 */
static btAction_t *spf_fota(uint32_t i)
{
	btAction_t	*rval;

	rval = btAction_new(FUNC_NONE, EFFECT_NONE);
	rval->function = btFunction_new(FUNC_STRING, bts_strcpy("--xxx_fota_spell"));

	return rval;
#if 0
	btAction_t *rsp;

	rsp = spellEffect_new(S_DMGSPELL);

	DMGPTR(rsp, allFoes) = 0;
	DMGPTR(rsp, group) = 1;
	DMGPTR(rsp, levelMult) = 0;
	DMGPTR(rsp, isSpell) = 0;
	DMGPTR(rsp, isBreath) = 0;
	DMGPTR(rsp, spAttack) = 4;

	DMGPTR(rsp, outOfRange) = 0;
	DMGPTR(rsp, range) = 0;
	DMGPTR(rsp, addFlag) = 1;
	DMGPTR(rsp, distance) = 6;
	DMGPTR(rsp, attype) = 3;
	DMGPTR(rsp, ndice) = 300;
	DMGPTR(rsp, dieval) = 2;

	DMGPTR(rsp, rflags.evil) = 0;
	DMGPTR(rsp, rflags.demon) = 0;
	DMGPTR(rsp, rflags.spellcaster) = 0;
	DMGPTR(rsp, rflags.unk1) = 0;
	DMGPTR(rsp, rflags.unk2) = 0;
	DMGPTR(rsp, rflags.unk3) = 0;
	DMGPTR(rsp, rflags.unk4) = 0;
	DMGPTR(rsp, rflags.unk5) = 0;

	DMGPTR(rsp, elem.fire) = 0;
	DMGPTR(rsp, elem.unk1) = 0;
	DMGPTR(rsp, elem.holy) = 0;
	DMGPTR(rsp, elem.ice) = 0;
	DMGPTR(rsp, elem.lightning) = 0;
	DMGPTR(rsp, elem.spell) = 0;
	DMGPTR(rsp, elem.unk2) = 0;
	DMGPTR(rsp, elem.unk3) = 0;

	return rsp;
#endif
}


/********************************/
/*				*/
/* Public Functions		*/
/*				*/
/********************************/

#if 0
/*
 * getFigurineEffect()
 * Public interface to spf_summon
 */
btAction_t *getFigurineEffect(uint32_t i)
{
	btAction_t *rsp;
	uint8_t *mon;

	rsp = spellEffect_new(S_SUMMONSPELL);

	SUMPTR(rsp, sum_zero) = getSummonMacro(i);

	return rsp;
}

/*
 * getWeaponEffect()
 * Convert a weapon attack structure to a damage spell
 */
btAction_t *getWeaponEffect(uint8_t i)
{
	breathAtt_t b;
	weapAtt_t *w;

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
 * getSpellEffect()
 * Public interface to the spell functions
 */
btAction_t *getSpellEffect(uint32_t spell)
{
	uint8_t sptype;

	sptype = spellType[spell];

	if (spellFunctions[sptype] == NULL)
		return 0;

	return spellFunctions[sptype] (spell);
}
#if 0

/*
 * noSpellEffect()
 * Return a no-effect structure
 */
btAction_t *noSpellEffect(void)
{
	btAction_t *rsp;

	rsp = (btAction_t *) xmalloc(sizeof(btAction_t));
	rsp->type = S_NONE;
	rsp->effect = NULL;

	return rsp;
}

/*
 * genericSpellEffect()
 * Create a generic spell effect
 */
btAction_t *genericSpellEffect(uint8_t type, uint8_t flags)
{
	btAction_t *rsp;

	rsp = spellEffect_new(S_GENERIC);
	GENPTR(rsp, flags) = flags;
	GENPTR(rsp, type) = type;

	return rsp;
}

#endif
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
#if 0
		m->targetted = ((spellAttr[i] & 7) < 4) ? 1 : 0;
#endif

		m->action = getSpellEffect(i);

		cnvList_add(spells, m);
	}

	spellList_to_json(spells, mkJsonPath("spells.json"));
	cnvList_free(spells);
}
