#ifndef _CNV_SPELL_H
#define _CNV_SPELL_H

#include <cnvlib.h>
#include <bt_string.h>
#include <cnv_common.h>
#include <cnv_effect.h>

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#if 0 
#define S_NONE		0
#define S_HEALSPELL     1
#define S_PASSIVEEFFECT 2
#define S_GEOMANCER     3
#define S_BATBONUS      4
#define S_DISBELIEVE    5
#define S_DMGSPELL      6
#define S_SUMMONSPELL   7
#define S_GENERIC       8
#define S_LASTSPELL	S_GENERIC

#define PASS_LIGHT      0
#define PASS_COMPASS    1
#define PASS_LEVITATE   2
#define PASS_SHIELD     3
#define PASS_DETECT     4

#define GENS_NONE       0
#define GENS_TRAPZAP    1
#define GENS_MAGESTAR   2
#define GENS_TELEPORT   3
#define GENS_SCRYSITE   4
#define GENS_ANTIMAGIC  5
#define GENS_PHASEDOOR  6
#define GENS_SPELLBIND  7
#define GENS_SAFETYSPELL        8
#define GENS_DREAMSPELL 9
#define GENS_BATCHSPELL 10
#define GENS_CAMARADERIE        11
#define GENS_DIVINE     12
#define GENS_IDENTIFY   13
#define GENS_POSSESS    14
#define GENS_EARTHMAW   15
#define GENS_CHRONO     16
#define GENS_MAPSPELL   17
#define GENS_REENERGIZE 18
#define GENS_SUMMONHELP	19
#define GENS_TARJAN	20
#define GENS_DOPPEL	21
#define GENS_STRING	22

#define BBOPTR(ptr,pte)	((eff_BattleBonus *)((ptr)->effect))->pte
#define DISPTR(ptr,pte)	((eff_Disbelieve *)((ptr)->effect))->pte
#define DMGPTR(ptr,pte) ((eff_Damage *)((ptr)->effect))->pte
#define GENPTR(ptr,pte) ((eff_generic *)((ptr)->effect))->pte
#define GEOPTR(ptr,pte) ((eff_geomancer *)((ptr)->effect))->pte
#define HEAPTR(ptr,pte)	((eff_Heal *)((ptr)->effect))->pte
#define PASPTR(ptr,pte) ((eff_PassiveEffect *)((ptr)->effect))->pte
#define SUMPTR(ptr,pte) ((eff_Summon *)((ptr)->effect))->pte
#endif


/********************************/
/*				*/
/* Structures			*/
/*				*/
/********************************/

typedef	struct songList_t songList_t;

#if 0 
typedef struct {
	Bitfield(fire, 1);
	Bitfield(unk1, 1);
	Bitfield(holy, 1);
	Bitfield(ice, 1);
	Bitfield(lightning, 1);
	Bitfield(spell, 1);
	Bitfield(unk2, 1);
	Bitfield(unk3, 1);
} elements_t;

typedef struct {
	Bitfield(evil, 1);
	Bitfield(demon, 1);
	Bitfield(spellcaster, 1);
	Bitfield(unk1, 1);
	Bitfield(unk2, 1);
	Bitfield(unk3, 1);
	Bitfield(unk4, 1);
	Bitfield(unk5, 1);
} repel_t;

typedef struct {
	Bitfield(group, 1);
	Bitfield(antimagic, 7);

	Bitfield(acBon, 1);
	Bitfield(acPen, 1);
	Bitfield(acStk, 1);
	Bitfield(acRnd, 1);
	Bitfield(acAmt, 6);
	Bitfield(acDice, 6);

	Bitfield(tohitBon, 1);
	Bitfield(tohitPen, 1);
	Bitfield(tohitStk, 1);
	Bitfield(tohitRnd, 1);
	Bitfield(tohitAmt, 6);
	Bitfield(tohitDice, 6);

	Bitfield(dmgBon, 1);
	Bitfield(dmgPen, 1);
	Bitfield(dmgStk, 1);
	Bitfield(dmgRnd, 1);
	Bitfield(dmgAmt, 6);
	Bitfield(dmgDice, 6);
} eff_BattleBonus;

/*
 * Damage spell effects
 */
typedef struct {
	Bitfield(allFoes, 1);	/* Damage all foes              */
	Bitfield(group, 1);	/* An entire group              */
	Bitfield(levelMult, 1);	/* Damage increases with level  */
	Bitfield(isSpell, 1);	/* Is a spell flag              */
	Bitfield(isBreath, 1);	/* Is breath attack flag        */

	Bitfield(outOfRange, 1);	/* Effects targets out of range */
	Bitfield(range, 4);	/* Spell range                  */

	Bitfield(addFlag, 1);	/* If set, add value to group distance  */
	Bitfield(distance, 4);	/* Distance to move or be moved to      */

	uint8_t msg_target;
	uint8_t attype;
	uint8_t spAttack;

	uint16_t ndice;		/* The X in XdY                 */
	uint16_t dieval;	/* The Y in XdY                 */

	repel_t rflags;		/* Attack repel flags           */
	elements_t elem;	/* Attack element flags         */
} eff_Damage;

/*
 * Disbelieve flags
 */
typedef struct {
	Bitfield(disbelieve, 1);
	Bitfield(allBattle, 1);
	Bitfield(revealDoppl, 1);
	Bitfield(noSummon, 1);
	Bitfield(unused, 4);
} eff_Disbelieve;

/*
 * Healing effect
 */
typedef struct {
	Bitfield(rndHeal, 1);
	Bitfield(groupHeal, 1);
	Bitfield(fullHeal, 1);
	Bitfield(levelMult, 1);
	Bitfield(dice, 5);

	Bitfield(dispossess, 1);
	Bitfield(resurrect, 1);
	Bitfield(poison, 1);
	Bitfield(paralysis, 1);
	Bitfield(insanity, 1);
	Bitfield(old, 1);
	Bitfield(stoned, 1);

	uint32_t	dieval;
	uint8_t quench_bard;
} eff_Heal;

/*
 * Passive spell effect
 */
typedef struct {
	uint8_t duration;
	uint8_t type;

	 Bitfield(lightDist, 3);
	 Bitfield(secretDoor, 1);
	 Bitfield(detectFlag, 4);
	uint8_t detStairs;
	uint8_t detTraps;
	uint8_t detSpecial;
} eff_PassiveEffect;

/*
 * Summon effect
 */
typedef struct {
	Bitfield(rndflag, 1);	/* Flag to randomly choose monster      */
	Bitfield(illflag, 1);	/* Illusion flag                */
	Bitfield(unused, 6);

	btstring_t *sum_zero;
	btstring_t *sum_one;
} eff_Summon;

/* Generic spell */
typedef struct {
	btstring_t	*function;
	uint8_t		type;
	uint8_t		flags;
} eff_generic;

/* Geomancer spell */
typedef struct {
	uint8_t type;
} eff_geomancer;


typedef struct {
	uint8_t type;
	uint8_t subtype;
	uint8_t etype;

	void *effect;
} spellEffect_t;
#endif

typedef struct {
	btstring_t	*full;

	cnvList_t	*nonCombat;
	cnvList_t	*combat;
} bardSong_t;

typedef struct {
	btstring_t 	*abbr;
	btstring_t	*full;
	btstring_t	*class;
	uint8_t		level;

	uint8_t		sppt;

	uint8_t		combat;
	uint8_t		noncombat;
	btTargetting_t	targetting;

/*	spellEffect_t	*sp;*/
	btFunction_t	*function;
	btEffect_t	*effect;
} mageSpell_t;

/********************************/
/*				*/
/* Public Functions		*/
/*				*/
/********************************/

#if 0
spellEffect_t *spellEffect_new(uint8_t type);
#endif
mageSpell_t *mageSpell_new(uint8_t type);
bardSong_t *bardSong_new(void);
#if 0
void 		spellEffect_free(const void *vse);
uint8_t *getSpellEffectString(uint8_t type);
void printSpellEffectXML(uint8_t indent, spellEffect_t * se);
void printRepelXML(uint8_t indent, repel_t r);
void printElemXML(uint8_t indent, elements_t e);

json_t		*spellEffect_toJson(const void *vse);

cnvList_t 	*spellEffectList_new(void);
#endif

cnvList_t	*spellList_new(void);
void		spellList_to_json(cnvList_t *sl, btstring_t *fname);

cnvList_t	*songList_new(void);
void		songList_to_json(cnvList_t *sl, btstring_t *fname);

#endif
