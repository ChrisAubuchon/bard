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

/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/

static btstring_t	*readData(uint8_t);

static dunLevel_t	*convertLevel(uint8_t);
static b3level_t	*b3_readLevel(btstring_t *);

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

	addItems(rval->day->items, c->levFlags & 7);
	addMonsters(rval->day->monsters, c->monsterIndex);

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
