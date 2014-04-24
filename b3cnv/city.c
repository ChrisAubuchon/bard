#include <b3lib.h>
#include <dun.h>
#include <cnv_dun.h>
#include <cnv_city.h>

#include <gl_list.h>
#include <gl_linkedhash_list.h>

#define DEBUG
#include <debug.h>


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

static dunLevel_t	*convertLevel(uint8_t);
static b3level_t	*b3_readLevel(btstring_t *);
static void		freeLevel(b3level_t *);

static btcity_t		*convertCity(uint32_t);
static b3city_t		*readCity(btstring_t *);
static void		freeCity(b3city_t *);
static uint32_t		isPath(uint8_t);
static uint32_t		countPaths(b3city_t *, int32_t, int32_t);
static uint8_t		wrapNumber(int32_t, uint32_t);
static void		addCitySquares(btcity_t *, b3city_t *);
static void		addWildSquares(btcity_t *, b3city_t *);
static uint32_t		sqInHash(gl_list_t, uint32_t);

static btstring_t	*toLabel(int32_t, int32_t, b3city_t *);
static btstring_t	*toFace(uint8_t);

static uint32_t		building_getHash(const void *);
static bool		building_compare(const void *, const void *);
static void		building_free(const void *);

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

static uint32_t building_getHash(const void *data)
{
	return (uint32_t)data % 255;
}

static bool building_compare(const void *data, const void *cmp)
{
	return ((uint32_t)data == (uint32_t)cmp);
}

static void building_free(const void *data)
{
}

/*
 * toLabel()
 */
static btstring_t *toLabel(int32_t x, int32_t y, b3city_t *inCity)
{
	if ((x < 0) || (y < 0) || (x >= inCity->width) || (y >= inCity->height))
		return bts_strcpy("entrance");
	else if (inCity->squares[y][x]) 
		return bts_sprintf("b%08x", inCity->squares[y][x]);
	else
		return bts_sprintf("%02d-%02d", x, y);
}

/*
 * toFace()
 */
static btstring_t *toFace(uint8_t sq)
{
	return bts_sprintf("%d", (sq & 0x0f) - 1);
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

/*
 * convertWild()
 */
static dunLevel_t *convertWild(b3city_t *w)
{
	dunLevel_t	*rval;
	int32_t		x, y;

	rval = dunLevel_new();

	rval->name = bts_strcpy(w->name);
	rval->path = mkJsonPath("");
#if 0
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
#endif


	return rval;
}

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

/********************************/
/*				*/
/* City Functions		*/
/*				*/
/********************************/

/*
 * convertCity()
 */
static btcity_t *convertCity(uint32_t cityIndex)
{
	btstring_t	*data;
	b3city_t	*c;
	btcity_t	*rval;

	data		= readData(cityList[cityIndex].levels[0]);
	c		= readCity(data);

	rval		= btcity_new(bts_strcpy(cityList[cityIndex].name));
	rval->path	= mkJsonPath("");
	rval->title	= bts_strcpy(c->name);

	/*
	 * Location string data for the '?' command
	 */
	if (cityIndex == 0) {
		param_add(rval->params, PARAM_BTSTRING, "locationString",
			bts_sprintf("The %s", c->name));
	} else {
		param_add(rval->params, PARAM_BTSTRING, "locationString",
			bts_strcpy(c->name));
	}
	param_add(rval->params, PARAM_STRING, "referenceString",
		city_referenceLocation[city_referenceIndex[cityIndex]]);

	if ((c->levFlags >> 6)  & 3) {
		param_add(rval->params, PARAM_STRING, "tileset", "skara");
		addCitySquares(rval, c);
	} else {
		param_add(rval->params, PARAM_STRING, "tileset", "wild");
		addWildSquares(rval, c);
	}

	freeCity(c);
	bts_free(data);

	return rval;
}

/*
 * addCitySquares()
 */
static void addCitySquares(btcity_t *outCity, b3city_t *inCity)
{
	gl_list_t	blhash;
	int32_t		x,y;
	uint32_t	sq;

	blhash		= gl_list_nx_create_empty(GL_LINKEDHASH_LIST,
				building_compare,
				building_getHash,
				building_free,
				0);

	for (y = inCity->height - 1; y > 0; y--) {
		for (x = 0; x < inCity->width; x++) {
			if (!countPaths(inCity, x, y))
				continue;

			sq 	= inCity->squares[y][x] & 0xff;


			if (sq) {
				if (sqInHash(blhash, sq))
					continue;

				citybldg_new(outCity,
					toLabel(x, y, inCity),
					toFace(sq),
					NULL,
					NULL
					);
			} else {
				citypath_new(outCity,
					toLabel(x, y, inCity),
					toLabel(x, y + 1, inCity),
					toLabel(x, y - 1, inCity),
					toLabel(x + 1, y, inCity),
					toLabel(x - 1, y, inCity),
					NULL,
					NULL,
					NULL
				);
			}
		}
	}

	gl_list_free(blhash);
}

/*
 * addWildSquares()
 */
static void addWildSquares(btcity_t *outCity, b3city_t *inCity)
{
	int32_t		x,y;
	uint8_t		sq;

	int8_t		northY, southY;
	int8_t		eastX, westX;

	for (y = inCity->height - 1; y > 0; y--) {
		for (x = 0; x < inCity->width; x++) {
			sq 	= inCity->squares[y][x];

			northY	= wrapNumber(y + 1, inCity->height);
			southY	= wrapNumber(y - 1, inCity->height);
			eastX	= wrapNumber(x + 1, inCity->width);
			westX	= wrapNumber(x - 1, inCity->width);

			citypath_new(outCity,
				toLabel(x, y, inCity),
				toLabel(x, northY, inCity),
				toLabel(x, southY, inCity),
				toLabel(eastX, y,  inCity),
				toLabel(westX, y,  inCity),
				NULL,
				NULL,
				sq ? toFace(sq) : NULL
			);
		}
	}
}

/*
 * sqInHash()
 */
static uint32_t sqInHash(gl_list_t blhash, uint32_t sq)
{
	if (gl_list_search(blhash, (void *)sq))
		return 1;

	if (gl_list_nx_add_last(blhash, (void *)sq) == NULL) {
		bt_error("Out of memory: %d\n", gl_list_size(blhash));
	}

	return 0;
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
 * isPath()
 */
static uint32_t isPath(uint8_t sq)
{
	if ((sq == 0) || ((sq & 0xf0) == 0xf0))
		return 1;

	return 0;
}

/*
 * countPaths()
 */
static uint32_t countPaths(b3city_t *inCity, int32_t x, int32_t y)
{
	uint32_t	paths = 0;
	int32_t		deltaX, deltaY;
	uint32_t	doWrap;

	if (inCity->wrapFlag & 2)
		doWrap = 1;

	/* north */
	deltaY = y + 1;
	if ((deltaY >= inCity->height) && doWrap) 
		deltaY = wrapNumber(deltaY, inCity->height);

	if (deltaY < inCity->height) {
		if (isPath(inCity->squares[deltaY][x]))
			paths++;
	}

	/* south */
	deltaY = y - 1;
	if ((deltaY < 0) && doWrap) 
		deltaY = wrapNumber(deltaY, inCity->height);

	if (deltaY >= 0) {
		if (isPath(inCity->squares[deltaY][x]))
			paths++;
	}

	/* east */
	deltaX = x + 1;
	if ((deltaX >= inCity->width) && doWrap) 
		deltaX = wrapNumber(deltaX, inCity->width);

	if (deltaX < inCity->width) {
		if (isPath(inCity->squares[y][deltaX]))
			paths++;

	}
	/* west */
	deltaX = x - 1;
	if ((deltaX < 0) && doWrap) 
		deltaX = wrapNumber(deltaY, inCity->width);

	if (deltaX >= 0) {
		if (isPath(inCity->squares[y][deltaX]))
			paths++;
	}

	return paths;
}

/*
 * readCity()
 */
static b3city_t *readCity(btstring_t *data)
{
	uint32_t	i, j;
	uint8_t		*buf;
	b3city_t	*rval;

	buf = data->buf;
	rval = xzalloc(sizeof(b3city_t));

	for (i = 0; i < 16; i++) {
		rval->name[i] = *buf++ & 0x7f;
		if (rval->name[i] == 0x7f)
			rval->name[i] = '\0';
	}

	rval->width		= *buf++;
	rval->height		= *buf++;
	rval->wrapFlag		= *buf++;
	rval->monsterIndex	= *buf++;
	rval->levFlags		= *buf++;
	rval->dataOffset	= str_read16le(buf);
	buf			+= sizeof(uint16_t);

	rval->rowOffset		= (uint16_t *)xzalloc(
					rval->height * sizeof(uint16_t)
				);
	rval->squares		= (uint8_t **)xzalloc(
					rval->height * sizeof(uint8_t *)
				);

	for (i = 0; i < rval->height; i++) {
		rval->rowOffset[i]	= str_read16le(buf);
		buf			+= sizeof(uint16_t);
		rval->squares[i]	= (uint8_t *)xzalloc(
						rval->width * sizeof(uint8_t)
					);
	}

	for (i = 0; i < rval->height; i++) {
		for (j = 0; j < rval->width; j++) {
			rval->squares[i][j] = data->buf[rval->rowOffset[i] + j];
			buf++;
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
	debug("rval->name = %s\n", rval->name);
	debug("rval->width = %d\n", rval->width);
	debug("rval->height = %d\n", rval->height);
	debug("rval->graphicsIndex = %d\n", (rval->levFlags >> 6) & 3);
	debug("rval->rvalelNo = %d\n", rval->levFlags & 7);
	debug("rval->wrapFlag = %d\n", rval->wrapFlag & 2);
	debug("rval->monsterIndex = %d\n", rval->monsterIndex);
	debug("rval->dataOffset = %x\n", rval->dataOffset);
#endif

	return rval;
}

/*
 * freeCity()
 */
static void freeCity(b3city_t *c)
{
	uint32_t	i;

	for (i = 0; i < c->height; i++)
		free(c->squares[i]);

	free(c->squares);
	free(c->rowOffset);
	free(c);
}

/********************************/
/*				*/
/* Public Interface		*/
/*				*/
/********************************/

void convertCities(void)
{
	cnvList_t	*cities;
	uint32_t	i;

	cities = cityList_new();

	i = 0;
	while (cityList[i].name != NULL) {
		cnvList_add(cities, convertCity(i));	
		i++;
	}

	cityList_to_json(cities, mkJsonPath("cities.json"));

	cnvList_free(cities);
}
