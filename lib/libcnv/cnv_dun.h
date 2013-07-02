#ifndef _CNV_DUN_H
#define _CNV_DUN_H

#include <cnvlib.h>

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
	btstring_t	*label;
	uint32_t	value;
	btstring_t	*sq1;
	btstring_t 	*gfx1;
	uint8_t		secret1;
	uint8_t		canPhase1;
	uint8_t		isWall1;

	btstring_t	*sq2;
	btstring_t	*gfx2;
	uint8_t		secret2;
	uint8_t		canPhase2;
	uint8_t		isWall2;
} dunEdge_t;

typedef struct {
	btstring_t	*label;
	btstring_t 	*northEdge;
	btstring_t	*southEdge;
	btstring_t	*eastEdge;
	btstring_t	*westEdge;
	uint32_t	value;
	uint8_t		isSpinner;
	uint8_t		isSmoke;
	uint8_t		isNoSpptRegen;
	uint8_t		isAntiMagic;
	uint8_t		isLifeDrain;
	uint8_t		isDarkness;
	uint8_t		isTrap;
	uint8_t		isStuck;
	uint8_t		hasCeilPortal;
	uint8_t		hasFloorPortal;
	uint8_t		isRandomBattle;
	uint8_t		isSpecial;
	btstring_t	*isTeleport;
	btstring_t 	*isStairs;
	btstring_t 	*isMessage;
} dunVertex_t;

typedef struct {
	btstring_t 	*title;
	btstring_t	*name;
	btstring_t	*path;
	uint8_t		poisonDmg;
	uint8_t		tileSet;
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

#if 0
typedef struct {
	Bitfield(stairs, 1);
	Bitfield(special, 1);
	Bitfield(darkness, 1);
	Bitfield(trap, 1);
	Bitfield(portbelow, 1);
	Bitfield(portabove, 1);
	Bitfield(highbat, 1);
	Bitfield(spinner, 1);
	Bitfield(antimagic, 1);
	Bitfield(drainhp, 1);
	Bitfield(odd, 1);
	Bitfield(silence, 1);
	Bitfield(regensppt, 1);
	Bitfield(drainsppt, 1);
	Bitfield(monhost, 1);
	Bitfield(explosion, 1);
	Bitfield(hpregen, 1);
	Bitfield(stuck, 1);
	Bitfield(smoke, 1);
	Bitfield(teleonly, 1);
} sqFlags_t;

typedef struct {
	uint8_t nopass;
	uint8_t canphase;
	uint8_t secret;

	uint8_t style;
} dunWall_t;

typedef struct {
	uint8_t x;
	uint8_t y;

	dunWall_t n_wall;
	dunWall_t s_wall;
	dunWall_t e_wall;
	dunWall_t w_wall;
	sqFlags_t flags;
} dunSquare_t;

/*
typedef struct {
  uint8_t nitems;
  btstring_t **item;
  uint32_t weight_total;
  uint32_t *weight;
} dunItems_t;
*/

typedef struct {
	uint32_t weight;
	btstring_t *name;
} dunItem_t;

typedef struct {
	btstring_t *tag;
	btstring_t *name;

	bt_array_t *chest_traps;
	bt_array_t *floor_traps;
	bt_array_t *monsters;

	btstring_t *codeBuffer;
	bt_array_t *codeOffsets;

	uint32_t item_total_weight;
	bt_array_t *items;

	uint8_t poisonDmg;
	uint8_t spptDrain_lo;
	uint8_t spptDrain_hi;
	uint8_t hpDrain_lo;
	uint8_t hpDrain_hi;

	uint8_t width;
	uint8_t height;
	int8_t deltaSqE;
	int8_t deltaSqN;
	uint8_t exitSqN;
	uint8_t exitSqE;
	uint8_t exitLoc;
	uint8_t wrapFlag;

	uint8_t useDunBG;

	dunSquare_t **squares;
} dunLevel_t;

typedef struct {
	btstring_t *name;
	btstring_t *gfx_tile;
	uint8_t entry_type;
	uint8_t entrySqN;
	uint8_t entrySqE;
	uint8_t direction;

	uint8_t nlevs;
	dunLevel_t **levs;
} dun_t;
#endif

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

dunEdge_t	*dunEdge_new(dunLevel_t *dl, uint32_t value);
dunVertex_t	*dunVertex_new(dunLevel_t *dl, uint32_t value);
dunVertex_t	*dunVertex_get(dunLevel_t *dl, uint32_t value);
dunLevel_t	*dunLevel_new(void);

cnvList_t	*dunList_new(void);
void		dunList_to_json(cnvList_t *dl, btstring_t *fname); 

dun_t		*dun_new(btstring_t *name);

#endif
