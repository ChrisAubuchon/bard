#include <b3lib.h>
#include <dun.h>
#include <cnv_dun.h>
#include <cnv_city.h>

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

#if 0
static int hifd = -1;
static int lofd = -1;

static bt_array_t *codeOffsets;
static uint32_t currentDungeon;
static uint32_t currentLevel;
static dun_t *dungeon;

#endif

/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/

static btstring_t	*readData(uint8_t);
static btstring_t	*getLabel(int32_t, int32_t);

static dunLevel_t	*convertLevel(uint32_t, uint32_t);
static b3level_t	*readLevel(btstring_t *);
static void		freeLevel(b3level_t *);

static uint32_t		getVertex(int32_t, int32_t);
static void		setVertex(dunLevel_t *, b3level_t *, int32_t, int32_t);
static void		getPath(b3level_t *, int32_t, int32_t, uint32_t,
					dunPath_t *);
static uint8_t		wrapNumber(int32_t, uint32_t);

#if 0

static b3level_t *b3_readLevel(btstring_t * dbuf);
static b3city_t *b3_readWild(btstring_t * dbuf);
static dunLevel_t *b3_convertLevel(b3level_t * b);
static dun_t *readDungeon(uint32_t dunno);

static void getWalls(dunWall_t * dw, uint16_t walls, uint8_t shift);
static void getWildWall(dunWall_t * dw, uint8_t wall);

static uint8_t countItems(uint8_t level);
static bt_array_t *getItems(uint8_t level, uint32_t * total_weight);
static bt_array_t *getMonsters(uint16_t monlo, uint16_t monhi);
static uint8_t wild_wrapSquare(int32_t sq, uint8_t width);
#endif

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

#if 0
static uint8_t wild_wrapSquare(int32_t sq, uint8_t width)
{
	while (1) {
		if (sq < 0) {
			sq += width;
		} else if (sq >= width) {
			sq -= width;
		} else {
			return sq;
		}
	}
}

static void getWalls(dunWall_t * dw, uint16_t walls, uint8_t shift)
{
	uint8_t face;
	uint8_t style;

	face = (walls >> shift) & 0x0f;
	if (!face) {
		dw->style = 0xff;
		return;
	}

	style = dun_styleMap[face];
	dw->nopass = dun_wallFlag[face];

	if (style) {
		dw->canphase = (style < 9) ? 1 : 0;
		dw->style = style - 1;
	} else {
		dw->style = 0xff;
	}
}

static void getWildWall(dunWall_t * dw, uint8_t wall)
{
	if (wall == 0) {
		dw->style = 0xff;
		return;
	}

	dw->style = (wall & 0x0f) - 1;
	dw->nopass = (wall & 0xe0) == 0xe0 ? 1 : 0;
}

#endif

#if 0

static dunLevel_t *b3_convertLevel(b3level_t * b)
{
	dunLevel_t *rval;
	uint32_t i, j;
	uint32_t sq;

	rval = dunLevel_new(b->width, b->height);

	rval->name = bts_strcpy(b->name);
	rval->tag = bts_sprintf("level_%d", b->levelNum);
	rval->width = b->width;
	rval->height = b->height;
	rval->deltaSqN = b->deltaSqN;
	rval->deltaSqE = b->deltaSqE;
	rval->exitSqN = b->exitSqN;
	rval->exitSqE = b->exitSqE;

	rval->useDunBG = (b->levFlags & 0x20) ? 0 : 1;

	rval->monsters = getMonsters(dun_monIndexList[b->monIndex].mon_lo, dun_monIndexList[b->monIndex].mon_hi);

	rval->chest_traps = bt_array_new(4, bts_free);
	rval->floor_traps = bt_array_new(3, bts_free);

	for (i = 0; i < bt_array_length(rval->chest_traps); i++)
		bt_array_set(rval->chest_traps, i, getChestTrap(b->levFlags & 7, i));

	for (i = 0; i < bt_array_length(rval->floor_traps); i++)
		bt_array_set(rval->floor_traps, i, getDunTrap(b->levFlags & 7, i));

	rval->poisonDmg = dun_poisonDmg[b->levFlags & 7];
	rval->spptDrain_lo = b->levFlags & 7;
	rval->spptDrain_hi = (b->levFlags & 7) + 3;
	rval->hpDrain_lo = rval->hpDrain_hi = b->levFlags & 7;

	rval->items = getItems(b->levFlags & 7, &rval->item_total_weight);

	sq = 0;
	for (i = 0; i < b->height; i++) {
		for (j = 0; j < b->width; j++) {
			rval->squares[sq] = dunSquare_new();
			rval->squares[sq]->x = j;
			rval->squares[sq]->y = i;
#define CHECKFLAG(x, y) (b->squares[i][j].x & (y)) ? 1 : 0
			rval->squares[sq]->flags.stairs = CHECKFLAG(flags, SQ_STAIRS);
			rval->squares[sq]->flags.special = CHECKFLAG(flags, SQ_SPECIAL);
			rval->squares[sq]->flags.darkness = CHECKFLAG(flags, SQ_DARKNESS);
			rval->squares[sq]->flags.trap = CHECKFLAG(flags, SQ_TRAP);
			rval->squares[sq]->flags.portbelow = CHECKFLAG(flags, SQ_PORTBELOW);
			rval->squares[sq]->flags.portabove = CHECKFLAG(flags, SQ_PORTABOVE);
			rval->squares[sq]->flags.highbat = CHECKFLAG(flags, SQ_HIGHBATTLE);
			rval->squares[sq]->flags.spinner = CHECKFLAG(flags, SQ_SPINNER);
			rval->squares[sq]->flags.antimagic = CHECKFLAG(flags, SQ_ANTIMAGIC);
			rval->squares[sq]->flags.drainhp = CHECKFLAG(flags, SQ_DRAINHP);
			rval->squares[sq]->flags.odd = CHECKFLAG(flags, SQ_ODD);
			rval->squares[sq]->flags.silence = CHECKFLAG(flags, SQ_SILENCE);
			rval->squares[sq]->flags.regensppt = CHECKFLAG(flags, SQ_REGENSPPT);
			rval->squares[sq]->flags.drainsppt = CHECKFLAG(flags, SQ_DRAINSPPT);
			rval->squares[sq]->flags.monhost = CHECKFLAG(flags, SQ_MONHOSTILE);
			rval->squares[sq]->flags.explosion = CHECKFLAG(field_4, SQ_EXPLOSION);
			rval->squares[sq]->flags.hpregen = CHECKFLAG(field_4, SQ_UNKNOWN3);
			rval->squares[sq]->flags.stuck = CHECKFLAG(field_4, SQ_STUCK);
#undef CHECKFLAG

			getWalls(&rval->squares[sq]->n_wall, b->squares[i][j].walls, 4);
			getWalls(&rval->squares[sq]->s_wall, b->squares[i][j].walls, 12);
			getWalls(&rval->squares[sq]->e_wall, b->squares[i][j].walls, 0);
			getWalls(&rval->squares[sq]->w_wall, b->squares[i][j].walls, 8);
			sq++;
		}
	}

	return (rval);
}

#endif

/*
 * readLevel()
 */
static b3level_t *readLevel(btstring_t *data)
{
	uint8_t		*buf;
	int8_t		i, j;
	b3level_t	*r;

	buf = data->buf;
	r = (b3level_t *)xzalloc(sizeof(b3level_t));

	for (i = 0; i < 16; i++) {
		r->name[i] = *buf++ & 0x7f;
		if (r->name[i] == 0x7f)
			r->name[i] = '\0';
	}

	r->levFlags = *buf++;
	r->levelNum = *buf++;

	for (i = 0; i < 8; i++)
		r->dunLevel[i] = *buf++;

	r->exitSqN = *buf++;
	r->exitSqE = *buf++;
	r->exitLoc = *buf++;
	r->monIndex = *buf++;
	r->width = *buf++;
	r->height = *buf++;
	r->deltaSqE = *buf++;
	r->deltaSqN = *buf++;
	r->dataOffset = str_read16le(buf);
	buf += sizeof(uint16_t);

	r->rowOffset = (uint16_t *) xzalloc(r->height * sizeof(uint16_t));
	r->squares = (b3dunSq_t **) xzalloc(r->height * sizeof(b3dunSq_t *));
	for (i = 0; i < r->height; i++) {
		r->rowOffset[i] = str_read16le(buf);
		buf += sizeof(uint16_t);
		r->squares[i] = (b3dunSq_t *) xzalloc(r->width * sizeof(b3dunSq_t));
	}

	for (i = (r->height - 1); i >= 0; i--) {
		for (j = 0; j < r->width; j++) {
			r->squares[i][j].walls = str_read16le(buf);
			buf += sizeof(uint16_t);
			r->squares[i][j].flags = str_read16le(buf);
			buf += sizeof(uint16_t);
			r->squares[i][j].field_4 = *buf++;
		}
	}
	
#if 0
	codeOffsets = bt_array_new(*buf++, xfree);
	for (i = 0; i < bt_array_length(codeOffsets); i++) {
		b3data_t *new;

		new = (b3data_t *)xmalloc(sizeof(b3data_t));
		new->sqN = *buf++;
		new->sqE = *buf++;
		new->offset = str_read16le(buf);
		buf += sizeof(uint16_t);
		
		bt_array_set(codeOffsets, i, new);
	}
#endif
		
#if 0
	r->dataCount = *buf++;
	r->dataList = (b3data_t *) xmalloc(r->dataCount * sizeof(b3data_t));
	for (i = 0; i < r->dataCount; i++) {
		r->dataList[i].sqN = *buf++;
		r->dataList[i].sqE = *buf++;
		r->dataList[i].offset = str_read16le(buf);
		buf += sizeof(uint16_t);
	}
#endif

	return r;
}

/*
 * freeLevel()
 */
static void freeLevel(b3level_t *l)
{
	uint32_t	i;

	if (l == NULL)
		return;

	for (i = 0; i < l->height; i++)
		free(l->squares[i]);

	free(l->squares);
	free(l->rowOffset);
	free(l);
}

/*
 * b3_readWild()
 */

#if 0

static uint8_t countItems(uint8_t level)
{
	uint8_t mask;
	uint8_t i;
	uint8_t rval = 0;

	mask = 0x80 >> level;

	for (i = 0; i < 255; i++)
		if (dun_items[i] & mask)
			rval++;

	return rval;
}

static bt_array_t *getItems(uint8_t level, uint32_t * total_weight)
{
	bt_array_t *rval;
	uint8_t i, j = 0;
	uint8_t mask;
	uint8_t harmonic = 0;

	rval = bt_array_new(countItems(level), dunItem_free);

	mask = 0x80 >> level;

	for (i = 0; i < 255; i++) {
		if (dun_items[i] & mask) {
			dunItem_t *new;

			(i == 195) ? harmonic = 1 : 0;

			new = dunItem_new();
			new->name = getItemMacro(i);

			bt_array_set(rval, j, new);
			j++;
		}
	}

	j = 0;
	if (harmonic) {
		*total_weight = (bt_array_length(rval) - 1) << 8;

		for (i = 0; i < 255; i++) {
			if (dun_items[i] & mask) {
				dunItem_t *d;

				d = bt_array_get(rval, j);

				if (i == 195)
					d->weight = (bt_array_length(rval) - 1) * 17;
				else
					d->weight = 239;
				j++;
			}
		}
	} else {
		*total_weight = bt_array_length(rval);

		for (i = 0; i < bt_array_length(rval); i++) {
			dunItem_t *d;

			d = bt_array_get(rval, j);

			d->weight = 1;
		}
	}

	return rval;
}

static bt_array_t *getMonsters(uint16_t monlo, uint16_t monhi)
{
	bt_array_t *rval;
	uint16_t nmons;
	uint32_t i, j = 0;

	nmons = (monhi - monlo) + 1;
	rval = bt_array_new(nmons, bts_free);

	for (i = monlo; i <= monhi; i++)
		bt_array_set(rval, j++, getMonsterMacro(i));

	return rval;
}

static dun_t *readDungeon(uint32_t dunno)
{
	btstring_t *dbuf;
	dun_t *rval;
	uint8_t j;

	rval = dun_new(duns[dunno].nlevs);
	rval->name = bts_strcpy(duns[dunno].name);

	for (j = 0; j < rval->nlevs; j++) {
		dbuf = dun_readData(dunno, j);

		if (duns[dunno].wflag) {
			b3city_t *b3wild;

			b3wild = b3_readWild(dbuf);

			rval->levs[j] = b3_convertWild(b3wild);
			rval->gfx_tile = (b3wild->levFlags >> 6) ? bts_strcpy("tile_skara") : bts_strcpy("tile_wild");

			freeWild(b3wild);
		} else {
			b3level_t *b3lev;

			b3lev = readLevel(dbuf);

			if (!j) {
				rval->entry_type = dun_entryTypeList[b3lev->dunLevel[j]];
				rval->entrySqN = dun_entrySqN[b3lev->dunLevel[j]];
				rval->entrySqE = dun_entrySqE[b3lev->dunLevel[j]];
				rval->direction = (b3lev->levFlags & 0x10) ? 1 : 0;
				rval->gfx_tile = bts_strcpy("tile_gdung");
			}

			rval->levs[j] = b3_convertLevel(b3lev);

			freeLevel(b3lev);
		}

		rval->levs[j]->codeOffsets = codeOffsets;
		rval->levs[j]->codeBuffer = bts_ncopy(dbuf, dbuf->size - duns[dunno].files[j].code_offset, duns[dunno].files[j].code_offset);
		codeOffsets = NULL;

		bts_free(dbuf);
		dbuf = NULL;
	}

	return rval;
}
#endif

/*
 * readData()
 */
static btstring_t *readData(uint8_t levNumber)
{
	FILE		*fp;
	btstring_t	*data;
	btstring_t	*rval;

	if (levNumber & HIFLAG)
		fp = xfopen(mkBardThreePath("MAPS.HI"), "rb");
	else
		fp = xfopen(mkBardThreePath("MAPS.LO"), "rb");

	fp_moveToIndex16(fp, levNumber & ~HIFLAG, 0);
	data	= bts_new(19712);

	xfread(data->buf, 1, data->size, fp);
	fclose(fp);

	rval	= d3comp(data->buf, data->size);
	bts_free(data);


	return rval;
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
		deltaY		= -1;
		break;
	case DIR_SOUTH:
		shift		= 12;
		deltaY		= 1;
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
		path->isWall = dun_wallFlag[face];
		style = dun_styleMap[face];

		if (style) {
			path->canPhase = (style < 9) ? 1 : 0;
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
}

/*
 * convertLevel()
 */
static dunLevel_t *convertLevel(uint32_t dunno, uint32_t levno)
{
	dunLevel_t	*rval;
	btstring_t	*data;
	b3level_t	*level;
	int32_t		y, x;

	data	= readData(duns[dunno].levels[levno]);

	level	= readLevel(data);

	rval	= dunLevel_new();
	rval->name	= bts_sprintf("%s-%d", duns[dunno].name, levno);
	rval->path	= mkJsonPath("");

	for (y = level->height - 1; y > 0; y--) {
		for (x = 0; x < level->width; x++) {
			setVertex(rval, level, x, y);
		}
	}

	freeLevel(level);
	bts_free(data);

	return rval;
}

/********************************/
/*				*/
/* Public Interface		*/
/*				*/
/********************************/

#if 0
btstring_t *getLevelPath(uint8_t dun, uint8_t level)
{
	return bts_sprintf("%s/level_%d", duns[dun].name ,level);
}

btstring_t *getChangeLevelPath(uint8_t dun, uint8_t level, uint8_t direction)
{
	if (direction) {
		/* Up a dungeon level */
		return getLevelPath(dun, level + 1);
	} else {
		/* Down a dungeon level or exit */
		if (level) {
			return getLevelPath(dun, level - 1);
		} else {
			return getLevelPath(levno2dun[dungeon->levs[level]->exitLoc], levno2lev[dungeon->levs[level]->exitLoc]);
		}
	}
}

dunLevel_t *getCurrentLevel(void)
{
	return dungeon->levs[currentLevel];
}

uint8_t getCurrentLevelNum(void)
{
	return currentLevel;
}

#endif

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
#if 0
	currentDungeon = 3;
	currentDungeon = 0;
	while (duns[currentDungeon].name != 0) {
		printf("Current dungeon #: %d\n", currentDungeon);
		dungeon = readDungeon(currentDungeon);

		disasm_code(dungeon, currentDungeon);
		outputDungeonXML(2, dungeon); 
		currentDungeon++;

		dun_free(dungeon);
	}

	close(hifd);
	close(lofd);
#endif

	dunList_to_json(dungeons, mkJsonPath("dungeons.json"));
	cnvList_free(dungeons);
}

