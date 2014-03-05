#include <b3lib.h>
#include <dun.h>
#include <cnv_dun.h>

/*#define DEBUG*/
#include <debug.h>

/****************************************/
/*					*/
/* Global variables			*/
/*					*/
/****************************************/

static int hifd = -1;
static int lofd = -1;

static bt_array_t *codeOffsets;
static uint32_t currentDungeon;
static uint32_t currentLevel;
static dun_t *dungeon;

/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/

btstring_t *dun_readData(uint8_t dun, uint8_t lev);

static b3level_t *b3_readLevel(btstring_t * dbuf);
static b3wild_t *b3_readWild(btstring_t * dbuf);
static dunLevel_t *b3_convertLevel(b3level_t * b);
static dunLevel_t *b3_convertWild(b3wild_t * w);
static dun_t *readDungeon(uint32_t dunno);

static void freeLevel(b3level_t * lev);
static void freeWild(b3wild_t * wild);

static void getWalls(dunWall_t * dw, uint16_t walls, uint8_t shift);
static void getWildWall(dunWall_t * dw, uint8_t wall);

static uint8_t countItems(uint8_t level);
static bt_array_t *getItems(uint8_t level, uint32_t * total_weight);
static bt_array_t *getMonsters(uint16_t monlo, uint16_t monhi);
static uint8_t wild_wrapSquare(int32_t sq, uint8_t width);

/****************************************/
/*					*/
/* Local functions			*/
/*					*/
/****************************************/

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

static dunLevel_t *b3_convertWild(b3wild_t * w)
{
	dunLevel_t *rval;
	int32_t i, j;
	uint32_t sq;

	rval = dunLevel_new(w->width, w->height);

	rval->name = bts_strcpy(w->name);
	rval->tag = bts_strcpy("level_0");

	rval->width = w->width;
	rval->height = w->height;
	rval->wrapFlag = (w->wrapFlag & 2) ? 1 : 0;

	rval->monsters = getMonsters(dun_monIndexList[w->monsterIndex].mon_lo, dun_monIndexList[w->monsterIndex].mon_hi);

	rval->poisonDmg = dun_poisonDmg[w->levFlags & 7];

	rval->items = getItems(w->levFlags & 7, &rval->item_total_weight);

	sq = 0;
	for (i = 0; i < w->height; i++) {
		for (j = 0; j < w->width; j++) {
			rval->squares[sq] = dunSquare_new();
			rval->squares[sq]->x = j;
			rval->squares[sq]->y = i;

			if (w->squares[i][j] == 0) {
				getWildWall(&rval->squares[sq]->n_wall, w->squares[wild_wrapSquare(i + 1, w->height)][j]);
				getWildWall(&rval->squares[sq]->s_wall, w->squares[wild_wrapSquare(i - 1, w->height)][j]);
				getWildWall(&rval->squares[sq]->e_wall, w->squares[i][wild_wrapSquare(j + 1, w->width)]);
				getWildWall(&rval->squares[sq]->w_wall, w->squares[i][wild_wrapSquare(j - 1, w->width)]);
			} else {
				rval->squares[sq]->n_wall.style = 0xff;
				rval->squares[sq]->s_wall.style = 0xff;
				rval->squares[sq]->w_wall.style = 0xff;
				rval->squares[sq]->e_wall.style = 0xff;
			}

			sq++;
		}
	}


	return rval;
}

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

btstring_t *dun_readData(uint8_t dun, uint8_t lev)
{
	btstring_t *rval;
	btstring_t *dat;
	int fd;

	if (hifd < 0)
		hifd = xopen("maps.hi", O_RDONLY);
	if (lofd < 0)
		lofd = xopen("maps.lo", O_RDONLY);

	fd = (duns[dun].files[lev].dun) ? hifd : lofd;

	moveToIndex16(fd, duns[dun].files[lev].lev, 0);
	dat = bts_new(19712);
	xread(fd, dat->buf, dat->size);
	rval = d3comp(dat->buf, dat->size);

	bts_free(dat);

	return rval;
}

static b3level_t *b3_readLevel(btstring_t * dbuf)
{
	b3level_t *r;
	uint8_t *buf;
	int8 i, j;

	buf = dbuf->buf;
	r = (b3level_t *) xmalloc(sizeof(b3level_t));

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

	r->rowOffset = (uint16_t *) xmalloc(r->height * sizeof(uint16_t));
	r->squares = (b3dunSq_t **) xmalloc(r->height * sizeof(b3dunSq_t *));
	for (i = 0; i < r->height; i++) {
		r->rowOffset[i] = str_read16le(buf);
		buf += sizeof(uint16_t);
		r->squares[i] = (b3dunSq_t *) xmalloc(r->width * sizeof(b3dunSq_t));
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

static b3wild_t *b3_readWild(btstring_t * dbuf)
{
	uint32_t i, j;
	b3wild_t *rval;
	uint8_t *buf;

	buf = dbuf->buf;

	rval = (b3wild_t *) xmalloc(sizeof(b3wild_t));

	for (i = 0; i < 16; i++) {
		rval->name[i] = *buf++ & 0x7f;
		if (rval->name[i] == 0x7f)
			rval->name[i] = '\0';
	}

	rval->width = *buf++;
	rval->height = *buf++;
	rval->wrapFlag = *buf++;
	rval->monsterIndex = *buf++;
	rval->levFlags = *buf++;
	rval->dataOffset = str_read16le(buf);
	buf += sizeof(uint16_t);

	rval->rowOffset = (uint16_t *) xmalloc(rval->height * sizeof(uint16_t));
	rval->squares = (uint8_t **) xmalloc(rval->height * sizeof(uint8_t *));
	for (i = 0; i < rval->height; i++) {
		rval->rowOffset[i] = str_read16le(buf);
		buf += sizeof(uint16_t);
		rval->squares[i] = (uint8_t *) xmalloc(rval->width * sizeof(uint8_t));
	}

	for (i = 0; i < rval->height; i++) {
		for (j = 0; j < rval->width; j++) {
			rval->squares[i][j] = dbuf->buf[rval->rowOffset[i] + j];
			buf++;
		}
	}

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
		
	debug("rval->name = %s\n", rval->name);
	debug("rval->width = %d\n", rval->width);
	debug("rval->height = %d\n", rval->height);
	debug("rval->graphicsIndex = %d\n", (rval->levFlags >> 6) & 3);
	debug("rval->rvalelNo = %d\n", rval->levFlags & 7);
	debug("rval->wrapFlag = %d\n", rval->wrapFlag & 2);
	debug("rval->monsterIndex = %d\n", rval->monsterIndex);
	debug("rval->dataOffset = %x\n", rval->dataOffset);

	return rval;
}

static void freeLevel(b3level_t * lev)
{
	int i;

	for (i = 0; i < lev->height; i++)
		free(lev->squares[i]);
	free(lev->squares);
	free(lev->rowOffset);
	free(lev->dataList);
	free(lev);
}

static void freeWild(b3wild_t * wild)
{
	uint32_t i;

	for (i = 0; i < wild->height; i++)
		free(wild->squares[i]);
	free(wild->squares);
	free(wild->rowOffset);
	free(wild->dataList);
	free(wild);
}

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
			b3wild_t *b3wild;

			b3wild = b3_readWild(dbuf);

			rval->levs[j] = b3_convertWild(b3wild);
			rval->gfx_tile = (b3wild->levFlags >> 6) ? bts_strcpy("tile_skara") : bts_strcpy("tile_wild");

			freeWild(b3wild);
		} else {
			b3level_t *b3lev;

			b3lev = b3_readLevel(dbuf);

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

/********************************/
/*				*/
/* Public Interface		*/
/*				*/
/********************************/

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

void convertDungeons(void)
{
	uint32_t i, j;

	btstring_t *dbuf;

	cnv_printDunHeader();

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

	cnv_printDunFooter();

	close(hifd);
	close(lofd);
}
