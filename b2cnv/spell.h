#ifndef _SPELL_H
#define _SPELL_H

#include <cnv_spell.h>

extern uint8_t spptReq[];
extern uint8_t spellAttr[];
extern uint8_t spellType[];
extern uint8_t spellDuration[];
extern uint8_t spellRange[];
extern uint8_t summonIndex[];
extern uint8_t spellLevel[];

extern uint8_t *spellAbbr[];
extern uint8_t *spellFull[];
extern uint8_t *spellClass[];

/* Light spell specific arrays */
extern uint8_t lightDist[];
extern uint8_t lightDur[];
extern uint8_t lightSDFlag[];

/* Levitation spell array */
extern uint8_t levitDur[];

/* Phase blur bonus list */
extern uint8_t phaseBlur[];
extern uint8_t shieldSelf[];

/* Word of fear penalty list */
extern uint8_t wordoffear[];
extern uint8_t battleskill[];

#define sp_Lightspell	0
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
#define sp_Summon	11
#define sp_StoneTouch	12
#define sp_Teleport	13
#define sp_PhaseBlur	14
#define sp_Disbelieve	15
#define sp_WordOfFear	16
#define sp_AreaEnchant	17
#define sp_MindBlade	18
#define sp_VorpalPlating	19
#define sp_ScrySight	20
#define sp_HolyWater	21
#define sp_GroupShield	22
#define sp_SpectreTouch	23
#define sp_AntiMagic	24
#define sp_PhaseDoor	25
#define sp_Spellbind	26
#define sp_FarFoe	27
#define sp_QuickFix	28
#define sp_SafetySpell	29
#define sp_DreamSpell	30
#define sp_HaltFoe	31
#define sp_MeleeMen	32
#define sp_BatchSpell	33
#define sp_Camaraderie	34
#define sp_None		35

#endif
