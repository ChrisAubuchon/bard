#ifndef _B3_MON_H
#define _B3_MON_H

extern uint8_t	*sumMacro[];

typedef struct {
	uint8_t		type;
	uint8_t		damage;
} _b3monAtt_t;

typedef struct {
	uint8_t		name[16];
	uint8_t		hpDice;
	uint16_t	hpBase;
	uint8_t		distance;
	uint8_t		packedGender;
	uint8_t		groupSize;
	_b3monAtt_t	att[4];
	uint8_t		breathFlag;
	uint8_t		breathRange;
	uint8_t		picIndex;
	uint8_t		rewardLo;
	uint8_t		rewardMid;
	uint8_t		rewardHi;
	uint8_t		flags;
	uint8_t		breathSaveLo;
	uint8_t		breathSaveHi;
	uint8_t		priorityLo;
	uint8_t		priorityHi;
	uint8_t		strongElement;
	uint8_t		weakElement;
	uint8_t		repelFlags;
	uint8_t		toHitLo;
	uint8_t		toHitHi;
	uint8_t		spellSaveLo;
	uint8_t		spellSaveHi;
} b3mon_t;

extern b3mon_t	sumMons[];
extern uint8_t	*monMacro[];

#endif

