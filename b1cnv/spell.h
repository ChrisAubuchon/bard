#ifndef _SPELL_H
#define _SPELL_H

#include <cnv_spell.h>
#include <cnv_effect.h>

extern uint8_t spellLevel[];
extern uint8_t spptReq[];
extern uint8_t spellAttr[];
extern uint8_t spellType[];
extern uint8_t spellDuration[];
extern uint8_t summonIndex[];

extern uint8_t *spellClass[];
extern uint8_t *spellAbbr[];
extern uint8_t *spellFull[];

extern uint8_t lightDist[];
extern uint8_t lightDur[];
extern uint8_t lightSDFlag[];

extern uint8_t levitDur[];

extern uint8_t phaseBlur[];

extern uint8_t shieldSelf[];

extern uint8_t battleSkill[];

extern uint8_t wordoffear[];

#define sp_LightSpell	0
#define sp_ArcFire	1
#define sp_ShieldSelf	2
#define sp_TrapZap	3
#define sp_FreezeFoes	4
#define sp_MagicCompass	5
#define sp_BattleSkill	6
#define sp_WordOfHealing	7
#define sp_MageStar	8
#define sp_Levitation	9
#define sp_WarStrike	10
#define sp_InstantWolf	11
#define sp_PoisonStrike	12
#define sp_Teleport	13
#define sp_PhaseBlur	14
#define sp_Disbelieve	15
#define sp_WordOfFear	16
#define sp_LocateTraps	17
#define sp_MindBlade	18
#define sp_VorpalPlating	19
#define sp_ScrySight	20
#define sp_HolyWater	21
#define sp_GroupShield	22
#define sp_SpectreTouch	23
#define sp_AntiMagic	24
#define sp_PhaseDoor	25
#define sp_Spellbind	26
#define sp_Generic	27

#endif
