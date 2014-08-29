#ifndef _CNV_DUN_H
#define _CNV_DUN_H

#include <cnvlib.h>
#include <cnv_param.h>

/********************************/
/*				*/
/* Structures			*/
/*				*/
/********************************/

typedef struct dunInfo_t dunInfo_t;
typedef struct dunGraph_t dunGraph_t;

typedef struct {
	btstring_t	*name;
	cnvList_t	*levels;
} dun_t;

typedef struct {
	btstring_t	*dstSquare;
	btstring_t	*gfx;
	uint8_t		isSecret;
	uint8_t		canPhase;
	uint8_t		isWall;
	uint8_t		isDoor;
} dunPath_t;

typedef struct {
	btstring_t	*label;
	dunPath_t	north;
	dunPath_t	south;
	dunPath_t	east;
	dunPath_t	west;
	uint32_t	value;
	uint8_t		isSpinner;
	uint8_t		isSmoke;
	uint8_t		isNoSpptRegen;
	uint8_t		isSpptRegen;
	uint8_t		isSpptDrain;
	uint8_t		isAntiMagic;
	uint8_t		isLifeDrain;
	uint8_t		isHpRegen;
	uint8_t		isDarkness;
	uint8_t		isTrap;
	uint8_t		isStuck;
	uint8_t		isOdd;
	uint8_t		isSilent;
	uint8_t		isExplosion;
	uint8_t		hasCeilPortal;
	uint8_t		hasFloorPortal;
	uint8_t		isRandomBattle;
	uint8_t		isSpecial;
	uint8_t		isMakeHostile;
	btstring_t	*isTeleport;
	btstring_t 	*isStairs;
	btstring_t 	*isMessage;
	btstring_t	*isForcedBattle;
	btstring_t	*code;
} dunVertex_t;

typedef struct {
	btstring_t 	*title;
	btstring_t	*name;
	btstring_t	*path;
	uint8_t		tileSet;
	uint8_t		poisonDmg;
	uint8_t		level;
	uint8_t		dungeonLevel;
	uint8_t		dungeonDirection;
	uint8_t		canTeleportTo;
	uint8_t		canTeleportFrom;
	dunGraph_t	*graph;
	cnvList_t	*items;
	cnvList_t	*floorTraps;
	cnvList_t	*chestTraps;
	cnvList_t	*monsters;

	paramList_t	*params;
} dunLevel_t;

#define	DIR_UPWARD	0
#define DIR_DOWNWARD	1


#define ENT_STAIRS	0
#define ENT_PORTAL	1
#define ENT_ENTRYWAY	2
#define ENT_MOUNTBASE	3
#define	ENT_VANTAGE	4
#define ENT_WAYIN	5
#define ENT_THEEXIT	6

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

dunVertex_t	*dunVertex_new(dunLevel_t *dl, uint32_t value);
dunVertex_t	*dunVertex_get(dunLevel_t *dl, uint32_t value);
dunLevel_t	*dunLevel_new(void);

cnvList_t	*dunList_new(void);
void		dunList_to_json(cnvList_t *dl, btstring_t *fname); 

dun_t		*dun_new(btstring_t *name);

#endif
