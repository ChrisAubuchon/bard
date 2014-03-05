#ifndef _B3_SPELL_H
#define _B3_SPELL_H

typedef spellEffect_t* (spfunc_t)(uint32_t index);

typedef struct {
  uint8_t abbr[5];
  uint8_t full[32];
} spName_t;

typedef struct {
  uint8_t effect;
  uint8_t elements;
  uint8_t breathFlag;
  uint8_t repelFlags;
  uint8_t damage;
  uint8_t targetFlags;
  uint8_t levelMult;
} breathAtt_t;

typedef struct {
  uint8_t effect;
  uint8_t elements;
  uint8_t damage;
  uint8_t targetFlags;
  uint8_t levelMult;
  uint8_t range;
} weapAtt_t;


uint8_t spellType[];
uint8_t spptReq[];
uint8_t spellAttr[];
uint8_t spellDuration[];
uint8_t spellRange[];

uint8_t lightDist[];
uint8_t lightDur[];
uint8_t lightSDFlag[];

spName_t spName[];
breathAtt_t dmgspells[];
weapAtt_t weapeff[];
uint8_t weapIndex[];

#define _sp_reenergizeMage	0
#define _sp_useAcorn		1
#define _sp_useFigurine		2
#define _sp_useLightObj		3
#define _sp_useWeapon		4
#define _sp_useWineskin		5
#define sp_acBonus		6
#define sp_antiMagic		7
#define sp_areaEnchant		8
#define sp_batchspell		9
#define sp_camaraderie		10
#define sp_compassSpell		11
#define sp_damageSpell		12
#define sp_disbelieve		13
#define sp_divineIntervention	14
#define sp_earthMaw		15
#define sp_farFoes		16
#define sp_freezeFoes		17
#define sp_geomancerSpell	18
#define sp_haltFoe		19
#define sp_healSpell		20
#define sp_identifySpell	21
#define sp_levitation		22
#define sp_lightSpell		23
#define sp_luckSpell		24
#define sp_meleeMen		25
#define sp_phaseDoor		26
#define sp_possessChar		27
#define sp_scrySight		28
#define sp_shieldSpell		29
#define sp_spellbind		30
#define sp_strengthBonus	31
#define sp_summonSpell		32
#define sp_teleport		33
#define sp_trapZap		34
#define sp_vorpalPlating	35
#define sp_wordOfFear		36
#define printSpellFizzled	37
#define printNoEffect		38
#define printCantFindUse	39
#define sp_Grro			40
#define sp_Fota			41
#define sp_mapOnly		42

#endif
