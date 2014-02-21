#include <b2lib.h>
#include <dun_level.h>
#include <dehuf.h>
#include <dun.h>
#include <level.h>
#include <trap.h>
#include <cnv_trap.h>
#include <cnv_dun.h>

#define DEBUG
#include <debug.h>

/********************************/
/*				*/
/* Internal structures		*/
/*				*/
/********************************/

typedef struct {
	dunLevel_t	*dst;
	dungeonLevel_t	*src;
	btstring_t	*data;
	btstring_t	*monsters;
	uint32_t	levno;
	uint32_t	dunno;
} b2level_t;

/********************************/
/*				*/
/* Internal macros		*/
/*				*/
/********************************/

#define DIR_NORTH	0
#define DIR_SOUTH	1
#define DIR_EAST	2
#define DIR_WEST	3

/********************************/
/*				*/
/* Internal function prototypes	*/
/*				*/
/********************************/

static btstring_t	*unmaskString(uint8_t *inString, uint8_t inSize);
static btstring_t	*getNodeLabel(uint32_t x, uint32_t y);
static FILE		*openLevelFile(void);
static void		convertDunLevel(b2level_t *level);
static uint32_t		getVertex(int32_t x, int32_t y);

static void		setVertex(b2level_t *level, int32_t x, int32_t y, 
				uint32_t index);
static void		getPath(dungeonLevel_t *, uint8_t, int32_t, int32_t,
				uint8_t, dunPath_t *);
static void		convertStairs(b2level_t *level, uint32_t index,
				dunVertex_t *v);
static void		addSpecialSquares(b2level_t *level);
static void 		getTeleport(b2level_t *level, uint32_t index);
static void 		getInfoMessage(b2level_t *level, uint32_t index);
static void 		getRequiredBattle(b2level_t *level, uint32_t index);
static void		addMonsters(b2level_t *level);
static void		addItems(b2level_t *level);
static void		addTraps(b2level_t *level);

/********************************/
/*				*/
/* Internal functions		*/
/*				*/
/********************************/

static btstring_t *getNodeLabel(uint32_t x, uint32_t y)
{
	return bts_sprintf("x%02x%02x", x, y);
}

static uint8_t wrapNumber(int8_t inN)
{
	if (inN < 0) {
		return inN + 22;
	} else if (inN > 21) {
		return inN - 22;
	} else {
		return inN;
	}
}

static btstring_t *unmaskString(uint8_t *inString, uint8_t inSize)
{
	uint32_t	i;
	btstring_t	*rval;
	
	rval = bts_strncpy(inString, inSize);
	for (i = 0; i < inSize; i++) {
		rval->buf[i] ^= 0x80;
		if (rval->buf[i] == 0x7f) {
			rval->buf[i] = '\0';
			return bts_resize(rval, i);
		}
	}

	return rval;
}

static FILE *openLevelFile(void)
{
	return xfopen(mkBardTwoPath("LEVS"), "rb");
}

/********************************/
/*				*/
/* Edge functions		*/
/*				*/
/********************************/

/*
 * getPath()
 */
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

	switch (direction) {
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
		path->canPhase = !level->phaseFlag;
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

/********************************/
/*				*/
/* Vertex functions		*/
/*				*/
/********************************/

static uint32_t getVertex(int32_t x, int32_t y)
{
	return (((x & 0xff) << 8) | (y & 0xff));
}

static uint8_t *stairCode[] = {
	/* Changing dungeon level */
	"if (currentLevel:doStairs(\"%s\")) then currentLevel:toLevel(\"dun\", currentLevel.name, %d, currentLevel:getX(), currentLevel:getY(), currentLevel.direction) end",
	/* Changing location */
	"if (currentLevel:doStairs(\"%s\")) then\
		currentLevel:toLevel(\"%s\", \"%s\", %d, %d, %d, \"%s\") \
	end"
};
	

static void convertStairs(b2level_t *level, uint32_t index, dunVertex_t *v)
{
	uint8_t		direction;
	uint8_t		type;
	uint8_t		levelNo		= 0;
	uint8_t		stairCodeIndex	= 0;
	dungeonLevel_t	*src		= level->src;
	uint8_t		exitLocation	= 7;
	uint8_t		exitSqE;
	uint8_t		exitSqN;
	uint8_t		exitDir;

	direction = (src->direction & 1) ^ ((src->loFlags[index] >> 1) & 1);

	if ((src->loFlags[index] & DUN_UPSTAIR) && (!level->levno)) {
		if (src->exitLocation) 
			type = 1;
		else
			type = 0;

		exitDir = dunExitDirection[level->dunno + 1];
		exitSqN = src->exitSQN;
		exitSqE = src->exitSQE;

		switch (exitDir) {
		case 0:
			exitSqN -= 1;
			break;
		case 2:
			exitSqN += 1;
			break;
		case 1:
			exitSqE += 1;
			break;
		case 3:
			exitSqE -= 1;
			break;
		}
		v->isStairs = bts_sprintf(stairCode[1],
			direction ? "down" : "up",
			levelTypes[type],
			dunExitLocations[src->exitLocation],
			0,
			exitSqE,
			exitSqN,
			directions[exitDir]
			);
	} else {
		if (src->loFlags[index] & DUN_DOWNSTAIR) {
			levelNo = level->levno + 1;
		} else {
			levelNo = level->levno - 1;
		}

		v->isStairs = bts_sprintf(stairCode[0],
			direction ? "down" : "up",
			levelNo
			);
	}
}


static void setVertex(b2level_t *level, int32_t x, int32_t y, uint32_t index) 
{
	dunVertex_t	*vertex;

	vertex = dunVertex_new(level->dst, getVertex(x, y));

	getPath(level->src, level->src->wallData[index], x, y, DIR_NORTH,
		&vertex->north);
	getPath(level->src, level->src->wallData[index], x, y, DIR_SOUTH,
		&vertex->south);
	getPath(level->src, level->src->wallData[index], x, y, DIR_EAST,
		&vertex->east);
	getPath(level->src, level->src->wallData[index], x, y, DIR_WEST,
		&vertex->west);

	if (level->src->loFlags[index] & DUN_SPECIAL) {
		vertex->isSpecial = 1;
	}

	if (level->src->loFlags[index] & DUN_DARKNESS) {
		vertex->isDarkness = 1;
	}

	if (level->src->loFlags[index] & DUN_TRAP) {
		vertex->isTrap = 1;
	}

	if (level->src->loFlags[index] & DUN_CEILPORT) {
		vertex->hasCeilPortal = 1;
	}

	if (level->src->loFlags[index] & DUN_FLOORPORT) {
		vertex->hasFloorPortal = 1;
	}

	if (level->src->loFlags[index] & DUN_BATTLE) {
		vertex->isRandomBattle = 1;
	}

	if (level->src->loFlags[index] & STAIR_MASK) {
		convertStairs(level, index, vertex);
	}

	if (level->src->hiFlags[index] & HI_SPINNER) {
		vertex->isSpinner = 1;
	}

	if (level->src->hiFlags[index] & HI_ANTIMAGIC) {
		vertex->isAntiMagic = 1;
	}

	if (level->src->hiFlags[index] & HI_DAMAGE) {
		vertex->isLifeDrain = 1;
	}

	if (level->src->hiFlags[index] & HI_SOMETHINGODD) {
		vertex->isOdd = 1;
	}

	if (level->src->hiFlags[index] & HI_ENDTUNE) {
		vertex->isSilent = 1;
	}

	if (level->src->hiFlags[index] & HI_SPREGEN) {
		vertex->isSpptRegen = 1;
	}

	if (level->src->hiFlags[index] & HI_DRAINSPPT) {
		vertex->isSpptDrain = level->src->songLevel;
	}

	if (level->src->hiFlags[index] & HI_HOSTILEMON) {
		vertex->isMakeHostile = 1;
	}
}

/********************************/
/*				*/
/* Special square code		*/
/*				*/
/********************************/

static void getInfoMessage(b2level_t *level, uint32_t index)
{
	dunLevel_t	*dl = level->dst;
	dungeonLevel_t	*l = level->src;
	uint16_t	start;
	uint16_t	offset;

	if (!l->infoOffset[index])
		return;

	start	= l->infoOffset[index] - 160;
	offset	= start;

	while ((l->text[offset] & 0x7f) != 0x7f) {
		l->text[offset] &= 0x7f;
		if (!l->text[offset])
			l->text[offset] = '\n';
		offset++;
	}

	setSquareMemberIfSpecial(dl, 
		l->infoMess[index].sqEast,
		l->infoMess[index].sqNorth,
		isMessage,
		bts_strncpy(&l->text[start], offset - start)
		);
}

static void getTeleport(b2level_t *level, uint32_t index)
{
	dunLevel_t	*dl = level->dst;
	dungeonLevel_t	*l = level->src;
	
	if ((l->teleporter[index].sqNorth == l->teleDest[index].sqNorth) &&
	    (l->teleporter[index].sqEast == l->teleDest[index].sqEast))
		return;

	setSquareMemberIfSpecial(dl,
		l->teleporter[index].sqEast,
		l->teleporter[index].sqNorth,
		isTeleport,
		bts_sprintf("x%04x", getVertex(l->teleDest[index].sqEast,
					       l->teleDest[index].sqNorth))
		);
}

static void getRequiredBattle(b2level_t *level, uint32_t index)
{
	btstring_t	*code;
	btstring_t	*mString;
	dungeonLevel_t	*l = level->src;

	if ((!l->reqBattle[index].sqNorth) && (!l->reqBattle[index].sqEast) &&
	    (!l->reqBatMon[index].monName) && (!l->reqBatMon[index].monSize))
		return;

	mString = getMonsterByIndex(level->monsters, 
				l->reqBatMon[index].monName,
				b2dungeons[level->dunno].levels[level->levno]
				);

	code = bts_sprintf("battle:new(\"%.*s\", %d)",
		mString->size,
		mString->buf,
		l->reqBatMon[index].monSize
		);
	bts_free(mString);

	setSquareMemberIfSpecial(level->dst,
		l->reqBattle[index].sqEast,
		l->reqBattle[index].sqNorth,
		isForcedBattle,
		code
		);
}

static void getSpecialSquare(b2level_t *level, uint32_t index)
{
	dungeonLevel_t	*l = level->src;
	dunLevel_t	*dl = level->dst;
	uint8_t		square;

	if ((!l->magicMouth[index].sqNorth) && (!l->magicMouth[index].sqEast) &&
	    (!l->mouthNum[index].sqNumber))
		return;

	square = l->mouthNum[index].sqNumber - 32;
	setSquareMember(dl,
		l->magicMouth[index].sqNorth,
		l->magicMouth[index].sqEast,
		code,
		bts_strcpy(b2dun_specialCode[square])
		);
}

static void addSpecialSquares(b2level_t *level)
{
	uint32_t	i;

	for (i = 0; i < 8; i++) {
		getInfoMessage(level, i);
		getTeleport(level, i);
		getRequiredBattle(level, i);
		getSpecialSquare(level, i);
	}

	if ((level->dunno == 7) && (level->levno == 0)) {
		setSquareMember(level->dst,
			18, 9, code, bts_strcpy(b2dun_specialCode[99]));
	}
}

/********************************/
/*				*/
/* Level functions		*/
/*				*/
/********************************/

static void addMonsters(b2level_t *level)
{
	getMonsterNameList(level->dst->monsters, level->monsters, 
		b2dungeons[level->dunno].levels[level->levno]);
}

static void addItems(b2level_t *level)
{
	uint32_t 	i;
	uint32_t	ilevel = level->src->songLevel;

	for (i = 0; i < rewardItemRange[ilevel]; i++) {
		cnvList_add(level->dst->items,
			getItemName(rewardItemBase[ilevel]+i));
	}
}

static void addTraps(b2level_t *level)
{
	dungeonLevel_t	*l;
	dunLevel_t	*dl;
	uint32_t	dlevel;
	uint32_t	i;
	uint32_t	trapIndex;
	trap_t		*t;

	l	= level->src;
	dl	= level->dst;
	dlevel	= l->songLevel;

	for (i = 0; i < 8; i++) {
		t = trap_new();

		t->name		= bts_strcpy(dun_trap_macros[i]);
		t->effectString	= bts_strcpy(dun_trap_strings[i]);
		t->partyFlag	= dun_trap_partyFlag[i];
		t->spAttack	= dun_trap_spAttack[i];
		t->ndice	= dun_trap_dice[i] * dlevel;
		t->dieval	= 4;

		/* 
		 * BT2 doesn't do saving throw for floor traps. It's
		 * a straight 50% chance.
		 */
		t->sv_lo	= 0;
		t->sv_hi	= 0;

		cnvList_add(dl->floorTraps, t);
	}

	for (i = 0; i < 4; i++) {
		t = trap_new();

		trapIndex = chest_trap_by_level[dlevel][i];
		if (trapIndex) {
			t->name	= bts_strcpy(chest_trap_strings[trapIndex]);
			t->effectString = NULL;
			t->partyFlag	= (trapIndex < 3) ? 1 : 0;
			t->spAttack	= chest_trap_spAttack[trapIndex];
			t->ndice	= chest_trap_dice[trapIndex] * dlevel;
			t->dieval	= 4;

			t->sv_lo	= 0;
			t->sv_hi	= 0;
		} else {
			t->name = NULL;
		}

		cnvList_add(dl->chestTraps, t);
	}
}

static void convertDunLevel(b2level_t *level)
{
	uint32_t	i, index;
	int32_t		x, y;
	dunLevel_t	*dl;
	dungeonLevel_t	*l;

	level->dst	= dunLevel_new();
	dl = level->dst;
	level->src	= (dungeonLevel_t *)level->data->buf;
	l = level->src;

	dl->title	= unmaskString(l->title, 14);
	dl->name	= bts_sprintf("%s-%d", 
				b2dungeons[level->dunno].name, level->levno);
	dl->path	= mkJsonPath("");

	/* XXX - Might have to be songLevel + 1; */
	dl->level	= l->songLevel;
	dl->poisonDmg	= poisonDamageList[l->songLevel];

	dl->dungeonLevel	= level->levno + 1;
	dl->dungeonDirection	= l->direction ? DIR_UPWARD : DIR_DOWNWARD;

	dl->canTeleportFrom	= 1;
	dl->canTeleportTo	= !level->src->canTeleportToFlag[level->levno];

	index = 0;
	for (y = 0; y < 22; y++) {
		for (x = 0; x < 22; x++) {
			setVertex(level, x, y, index);
			index++;
		}
	}

	addSpecialSquares(level);
	addMonsters(level);
	addItems(level);
	addTraps(level);
}

/********************************/
/*				*/
/* Public functions		*/ 
/*				*/
/********************************/

uint32_t getMaxLevels(void)
{
	FILE		*fp;
	uint32_t	rval;

	fp = openLevelFile();

	rval = fp_read32le(fp) / 4;

	fclose(fp);

	return rval;
}

btstring_t *getMonsters(uint32_t level)
{
	FILE		*fp;
	huffile_t	*huf;
	btstring_t	*data;

	fp = openLevelFile();
	fp_moveToIndex32(fp, level, 0);

	huf = dehuf_init(fp);
	data = dehuf(huf, 0x900);
	bts_free(data);
	data = dehuf(huf, 0x300);

	fclose(fp);

	dehuf_free(huf);

	return data;
	
}

void convertDungeons(void)
{
	FILE		*fp;
	uint32_t	nlevs, dunno, levno;
	cnvList_t	*dungeons;
	huffile_t	*huf;
	b2level_t	level;

	fp = openLevelFile();
	nlevs = fp_read32le(fp) / sizeof(uint32_t);

	dungeons = dunList_new();

	for (dunno = 0; dunno < 8; dunno++) {
		dun_t	*d;

		d = dun_new(bts_strcpy(b2dungeons[dunno].name));
		for (levno = 0; levno < b2dungeons[dunno].numLevels; levno++) {
			fp_moveToIndex32(fp, b2dungeons[dunno].levels[levno],0);

			huf = dehuf_init(fp);
			level.data	= dehuf(huf, 0x900);
			level.monsters = dehuf(huf, 0x300);
			level.dunno	= dunno;
			level.levno	= levno;

			convertDunLevel(&level);
			cnvList_add(d->levels, level.dst);

			dehuf_free(huf);
			bts_free(level.data);
			bts_free(level.monsters);
		}

		cnvList_add(dungeons, d);
	}

	dunList_to_json(dungeons, mkJsonPath("dungeons.json"));
	cnvList_free(dungeons);

	fclose(fp);
}
