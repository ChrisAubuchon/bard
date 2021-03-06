#include <b1lib.h>
#include <dehuf.h>
#include <cnv_dun.h>
#include <cnv_item.h>
#include <cnv_trap.h>
#include <dun_level.h>
#include <dun.h>
#include <trap.h>
#include <item.h>


#define DEBUG 1
#include <debug.h>

/****************************************/
/*					*/
/* Static Variables			*/
/*					*/
/****************************************/

#define DIR_NORTH	0
#define DIR_SOUTH	1
#define DIR_EAST	2
#define DIR_WEST	3

static uint8_t *dunNames[] = {
	"CELLARS-1",
	"SEWERS-1",
	"SEWERS-2",
	"SEWERS-3",
	"CATACOMBS-1",
	"CATACOMBS-2",
	"CATACOMBS-3",
	"CASTLE-1",
	"CASTLE-2",
	"CASTLE-3",
	"TOWER-1",
	"TOWER-2",
	"TOWER-3",
	"TOWER-4",
	"TOWER-5",
	"TOWER-6"
};

static uint8_t dungeonLevel[] = {
	1, 2, 3, 4,
	1, 2, 3,
	1, 2, 3,
	1,
	1, 2, 3, 4, 5
};

static uint8_t tilesets[] = {
	0, 0, 0, 0,
	2, 2, 2, 0,
	0, 0, 0, 1,
	1, 1, 1, 1
};

static uint8_t levelNumber[] = {
	1, 2, 3, 4,
	1, 2, 3, 1,
	2, 3, 1, 1,
	2, 3, 4, 5
};


/****************************************/
/*					*/
/* Output Function Prototypes		*/
/*					*/
/****************************************/

static int8_t		wrapNumber(int8_t in);
static btstring_t	*getNodeLabel(uint16_t x, uint16_t y);
static void		getPath(dungeonLevel_t *, uint8_t, int32_t, int32_t,
				uint8_t, dunPath_t *);
static uint32_t		getVertex(int32_t x, int32_t y);
static dunLevel_t	*convertDunLevel(btstring_t *data, uint32_t index);
static void 		convertStairs(dungeonLevel_t *l, uint32_t index, 
					dunVertex_t *v, uint32_t dunIndex);

static void 		addItems(dunLevel_t *dl);
static void 		addMonsters(dunLevel_t *dl);
static void 		addTraps(dunLevel_t *dl);

/****************************************/
/*					*/
/* Helper Functions			*/
/*					*/
/****************************************/

static btstring_t *getNodeLabel(uint16_t x, uint16_t y)
{
	return bts_sprintf("x%02x%02x", x, y);
}

static uint32_t getVertex(int32_t x, int32_t y)
{
	x &= 0xff;
	y &= 0xff;

	return ((x<<8) | y);
}

static int8_t wrapNumber(int8_t in)
{
	if (in < 0)
		return in + 22;
	else if (in >= 22) 
		return in - 22;
	else
		return in;
}

static void getPath(
	dungeonLevel_t	*level,
	uint8_t		square,
	int32_t		x,
	int32_t		y,
	uint8_t		direction,
	dunPath_t	*path
	)
{
	uint8_t		wallMask, doorMask, squareMask;
	int32_t		deltaX = 0;
	int32_t		deltaY = 0;

	switch (direction){
	case DIR_NORTH:
		wallMask = WALL_NORTH;
		doorMask = DOOR_NORTH;
		squareMask = NORTH_MASK;
		deltaY = 1;
		break;
	case DIR_SOUTH:
		wallMask = WALL_SOUTH;
		doorMask = DOOR_SOUTH;
		squareMask = SOUTH_MASK;
		deltaY = -1;
		break;
	case DIR_EAST:
		wallMask = WALL_EAST;
		doorMask = DOOR_EAST;
		squareMask = EAST_MASK;
		deltaX = 1;
		break;
	case DIR_WEST:
		wallMask = WALL_WEST;
		doorMask = DOOR_WEST;
		squareMask = WEST_MASK;
		deltaX = -1;
		break;
	}
	path->dstSquare = getNodeLabel(	wrapNumber(x + deltaX), 
					wrapNumber(y + deltaY));

	if (square & squareMask) {
		path->canPhase = !level->dun_phaseFlag;
		if (square & wallMask) {
			path->gfx = bts_strcpy("wall");
			if (square & doorMask) {
				path->isDoor = 1;
				path->isSecret = 1;
			} else {
				path->isWall = 1;
			}
		} else {
			path->gfx = bts_strcpy("door");
			path->isDoor = 1;
		}
	}
}

static uint8_t *stairCode[] = {
	"if (currentLevel:doStairs(\"%s\")) then \
		currentLevel:changeLevel(%d)\
	end",
	"if (currentLevel:doStairs(\"%s\")) then \
		currentLevel:exitToCity() \
	end"
};

static void convertStairs(dungeonLevel_t *l, uint32_t index, dunVertex_t *v,
				uint32_t dunIndex)
{
	uint8_t direction;
	uint8_t newX = 0;
	uint8_t newY = 0;
	int8_t newLevel = 0;
	uint8_t stairCodeIndex = 0;

	direction = (l->dun_direction & 1) ^ ((l->dun_flags[index] >> 1) & 1);

	if (l->dun_flags[index] & DUN_DOWNSTAIR) {
		newX = 0;
		newY = 0;
		newLevel = 1;
	} else {
		if (levelNumber[dunIndex] == 1) {
			stairCodeIndex = 1;
			
		} else {
			newY = l->dun_exitSqN;
			newX = l->dun_exitSqE;
			newLevel = -1;
		}
	}

	v->isStairs = bts_sprintf(stairCode[stairCodeIndex],
		direction ? "down" : "up",
		newLevel
		);
}

static dunLevel_t *convertDunLevel(btstring_t *data, uint32_t dunIndex)
{
	dunLevel_t	*dl;
	dungeonLevel_t	*l;
	dunVertex_t	*vertex;
	uint32_t	i, index;
	int32_t		x, y;

	dl = dunLevel_new();
	l = (dungeonLevel_t *)data->buf;
	for (i = 0; i < 10; i++) {
		l->dun_title[i] ^= 0x80;
		if (l->dun_title[i] == '\\')
			l->dun_title[i] = '\0';
	}
	l->dun_title[9] = '\0';

	dl->title	= bts_strcpy(l->dun_title);
	dl->name	= bts_strcpy(dunNames[dunIndex]);
	dl->tileSet	= tilesets[dunIndex];
	dl->level	= l->dun_monstLevel + 1;
	dl->dungeonLevel	= dungeonLevel[dunIndex];
	dl->dungeonDirection	= l->dun_direction ? DIR_UPWARD : DIR_DOWNWARD;
	dl->canTeleportTo	= l->dun_levelTeleFlag[dungeonLevel[dunIndex]-1] == 0xff ? 0 : 1;
	dl->canTeleportFrom	= l->dun_phaseFlag == 0 ? 1 : 0;
	dl->poisonDmg = b1dun_poisonDamage[l->dun_monstLevel];

	index = 0;
	for (y = 0; y < 22; y++) {
		for (x = 0; x < 22; x++) {
			vertex = dunVertex_new(dl, getVertex(x,y));
			getPath(l, l->wallData[index], x, y, DIR_NORTH, 
				&vertex->north);
			getPath(l, l->wallData[index], x, y, DIR_SOUTH, 
				&vertex->south);
			getPath(l, l->wallData[index], x, y, DIR_EAST, 
				&vertex->east);
			getPath(l, l->wallData[index], x, y, DIR_WEST, 
				&vertex->west);
			if (l->dun_flags[index] & DUN_SPECIAL) {
				vertex->isSpecial = 1;
			}
			if (l->dun_flags[index] & DUN_DARKNESS) {
				vertex->isDarkness = 1;
			}
			if (l->dun_flags[index] & DUN_TRAP) {
				vertex->isTrap = 1;
			}
			if (l->dun_flags[index] & DUN_CEILPORT) {
				vertex->hasCeilPortal = 1;
			}
			if (l->dun_flags[index] & DUN_FLOORPORT) {
				vertex->hasFloorPortal = 1;
			}
			if (l->dun_flags[index] & DUN_BATTLE) {
				vertex->isRandomBattle = 1;
			}
			if (l->dun_flags[index] & STAIR_MASK) {
				convertStairs(l, index, vertex, dunIndex);
			}
			index++;
		}
	}

	for (i = 0; i < 8; i++) {
		if (l->dun_infoMess[i].sqNorth != 0xff) {
			uint16_t start = l->dun_infoOffset[i] - 0xf800 - 0x520;
			uint16_t offset = start;

			while ((l->dun_text[offset] & 0x7f) != 0x7f)  {
				if ((l->dun_text[offset] & 0x7f) == '\\') {
					break;
				}
				l->dun_text[offset++] &= 0x7f;
			}
			setSquareMember(dl, l->dun_infoMess[i].sqEast,
					l->dun_infoMess[i].sqNorth, isMessage,
					bts_strncpy(&l->dun_text[start], offset - start));
		}
		if (l->dun_teleporter[i].sqNorth != 0xff) {
			setSquareMember(dl, l->dun_teleporter[i].sqEast,
				l->dun_teleporter[i].sqNorth, isTeleport,
				bts_sprintf("x%04x", getVertex(l->dun_teleDest[i].sqEast,
					  l->dun_teleDest[i].sqNorth))
				);
		}
		if (l->dun_spinner[i].sqNorth != 0xff) {
			setSquareFlag(dl, l->dun_spinner[i].sqEast, 
				      l->dun_spinner[i].sqNorth, isSpinner);
		}
		if (l->dun_smoke[i].sqNorth != 0xff) {
			setSquareFlag(dl, l->dun_smoke[i].sqEast,
					l->dun_smoke[i].sqNorth, isSmoke);
		}
		if (l->dun_spptRegen[i].sqNorth != 0xff) {
			setSquareFlag(dl, l->dun_spptRegen[i].sqEast,
					l->dun_spptRegen[i].sqNorth,
					isSpptRegen);
		}
		if (l->dun_antiMagic[i].sqNorth != 0xff) {
			setSquareFlag(dl, l->dun_antiMagic[i].sqEast,
					l->dun_antiMagic[i].sqNorth,
					isAntiMagic);
		}
		if (l->dun_antiMagic[i+8].sqNorth != 0xff) {
			setSquareFlag(dl, l->dun_antiMagic[i+8].sqEast,
					l->dun_antiMagic[i+8].sqNorth,
					isAntiMagic);
		}
		if (l->dun_lifeDrain[i].sqNorth != 0xff) {
			setSquareFlag(dl, l->dun_lifeDrain[i].sqEast,
					l->dun_lifeDrain[i].sqNorth,
					isLifeDrain);
		}
		if (l->dun_lifeDrain[i+8].sqNorth != 0xff) {
			setSquareFlag(dl, l->dun_lifeDrain[i+8].sqEast,
					l->dun_lifeDrain[i+8].sqNorth,
					isLifeDrain);
		}
		if (l->dun_teleonly[i].sqNorth != 0xff) {
			setSquareFlag(dl, l->dun_teleonly[i].sqEast,
					l->dun_teleonly[i].sqNorth,
					isStuck);
		}

		if (l->dun_spSquare[i].sqNorth != 0xff) {
			uint8_t index = l->dun_spSqOff[i].sqNumber - 10;

			if (b1dun_specialCode[index] != NULL) {
				setSquareMember(dl, l->dun_spSquare[i].sqEast,
					l->dun_spSquare[i].sqNorth,
					code,
					bts_strcpy(b1dun_specialCode[index])
					);
			}
		}

		if (l->dun_reqBattle[i].sqNorth != 0xff) {
			btstring_t *enc;

			enc = bts_sprintf("battle:new({\"%s\"}, %d)",
					getMonMacro(l->dun_reqBatMon[i].monName-1),
					l->dun_reqBatMon[i].monSize
					);
			setSquareMember(
					dl, 
					l->dun_reqBattle[i].sqEast,
					l->dun_reqBattle[i].sqNorth,
					isForcedBattle,
					enc
					);
		}
	}

	addItems(dl);
	addMonsters(dl);
	addTraps(dl);

	dl->path = mkJsonPath("");

	return dl;
}

static void addItems(dunLevel_t *dl)
{
	uint32_t i;

	for (i = 0; i <= b1dun_randomMask[dl->level]; i++) {
		cnvList_add(dl->items,
			getItemName(b1dun_randomOffset[dl->level] + i));
	}
}

static void addMonsters(dunLevel_t *dl)
{
	uint32_t	i;
	int32_t		offset;

	for (i = 0; i <= b1dun_randomMask[dl->level]; i++) {
		offset = b1dun_randomOffset[dl->level] + i - 1;

		if (offset < 0)
			offset = 0;
		cnvList_add(dl->monsters, getMonsterName(offset));
	}
}

static void addTraps(dunLevel_t *dl)
{
	uint32_t	level = dl->level - 1;
	uint32_t	i;
	uint8_t		trapIndex;
	trap_t		*t;

	/* Floor traps */
	for (i = 0; i < 8; i++) {
		t = trap_new();

		t->name = bts_strcpy(dun_trap_macros[i]);
		t->effectString = bts_strcpy(dun_trap_strings[i]);
		t->partyFlag = dun_trap_partyFlag[i];
		t->spAttack = dun_trap_spAttack[i];
		t->ndice = dun_trap_dice[i] * dl->level;
		t->dieval = 4;

		getSavingThrow((level << 4) + 1, &t->sv_lo, &t->sv_hi);

		cnvList_add(dl->floorTraps, t);
	}

	/* Chest traps */
	for (i = 0; i < 4; i++) {
		t = trap_new();

		trapIndex = chest_trap_by_level[level][i];

		if (trapIndex) {
			t->name = bts_strcpy(chest_trap_strings[trapIndex]);
			t->effectString = NULL;
			t->partyFlag = (trapIndex < 3) ? 1 : 0;
			t->spAttack = chest_trap_spAttack[trapIndex];
			t->ndice = chest_trap_dice[trapIndex] * dl->level;
			t->dieval = 4;

			getSavingThrow((level << 4) + 0x10, 
					&t->sv_lo, &t->sv_hi);
		} else {
			t->name = NULL;
		}

		cnvList_add(dl->chestTraps, t);
	}
}

/****************************************/
/*					*/
/* Output Functions			*/
/*					*/
/****************************************/

void convertDungeons(void)
{
	uint32_t	dunno, levno;
	uint32_t	nlevs;
	huffile_t	*huf;
	btstring_t	*data;
	cnvList_t	*dungeons;
	dunLevel_t	*level;

	FILE *fp;

	fp = xfopen(mkBardOnePath("levs"), "rb");
	nlevs = fp_read32be(fp) / sizeof(uint32_t);

	dungeons = dunList_new();

	for (dunno = 0; dunno < 5; dunno++) {
		dun_t *d;

		d = dun_new(bts_strcpy(b1dungeons[dunno].name));
		for (levno = 0; levno < b1dungeons[dunno].numLevels; levno++) {
			fp_moveToIndex32(fp, b1dungeons[dunno].levels[levno],1);

			huf = dehuf_init(fp);
			data = dehuf(huf, 0x800);

			cnvList_add(d->levels, convertDunLevel(data, 
					b1dungeons[dunno].levels[levno]));

			dehuf_free(huf);
			bts_free(data);
		}

		cnvList_add(dungeons, d);
	}

	dunList_to_json(dungeons, mkJsonPath("dungeons.json"));
	cnvList_free(dungeons);

	fclose(fp);
}
