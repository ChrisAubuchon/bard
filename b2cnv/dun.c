#include <b2lib.h>
#include <dun_level.h>
#include <dehuf.h>
#include <dun.h>
#include <level.h>
#include <cnv_dun.h>

#define DEBUG
#include <debug.h>

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
static dunLevel_t	*convertDunLevel(btstring_t *data, btstring_t *monsters,
				uint32_t dunno, uint32_t levno);
static uint32_t		getEdge(int32_t x, int32_t y, uint32_t dir);
static uint32_t		getVertex(int32_t x, int32_t y);

static void		setEdges(dunLevel_t *dl, dungeonLevel_t *l, int32_t x, 
				int32_t y, uint8_t walls);
static void		setVertex(dunLevel_t *dl, dungeonLevel_t *l,
				int32_t x, int32_t y, uint32_t index,
				uint32_t levno);
static void		setEdgeWall(dungeonLevel_t *l, dunEdge_t *edge,
				int32_t x, int32_t y, uint8_t square,
				uint8_t dir);
static uint8_t		wrapNumber(int8_t inN);
static void		convertStairs(dungeonLevel_t *l, uint32_t index,
				dunVertex_t *v, uint32_t levno);
static void		addSpecialSquares(dunLevel_t *dl, dungeonLevel_t *l);

static void 		getTeleport(dunLevel_t *dl, dungeonLevel_t *l, 
				uint32_t index);
static void 		getInfoMessage(dunLevel_t *dl, dungeonLevel_t *l, 
				uint32_t index);
static void 		getRequiredBattle(dunLevel_t *dl, dungeonLevel_t *l, 
				uint32_t index);

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

#define doLabel(side,srcX,srcY,dstX,dstY) \
	(((side) << 21) | (((srcX)&0x1f) << 15) | (((srcY)&0x1f) << 10) | \
	(((dstX)&0x1f) << 5) | ((dstY)&0x1f))

static uint32_t getEdge(int32_t x, int32_t y, uint32_t dir)
{
	uint32_t	rval = 0;
	switch (dir) {
	case DIR_NORTH:
		rval = doLabel(1, x, y, x, wrapNumber(y+1));
		break;
	case DIR_SOUTH:
		rval = doLabel(1, x, wrapNumber(y-1), x, y);
		break;
	case DIR_WEST:
		rval = doLabel(2, wrapNumber(x-1), y, x, y);
		break;
	case DIR_EAST:
		rval = doLabel(2, x, y, wrapNumber(x+1), y);
		break;
	}

	return rval;
}
#undef doLabel

static void setEdgeWall(dungeonLevel_t *l, dunEdge_t *edge, int32_t x, 
				int32_t y, uint8_t square, uint8_t dir)
{
	switch (dir) {
	case DIR_NORTH:
		edge->sq1 = getNodeLabel(x, y);
		if (square & NORTH_MASK) {
			edge->canPhase1 = l->phaseFlag;
			if (square & WALL_NORTH) {
				edge->gfx1 = bts_strcpy("wall");
				if (square & DOOR_NORTH) {
					edge->isDoor1 = 1;
					edge->secret1 = 1;
				} else {
					edge->isWall1 = 1;
				}
			} else {
				edge->isDoor1 = 1;
				edge->gfx1 = bts_strcpy("door");
			}
		}
		break;
	case DIR_SOUTH:
		edge->sq2 = getNodeLabel(x, y);
		if (square & SOUTH_MASK) {
			edge->canPhase2 = l->phaseFlag;
			if (square & WALL_SOUTH) {
				edge->gfx2 = bts_strcpy("wall");
				if (square & DOOR_SOUTH) {
					edge->isDoor2 = 1;
					edge->secret2 = 1;
				} else {
					edge->isWall2 = 1;
				}
			} else {
				edge->isDoor2 = 1;
				edge->gfx2 = bts_strcpy("door");
			}
		}
		break;
	case DIR_WEST:
		edge->sq1 = getNodeLabel(x, y);
		if (square & WEST_MASK) {
			edge->canPhase1 = l->phaseFlag;
			if (square & WALL_WEST) {
				edge->gfx1 = bts_strcpy("wall");
				if (square & DOOR_WEST) {
					edge->isDoor1 = 1;
					edge->secret1 = 1;
				} else {
					edge->isWall1 = 1;
				}
			} else {
				edge->isDoor1 = 1;
				edge->gfx1 = bts_strcpy("door");
			}
		}
		break;
	case DIR_EAST:
		edge->sq2 = getNodeLabel(x, y);
		if (square & EAST_MASK) {
			edge->canPhase2 = l->phaseFlag;
			if (square & WALL_EAST) {
				edge->gfx2 = bts_strcpy("wall");
				if (square & DOOR_EAST) {
					edge->isDoor2 = 1;
					edge->secret2 = 1;
				} else {
					edge->isWall2 = 1;
				}
			} else {
				edge->isDoor2 = 1;
				edge->gfx2 = bts_strcpy("door");
			}
		}
		break;
	}
}

static void setEdges(dunLevel_t *dl, dungeonLevel_t *l, int32_t x, int32_t y, 
			uint8_t walls)
{
	dunEdge_t	*edge;

	edge	= dunEdge_new(dl, getEdge(x, y, DIR_NORTH));
	setEdgeWall(l, edge, x, y, walls, DIR_NORTH);
	edge	= dunEdge_new(dl, getEdge(x, y, DIR_SOUTH));
	setEdgeWall(l, edge, x, y, walls, DIR_SOUTH);
	edge	= dunEdge_new(dl, getEdge(x, y, DIR_EAST));
	setEdgeWall(l, edge, x, y, walls, DIR_EAST);
	edge	= dunEdge_new(dl, getEdge(x, y, DIR_WEST));
	setEdgeWall(l, edge, x, y, walls, DIR_WEST);
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
	"if (currentLevel:doStairs(\"%s\")) then \
		currentLevel:changeLevel(%d)\
	end",
	"if (currentLevel:doStairs(\"%s\")) then \
		currentLevel:exitToCity() \
	end"
};

static void convertStairs(dungeonLevel_t *l, uint32_t index, dunVertex_t *v, 
				uint32_t levno)
{
	uint8_t		direction;
	uint8_t		stairCodeIndex = 0;
	int8_t		newLevel = 0;

	direction = (l->direction & 1) ^ ((l->loFlags[index] >> 1) & 1);
	if (l->loFlags[index] & DUN_DOWNSTAIR) {
		newLevel = 1;
	} else {
		if (!levno)
			stairCodeIndex = 1;
		newLevel = -1;
	}

	v->isStairs = bts_sprintf(stairCode[stairCodeIndex],
				direction ? "down" : "up",
				newLevel);
}

static void setVertex(dunLevel_t *dl, dungeonLevel_t *l, int32_t x, 
			int32_t y, uint32_t index, uint32_t levno)
{
	dunVertex_t	*vertex;

	vertex = dunVertex_new(dl, getVertex(x, y));
	vertex->northEdge = bts_sprintf("x%08x", getEdge(x,y,DIR_NORTH));
	vertex->southEdge = bts_sprintf("x%08x", getEdge(x,y,DIR_SOUTH));
	vertex->eastEdge = bts_sprintf("x%08x", getEdge(x,y,DIR_EAST));
	vertex->westEdge = bts_sprintf("x%08x", getEdge(x,y,DIR_WEST));

	if (l->loFlags[index] & DUN_SPECIAL) {
		vertex->isSpecial = 1;
	}

	if (l->loFlags[index] & DUN_DARKNESS) {
		vertex->isDarkness = 1;
	}

	if (l->loFlags[index] & DUN_TRAP) {
		vertex->isTrap = 1;
	}

	if (l->loFlags[index] & DUN_CEILPORT) {
		vertex->hasCeilPortal = 1;
	}

	if (l->loFlags[index] & DUN_FLOORPORT) {
		vertex->hasFloorPortal = 1;
	}

	if (l->loFlags[index] & DUN_BATTLE) {
		vertex->isRandomBattle = 1;
	}

	if (l->loFlags[index] & STAIR_MASK) {
		convertStairs(l, index, vertex, levno);
	}

	if (l->hiFlags[index] & HI_SPINNER) {
		vertex->isSpinner = 1;
	}

	if (l->hiFlags[index] & HI_ANTIMAGIC) {
		vertex->isAntiMagic = 1;
	}

	if (l->hiFlags[index] & HI_DAMAGE) {
		vertex->isLifeDrain = 1;
	}

	if (l->hiFlags[index] & HI_SOMETHINGODD) {
		vertex->isOdd = 1;
	}

	if (l->hiFlags[index] & HI_ENDTUNE) {
		vertex->isSilent = 1;
	}

	if (l->hiFlags[index] & HI_SPREGEN) {
		vertex->isSpptRegen = 1;
	}

	if (l->hiFlags[index] & HI_DRAINSPPT) {
		vertex->isSpptRegen = l->songLevel;
	}

	if (l->hiFlags[index] & HI_HOSTILEMON) {
		vertex->isMakeHostile = 1;
	}
}

/********************************/
/*				*/
/* Special square code		*/
/*				*/
/********************************/

static void getInfoMessage(dunLevel_t *dl, dungeonLevel_t *l, uint32_t index)
{
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

	setSquareMember(dl, 
		l->infoMess[index].sqEast,
		l->infoMess[index].sqNorth,
		isMessage,
		bts_strncpy(&l->text[start], offset - start)
		);
}

static void getTeleport(dunLevel_t *dl, dungeonLevel_t *l, uint32_t index)
{
	if ((l->teleporter[index].sqNorth == l->teleDest[index].sqNorth) &&
	    (l->teleporter[index].sqEast == l->teleDest[index].sqEast))
		return;

	setSquareMember(dl,
		l->teleporter[index].sqEast,
		l->teleporter[index].sqNorth,
		isTeleport,
		bts_sprintf("x%04x", getVertex(l->teleDest[index].sqEast,
					       l->teleDest[index].sqNorth))
		);
}

static void getRequiredBattle(dunLevel_t *dl, dungeonLevel_t *l, uint32_t index)
{
	if ((!l->reqBattle[index].sqNorth) && (!l->reqBattle[index].sqEast) &&
	    (!l->reqBatMon[index].monName) && (!l->reqBatMon[index].monSize))
		return;
	debug("reqBattle: (%2d,%2d) - (%2d,%2d)\n",
		l->reqBattle[index].sqNorth,
		l->reqBattle[index].sqEast,
		l->reqBatMon[index].monName,
		l->reqBatMon[index].monSize);
}

static void addSpecialSquares(dunLevel_t *dl, dungeonLevel_t *l)
{
	uint32_t	i;

	for (i = 0; i < 8; i++) {
		getInfoMessage(dl, l, i);
		getTeleport(dl, l, i);
		getRequiredBattle(dl, l, i);
	}
}

/********************************/
/*				*/
/* Level functions		*/
/*				*/
/********************************/

static dunLevel_t *convertDunLevel(
	btstring_t	*data, 
	btstring_t	*monsters,
	uint32_t	dunno,
	uint32_t	levno
	)
{
	dunLevel_t	*dl;
	dungeonLevel_t	*l;
	uint32_t	i, index;
	int32_t		x, y;

	dl	= dunLevel_new();
	l	= (dungeonLevel_t *)data->buf;

	dl->title	= unmaskString(l->title, 14);
	dl->name	= bts_sprintf("%s-%d", b2dungeons[dunno].name, levno);
	dl->path	= mkJsonPath("");

	/* XXX - Might have to be songLevel + 1; */
	dl->level	= l->songLevel + 1;
	dl->poisonDmg	= poisonDamageList[dl->level];

	dl->dungeonLevel	= levno;
	dl->dungeonDirection	= l->direction ? DIR_UPWARD : DIR_DOWNWARD;

	index = 0;
	for (y = 0; y < 22; y++) {
		for (x = 0; x < 22; x++) {
			setEdges(dl, l, x, y, l->wallData[index]);
			setVertex(dl, l, x, y, index, levno);
			index++;
		}
	}

	addSpecialSquares(dl, l);

	return dl;
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
	btstring_t	*data;
	btstring_t	*monsters;
	dunLevel_t	*dl;

	fp = openLevelFile();
	nlevs = fp_read32le(fp) / sizeof(uint32_t);

	dungeons = dunList_new();

	for (dunno = 0; dunno < 8; dunno++) {
		dun_t	*d;

		d = dun_new(bts_strcpy(b2dungeons[dunno].name));
		for (levno = 0; levno < b2dungeons[dunno].numLevels; levno++) {
			fp_moveToIndex32(fp, b2dungeons[dunno].levels[levno],0);

			huf = dehuf_init(fp);
			data = dehuf(huf, 0x900);
			monsters = dehuf(huf, 0x300);

			dl = convertDunLevel(data, monsters, dunno, levno);
			cnvList_add(d->levels, dl);

			dehuf_free(huf);
			bts_free(data);
			bts_free(monsters);
		}

		cnvList_add(dungeons, d);
	}

	dunList_to_json(dungeons, mkJsonPath("dungeons.json"));
	cnvList_free(dungeons);

	fclose(fp);
}
