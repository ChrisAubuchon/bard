#ifndef _LIBCNV_MON_H
#define _LIBCNV_MON_H

#include <cnv_effect.h>
#include <cnv_common.h>

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

/********************************/
/*				*/
/* Structures			*/
/*				*/
/********************************/

typedef struct {
	uint8_t		type;
	btEffect_t	*effect;
} monsterAttack_t;

typedef struct {
	btstring_t	*singular;
	btstring_t	*plural;
	btstring_t	*macro;
	btstring_t	*picture;

	uint8_t		hpRndNdice;
	uint16_t	hpRndDie;
	uint16_t	hpBase;

	uint32_t	reward; 
	int8_t		baseAC; 
	
	uint8_t		breathSaveLo;
	uint8_t		breathSaveHi;
	uint8_t		spellSaveLo;
	uint8_t		spellSaveHi;
	
	uint8_t		toHitLo;
	uint8_t		toHitHi;
	uint8_t		priorityLo;
	uint8_t		priorityHi;
	
	uint8_t		rndGroupSize;
	uint8_t		groupSize;
	
	uint8_t		numAttacks;
	uint8_t		pronoun;
	uint8_t		willAdvance;
	uint8_t		distance;
	uint8_t		advSpeed;
	uint8_t		noRandom;
	uint8_t		isIllusion;
	
	elements_t	strong;
	elements_t	weak;
	repel_t		repel;

	cnvList_t	*attacks;
} monster_t;


/********************************/
/*				*/
/* Function prototypes		*/
/*				*/
/********************************/

cnvList_t	*monList_new(void);
void		monList_to_json(cnvList_t *ml, btstring_t *fname);
monster_t 	*monster_new(void);
void 		printMonXML(uint8_t indent, monster_t *m);

monsterAttack_t *monsterAttack_new(uint8_t type);


#endif
