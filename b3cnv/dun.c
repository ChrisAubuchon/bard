#include <b3lib.h>
#include <dun.h>
#include <cnv_dun.h>
#include <cnv_city.h>
#include <trap.h>

#define DEBUG
#include <debug.h>

#define DIR_NORTH	0
#define DIR_SOUTH	1
#define DIR_EAST	2
#define DIR_WEST	3


/****************************************/
/*					*/
/* Global variables			*/
/*					*/
/****************************************/

/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/

static btstring_t	*getLabel(int32_t, int32_t);

static dunLevel_t	*convertLevel(uint32_t, uint32_t);

static uint32_t		getVertex(int32_t, int32_t);
static void		setVertex(dunLevel_t *, b3level_t *, int32_t, int32_t);
static void		getPath(b3level_t *, int32_t, int32_t, uint32_t,
					dunPath_t *);
static uint8_t		wrapNumber(int32_t, uint32_t);

static void		addTraps(dunLevel_t *, b3level_t *);

/****************************************/
/*					*/
/* Local functions			*/
/*					*/
/****************************************/

/*
 * getLabel()
 */
static btstring_t *getLabel(int32_t x, int32_t y)
{
	return bts_sprintf("x%02x%02x", x, y);
}

/*
 * wrapNumber()
 */
static uint8_t wrapNumber(int32_t number, uint32_t max)
{
	if (number < 0) {
		number += max;
	} else if (number >= max) {
		number -= max;
	} else {
		return number;
	}
}

/*
 * getVertex()
 */
static uint32_t getVertex(int32_t x, int32_t y)
{
	return (((x & 0xff) << 8) | (y & 0xff));
}

/*
 * getPath()
 */
static void getPath(b3level_t *level, int32_t x, int32_t y, uint32_t dir, dunPath_t *path)
{
	uint8_t		face;
	uint8_t		style;
	uint32_t	shift;
	int32_t		deltaX	= 0;
	int32_t		deltaY	= 0;

	switch (dir) {
	case DIR_NORTH:
		shift		= 4;
		deltaY		= 1;
		break;
	case DIR_SOUTH:
		shift		= 12;
		deltaY		= -1;
		break;
	case DIR_EAST:
		shift		= 0;
		deltaX		= 1;
		break;
	case DIR_WEST:
		shift		= 8;
		deltaX		= -1;
		break;
	}

	path->dstSquare = getLabel(wrapNumber(x + deltaX, level->width),
				   wrapNumber(y + deltaY, level->height));

	face = (level->squares[y][x].walls >> shift) & 0x0f;
	if (face) {
		path->isWall = dun_wallFlag[face] ? 1 : 0;
		path->isDoor = dun_wallFlag[face] ? 0 : 1;
		path->canPhase = (face < 9) ? 1 : 0;
		style = dun_styleMap[face];

		if (style) {
			path->gfx = bts_sprintf("%d", style);
		}
	}
}

/*
 * setVertex()
 */
static void setVertex(dunLevel_t *rval, b3level_t *level, int32_t x, int32_t y)
{
	dunVertex_t	*vertex;

	vertex = dunVertex_new(rval, getVertex(x, y));

	getPath(level, x, y, DIR_NORTH, &vertex->north);
	getPath(level, x, y, DIR_SOUTH, &vertex->south);
	getPath(level, x, y, DIR_EAST, &vertex->east);
	getPath(level, x, y, DIR_WEST, &vertex->west);

	setFlag(vertex->isSpecial,	level, y, x, SQ_SPECIAL);
	setFlag(vertex->isTrap,		level, y, x, SQ_TRAP);
	setFlag(vertex->isDarkness,	level, y, x, SQ_DARKNESS);
	setFlag(vertex->hasCeilPortal,	level, y, x, SQ_PORTABOVE);
	setFlag(vertex->hasFloorPortal,	level, y, x, SQ_PORTBELOW);
	setFlag(vertex->isRandomBattle,	level, y, x, SQ_HIGHBATTLE);
	setFlag(vertex->isSpinner,	level, y, x, SQ_SPINNER);
	setFlag(vertex->isAntiMagic,	level, y, x, SQ_ANTIMAGIC);
	setFlag(vertex->isLifeDrain,	level, y, x, SQ_DRAINHP);
	setFlag(vertex->isOdd,		level, y, x, SQ_ODD);
	setFlag(vertex->isSilent,	level, y, x, SQ_SILENCE);
	setFlag(vertex->isSpptRegen,	level, y, x, SQ_REGENSPPT);
	setFlag(vertex->isSpptDrain,	level, y, x, SQ_DRAINSPPT);
	setFlag(vertex->isMakeHostile,	level, y, x, SQ_MONHOSTILE);
	setXFlag(vertex->isExplosion,	level, y, x, SQ_EXPLOSION);
	setXFlag(vertex->isHpRegen,	level, y, x, SQ_REGENHP);
	setXFlag(vertex->isStuck,	level, y, x, SQ_STUCK);
}

/*
 * addTraps()
 */
static void addTraps(dunLevel_t *rval, b3level_t *level)
{
	trap_t		*t;
	uint32_t	i;
	uint8_t		trapIndex;

	for (i = 0; i < 3; i++) {
		t	= trap_new();

		trapIndex	= dun_trap_map[(level->levFlags &  7) + i];
		t->name		= bts_strcpy(dun_trap_macros[trapIndex]);
		t->effectString	= bts_strcpy(dun_trap_strings[trapIndex]);

		cnvList_add(rval->floorTraps, t);
	}

	for (i = 0; i < 4; i++) {
		t	= trap_new();

		trapIndex	= chest_trap_map[(level->levFlags & 7) + i];
		t->name		= bts_strcpy(chest_trap_macros[trapIndex]);
		t->effectString	= bts_strcpy(chest_trap_strings[trapIndex]);

		cnvList_add(rval->chestTraps, t);
	}
}


/*
 * convertLevel()
 */
static dunLevel_t *convertLevel(uint32_t dunno, uint32_t levno)
{
	dunLevel_t	*rval;
	b3level_t	*level;
	int32_t		y, x;

	level	= readMap(duns[dunno].levels[levno]);

	rval	= dunLevel_new();
	rval->name	= bts_sprintf("%s-%d", duns[dunno].name, levno);
	rval->path	= mkJsonPath("");
	rval->title	= bts_strcpy(level->name);

	rval->poisonDmg = dun_poisonDmg[level->levFlags & 7];

	param_add(rval->params, PARAM_NUMBER, "spptDrainLo",
			level->levFlags & 7);
	param_add(rval->params, PARAM_NUMBER, "spptDrainHi", 
			(level->levFlags & 7) + 3);
	param_add(rval->params, PARAM_NUMBER, "hpDrain", level->levFlags & 7);

	for (y = level->height - 1; y >= 0; y--) {
		for (x = 0; x < level->width; x++) {
			setVertex(rval, level, x, y);
		}
	}

	addTraps(rval, level);
	addItems(rval->items, level->levFlags & 7);
	addMonsters(rval->monsters, level->monIndex);

	freeLevel(level);

	return rval;
}

/********************************/
/*				*/
/* Public Interface		*/
/*				*/
/********************************/

/*
 * addItems()
 */
void addItems(cnvList_t *list, uint8_t level)
{
	uint8_t		mask;
	uint32_t	i;
	uint32_t	itemNo;

	mask = 0x80 >> level;

	for (i = 0; i < 255; i++) {
		itemNo = i;
		if ((itemNo >= 224) && (itemNo < 240))
			itemNo = 195;

		if (dun_items[itemNo] & mask) {
			cnvList_add(list, getItemMacro(itemNo));
		}
	}
}

/*
 * addMonsters()
 */
void addMonsters(cnvList_t *list, uint8_t monsterLevel)
{
	uint16_t	monLo, monHi;
	uint32_t	i;

	monLo	= dun_monIndexList[monsterLevel].mon_lo;
	monHi	= dun_monIndexList[monsterLevel].mon_hi;

	for (i = monLo; i <= monHi; i++) {
		cnvList_add(list, getMonsterMacro(i));
	}
}

/*
 * convertDungeons()
 */
void convertDungeons(void)
{
	uint32_t	dunno, levno;
	cnvList_t	*dungeons;
	dun_t		*dun;
	dunLevel_t	*dl;

	dungeons	= dunList_new();

	dunno = 0;
	while (duns[dunno].name != NULL) {
		levno	= 0;

		dun = dun_new(bts_strcpy(duns[dunno].name));
		while (duns[dunno].levels[levno] != 0xff) {
			dl = convertLevel(dunno, levno);
			cnvList_add(dun->levels, dl);
			levno++;
		}
		dunno++;

		cnvList_add(dungeons, dun);
	}

	dunList_to_json(dungeons, mkJsonPath("dungeons.json"));
	cnvList_free(dungeons);
}

