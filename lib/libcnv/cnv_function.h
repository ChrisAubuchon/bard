#ifndef _LIBCNV_CNV_FUNCTION_H
#define _LIBCNV_CNV_FUNCTION_H

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#define FUNC_NONE		0
#define FUNC_ATTACK		1
#define FUNC_DISBELIEVE		2
#define FUNC_BONUS		3
#define FUNC_HEAL		4
#define FUNC_SUMMON		5
#define FUNC_TRAPZAP		6
#define FUNC_MAGESTAR		7
#define FUNC_TELEPORT		8
#define FUNC_SCRYSITE		9
#define FUNC_ANTIMAGIC		10
#define FUNC_PHASEDOOR		11
#define FUNC_SPELLBIND		12
#define FUNC_SAFETYSPELL	13
#define FUNC_DREAMSPELL		14
#define FUNC_BATCHSPELL		15
#define FUNC_CAMARADERIE	16
#define FUNC_DIVINE		17
#define FUNC_IDENTIFY		18
#define FUNC_POSSESS		19
#define FUNC_EARTHMAW		20
#define FUNC_CHRONO		21
#define FUNC_MAPSPELL		22
#define FUNC_REENERGIZE		23
#define FUNC_SUMMONHELP		24
#define FUNC_TARJAN		25
#define FUNC_DOPPEL		26
#define FUNC_LIGHT		27
#define FUNC_COMPASS		28
#define FUNC_LEVITATE		29
#define FUNC_SHIELD		30
#define FUNC_DETECT		31
#define FUNC_STRING		32

/********************************/
/*				*/
/* Structures			*/
/*				*/
/********************************/

typedef struct {
	uint8_t		type;
	btstring_t	*string;
} btFunction_t;

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

btFunction_t	*btFunction_new(uint8_t type);
void		btFunction_free(const void *vbf);
json_t		*btFunction_toJson(const void *vbf);

#endif
