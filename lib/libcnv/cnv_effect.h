#ifndef _LIBCNV_LIB_EFFECT_H
#define _LIBCNV_LIB_EFFECT_H

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#define EFFECT_NONE		0
#define EFFECT_ATTACK		1
#define EFFECT_PASSIVE		2
#define EFFECT_DISBELIEVE	3
#define EFFECT_BONUS		4
#define EFFECT_HEAL		5
#define EFFECT_SUMMON		6
#define EFFECT_GENERIC		7

#define GENS_STRING		8
#define GENS_TRAPZAP		9
#define GENS_MAGESTAR		10
#define GENS_TELEPORT		11
#define GENS_SCRYSITE		12
#define GENS_ANTIMAGIC		13
#define GENS_PHASEDOOR		14
#define GENS_SPELLBIND		15
#define GENS_SAFETYSPELL	16
#define GENS_DREAMSPELL		17
#define GENS_BATCHSPELL		18
#define GENS_CAMARADERIE	19
#define GENS_DIVINE		20
#define GENS_IDENTIFY		21
#define GENS_POSSESS		22
#define GENS_EARTHMAW		23
#define GENS_CHRONO		24
#define GENS_MAPSPELL		25
#define GENS_REENERGIZE		26
#define GENS_SUMMONHELP		27
#define GENS_TARJAN		28
#define GENS_DOPPEL		29
#define GENS_NONE		30

/*
 * Passive effects
 */
#define PASS_LIGHT	1
#define PASS_COMPASS	2
#define PASS_LEVITATE	3
#define PASS_SHIELD	4
#define PASS_DETECT	5

/*
 * Effect function indexes
 */
#if 0
#define GENS_NONE		0
#define GENS_TRAPZAP		1
#define GENS_MAGESTAR		2
#define GENS_TELEPORT		3
#define GENS_SCRYSITE		4
#define GENS_ANTIMAGIC		5
#define GENS_PHASEDOOR		6
#define GENS_SPELLBIND		7
#define GENS_SAFETYSPELL	8
#define GENS_DREAMSPELL		9
#define GENS_BATCHSPELL		10
#define GENS_CAMARADERIE	11
#define GENS_DIVINE		12
#define GENS_IDENTIFY		13
#define GENS_POSSESS		14
#define GENS_EARTHMAW		15
#define GENS_CHRONO		16
#define GENS_MAPSPELL		17
#define GENS_REENERGIZE		18
#define GENS_SUMMONHELP		19
#define GENS_TARJAN		20
#define GENS_DOPPEL		21
#define GENS_STRING		22
#endif

/********************************/
/*				*/
/* Data structures		*/
/*				*/
/********************************/

typedef struct btEffect_t btEffect_t;

/*
 * Elemental flags
 */
typedef struct {
	uint8_t		fire;
	uint8_t		unk1;
	uint8_t		holy;
	uint8_t		ice;
	uint8_t		lightning;
	uint8_t		spell;
	uint8_t		unk2;
	uint8_t		unk3;
} elements_t;

/*
 * Resistances
 */
typedef struct {
	uint8_t		evil;
	uint8_t		demon;
	uint8_t		spellcaster;
	uint8_t		unk1;
	uint8_t		unk2;
	uint8_t		unk3;
	uint8_t		unk4;
	uint8_t		unk5;
} repel_t;


/* 
 * Attack effect
 */
typedef struct {
	uint16_t	ndice;
	uint16_t	dieval;

	uint8_t		allFoes;
	uint8_t		group;
	uint8_t		levelMult;
	uint8_t		isSpell;
	uint8_t		isBreath;

	uint8_t		outOfRange;
	uint8_t		range;

	uint8_t		addDistance;
	uint8_t		distance;

	uint8_t		spAttack;
	uint8_t		attype;
	uint8_t		meleeString;

	repel_t		rflags;
	elements_t	elem;
} bteAttack_t;
#define	btEffect_attack(be) (bteAttack_t *)btEffect_getDataPointer(be)

/*
 * Passive effect
 */
typedef struct {
	uint8_t		type;
	uint8_t		duration;

	uint8_t		lightDistance;
	uint8_t		detectSecret;

	uint8_t		detectStairs;
	uint8_t		detectTraps;
	uint8_t		detectSpecial;

	uint8_t		acBonus;
} btePassive_t;
#define btEffect_passive(be) (btePassive_t *)btEffect_getDataPointer(be)

/*
 * Disbelieve effect
 */
typedef struct {
	uint8_t		disbelieve;
	uint8_t		allBattle;
	uint8_t		revealDoppelganger;
	uint8_t		noSummon;
} bteDisbelieve_t;
#define btEffect_disbelieve(be) (bteDisbelieve_t *)btEffect_getDataPointer(be)

/*
 * Bonus effect
 */
typedef struct {
	uint8_t		group;
	uint8_t		antiMagic;

	uint8_t		acBonus;
	uint8_t		acPenalty;
	uint8_t		acStack;
	uint8_t		acRandom;
	uint8_t		acAmount;
	uint8_t		acDice;

	uint8_t		toHitBonus;
	uint8_t		toHitPenalty;
	uint8_t		toHitStack;
	uint8_t		toHitRandom;
	uint8_t		toHitAmount;
	uint8_t		toHitDice;

	uint8_t		damageBonus;
	uint8_t		damagePenalty;
	uint8_t		damageStack;
	uint8_t		damageRandom;
	uint8_t		damageAmount;
	uint8_t		damageDice;
} bteBonus_t;
#define btEffect_bonus(be) (bteBonus_t *)btEffect_getDataPointer(be)

typedef struct {
	uint8_t		randomHeal;
	uint8_t		groupHeal;
	uint8_t		fullHeal;
	uint8_t		levelMultiply;

	uint8_t		ndice;
	uint32_t	dieval;

	uint8_t		dispossess;
	uint8_t		resurrect;
	uint8_t		poison;
	uint8_t		paralysis;
	uint8_t		insanity;
	uint8_t		old;
	uint8_t		stoned;

	uint8_t		quenchBard;
} bteHeal_t;
#define btEffect_heal(be) (bteHeal_t *)btEffect_getDataPointer(be)

typedef struct {
	uint8_t		isRandom;
	uint8_t		isIllusion;
	btstring_t	*sumZero;
	btstring_t	*sumOne;
} bteSummon_t;
#define btEffect_summon(be) (bteSummon_t *)btEffect_getDataPointer(be);

typedef struct {
	uint8_t		type;
	uint8_t		flags;
	btstring_t	*funcString;
} bteGeneric_t;
#define btEffect_generic(be) (bteGeneric_t *)btEffect_getDataPointer(be);

typedef struct {
	uint8_t		targetted;
	uint8_t		targetParty;
	uint8_t		targetSummon;
	uint8_t		targetMonster;
	uint8_t		targetMelee;
} btTargetting_t;

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

json_t		*repel_toJson(repel_t *r);
json_t		*elements_toJson(elements_t *e);

btEffect_t	*btEffect_new(uint8_t type);
void		btEffect_free(const void *vbe);
json_t		*btEffect_toJson(const void *vbe);
void		*btEffect_getDataPointer(btEffect_t *be);

json_t		*btTargetting_toJson(btTargetting_t *bto);

#endif
