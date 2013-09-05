#ifndef _LIBCNV_COMMON_H
#define _LIBCNV_COMMON_H

#include <cnvlib.h>

#define ACT_NONE        0
#define ACT_MELEE       1
#define ACT_SPELL       2
#define ACT_DOPPEL      3
#define ACT_BREATH      4
#define ACT_FIRE        5
#define ACT_SUMMONHELP  6
#define ACT_TARJAN      7
#define ACT_USE         8

#define FIRE_NONE	0
#define FIRE_ARROW	1
#define FIRE_SPEAR	2
#define FIRE_AXE	3
#define FIRE_SHURIKEN	4
#define FIRE_BOOMERANG	5
#define FIRE_HAMMER	6
#define FIRE_BLADE	7
#define FIRE_LIGHT	8


uint8_t *getSpecialAttack(uint8_t sp);
uint8_t *getAttypeString(uint8_t at);
uint8_t *getActionString(uint8_t a);
uint8_t *getMeleeString(uint8_t m);
uint8_t *getFireString(uint8_t f);
uint8_t *getUseString(uint8_t u);
uint8_t *getPronounString(uint8_t p);
uint8_t *getSubtype(uint8_t type, uint8_t subtype);

json_t	*meleeString_toJson(uint8_t index);

#endif
