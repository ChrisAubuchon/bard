#include <b2lib.h>
#include <cnv_city.h>
#include <dehuf.h>
#include <level.h>
#include <gl_list.h>
#include <gl_linkedhash_list.h>
#include <mon.h>

#define DEBUG
#include <debug.h>

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#define CITY_LEVELINDEX	0
#define LOC_OFFSET	256

/********************************/
/*				*/
/* Internal prototypes		*/
/*				*/
/********************************/

static btcity_t		*convertCity(btstring_t *data, uint32_t index);

static void		convertPath(btstring_t *data, int32_t x, int32_t y);
static void		move(btstring_t *data, int32_t *x, int32_t *y);
static uint16_t 	xy2index(int32_t x, int32_t y);
static uint8_t		isPath(uint8_t sq);

static uint32_t		getBuildingLabel(int32_t x, int32_t y,  uint8_t sq);
static btstring_t	*toSquare(int32_t x, int32_t y);
static btstring_t	*toLabel(int32_t x, int32_t y, btstring_t *data,
				btstring_t *path);
static btstring_t	*getFace(uint8_t hi, uint8_t lo);
static btstring_t	*getStreet(btstring_t *data, uint32_t index);
static btstring_t	*getEvent(int32_t x, int32_t y, uint8_t sq,
				uint32_t cityIndex);
static btstring_t	*getTavernName(int32_t x, int32_t y);
static btstring_t	*getTempleName(int32_t x, int32_t y);

static uint32_t		building_getHash(const void *data);
static bool		building_compare(const void *data, const void *cmp);
static void		building_free(const void *data);

static void		addMonsters(cnvList_t *cl);
static void		addItems(cnvList_t *cl);

/********************************/
/*				*/
/* Internal functions		*/
/*				*/
/********************************/


/********************************/
/*				*/
/* Building list functions	*/
/*				*/
/********************************/

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


/********************************/
/*				*/
/* Path functions		*/
/*				*/
/********************************/

static uint16_t xy2index(int32_t x, int32_t y)
{
	return (y * 16) + x;
}

static uint8_t isPath(uint8_t sq)
{
	return (!(sq & 0x0f));
}

static uint32_t countPaths(btstring_t *data, int32_t x, int32_t y)
{
	uint32_t	rval = 0;

	if (x > 0) {
		if (isPath(data->buf[xy2index(x - 1, y)])) {
			rval++;
		}
	}

	if (y > 0) {
		if (isPath(data->buf[xy2index(x, y - 1)])) {
			rval++;
		}
	}

	if (y < 15) {
		if (isPath(data->buf[xy2index(x, y + 1)])) {
			rval++;
		}
	}

	if (x < 15) {
		if (isPath(data->buf[xy2index(x + 1, y)])) {
			rval++;
		}
	}

	return rval;
}

static void move(btstring_t *data, int32_t *x, int32_t *y)
{
	if (*x > 0) {
		if (isPath(data->buf[xy2index(*x - 1, *y)])) {
			*x = *x - 1;
			return;
		}
	}

	if (*y > 0) {
		if (isPath(data->buf[xy2index(*x, *y - 1)])) {
			*y = *y - 1;
			return;
		}
	}

	if (*x < 15) {
		if (isPath(data->buf[xy2index(*x + 1, *y)])) {
			*x = *x + 1;
			return;
		}
	}

	if (*y < 15) {
		if (isPath(data->buf[xy2index(*x, *y + 1)])) {
			*y = *y + 1;
			return;
		}
	}
}

static void convertPath(btstring_t *data, int32_t x, int32_t y)
{
	uint32_t		index;
	uint32_t		i;
	int32_t			saveX, saveY;
	uint32_t		numPaths;

	index	= xy2index(x, y);
	data->buf[index] = 0xff;

	while (countPaths(data, x, y) == 1) {
		move(data, &x, &y);
		data->buf[xy2index(x, y)] = 0xff;
	}

	if (!countPaths(data, x, y)) {
		return;
	}

	saveX	= x;
	saveY	= y;
	numPaths = countPaths(data, x, y);
	for (i = 0; i < numPaths; i++) {
		move(data, &x, &y);
		convertPath(data, x, y);
		x = saveX;
		y = saveY;
	}
}


/********************************/
/*				*/
/* Label functions		*/
/*				*/
/********************************/


static uint32_t getBuildingLabel(int32_t x, int32_t y, uint8_t sq)
{
	uint8_t		event;
	uint32_t	coords = 0;

	coords = (x << 8) | y;

	if (sq & 0xf0) {
		event = sq >> 4;
		switch (event) {
		case 1:		/* Tavern */
		case 2:		/* Temple */
		case 6:
			return coords | (sq << 16);
		default:
			return sq;
		}
	} else {
		return sq;
	}
}

static btstring_t *toSquare(int32_t x, int32_t y)
{
	return bts_sprintf("%02d-%02d", x, y);
}

static btstring_t *toLabel(int32_t x, int32_t y, btstring_t *data, btstring_t *path)
{
	uint32_t		index;

	index = ((15 - y) * 16) + x;
	if ((x < 0) || (y < 0) || (x > 15) || (y > 15)) {
		return bts_strcpy("entrance");
	} else if (path->buf[index] == 0xff) {
		return toSquare(x, y);
	} else {
		return bts_sprintf("b%08x", 
			getBuildingLabel(x, y, data->buf[index]));
	}
}

static btstring_t *getFace(uint8_t hi, uint8_t lo)
{
	if (citySpecialFace[hi] == NULL)
		return bts_strcpy(bldgFace[(lo > 4) ? (lo & 3) + 1 : lo]);
	else
		return bts_strcpy(citySpecialFace[hi]);
}

static btstring_t *getStreet(btstring_t *data, uint32_t index)
{
	uint8_t		sq;

	sq = data->buf[index + LOC_OFFSET];

	if (sq > 0x2b) {
		return bts_strcpy("plaza");
	} else {
		return bts_strcpy(cityStreetName[sq]);
	}

	return NULL;
}

static btstring_t *getTavernName(int32_t x, int32_t y)
{
	uint32_t	i = 0;

	while (cityTavernY[i] >= 0) {
		if ((cityTavernY[i] == (15-y)) && (cityTavernX[i] == x)) {
			return bts_sprintf("%s(\"%s\")",
				cityEvent[1],
				cityTavernName[i]
				);
		}
		i++;
	}

	return bts_sprintf("xxx-tavern: x: %d, y: %d", x, 15-y);
}

static btstring_t *getTempleName(int32_t x, int32_t y)
{
	uint32_t	i = 0;

	while (cityTempleY[i] >= 0) {
		if ((cityTempleY[i] == (15-y)) && (cityTempleX[i] == x)) {
			return bts_sprintf("%s(\"%s\")",
				cityEvent[2],
				cityTempleName[i]
				);
		}
		i++;
	}

	return bts_sprintf("xxx->temple: x: %2d, y: %2d", x, 15-y);
}

static btstring_t *getEvent(int32_t x, int32_t y, uint8_t sq, 
				uint32_t cityIndex)
{
	uint8_t		event;

	if (sq == 0) {
		return NULL;
	}

	event = sq >> 4;

	switch (event) {
	case 1:
		return getTavernName(x, y);
	case 2:
		return getTempleName(x, y);
	case 0x0c:
	case 0x0e:
	case 0x0f:
		return bts_strcpy(cityDungeon[cityIndex]);
	default:
		return bts_strcpy(cityEvent[event]);
	}
	return NULL;
}

/********************************/
/*				*/
/* convertCity function		*/
/*				*/
/********************************/

static btcity_t *convertCity(btstring_t *data, uint32_t cityIndex)
{
	gl_list_t		blhash;
	uint32_t		index;
	uint32_t		i;
	uint32_t		tag;
	uint8_t			lo, hi;
	int32_t			x, y;
	btcity_t		*c;
	btstring_t		*pathData;
	btstring_t		*event;

	c		= btcity_new(bts_strcpy(locationTitleList[cityIndex]));
	blhash		= gl_list_nx_create_empty(GL_LINKEDHASH_LIST,
				building_compare,
				building_getHash,
				building_free,
				0);

	pathData = bts_ncopy(data, 0x100, 0);
	x = cityStartX[cityIndex];
	y = cityStartY[cityIndex];
	convertPath(pathData, x, y);

	c->guildExitSquare = toSquare(guildExitSqE[cityIndex],
					15 - guildExitSqN[cityIndex]);
	c->guildExitDir = bts_strcpy(guildExitDir[cityIndex]);
	c->title = bts_strcpy(locationTitleList[cityIndex]);
	c->path = mkJsonPath("");

	citypath_new(c,
		bts_strcpy("entrance"),
		NULL,
		NULL,
		NULL,
		NULL,
		bts_sprintf("currentLevel:toCity(\"Wild\", %d, %d)", 
			cityToWildX[cityIndex], 47 - cityToWildY[cityIndex]
				),
		NULL,
		NULL
	);
		

	index = 0;
	for (y = 15; y >= 0; y--) {
		for (x = 0; x < 16; x++) {
			if (pathData->buf[index] == 0xff) {
				/* Path */
				citypath_new(c,
					toLabel(x, y, data, pathData),
					toLabel(x, y + 1,data, pathData),
					toLabel(x, y - 1,data, pathData),
					toLabel(x + 1, y,data, pathData),
					toLabel(x - 1, y,data, pathData),
					getEvent(x, y, data->buf[index], 
						cityIndex),
					getStreet(data, index),
					NULL
					);
			} else if (pathData->buf[index]) {
				/* Building */
				tag = getBuildingLabel(x, y, data->buf[index]);
				if (gl_list_search(blhash, (void *)tag) != NULL) {
					index++;
					continue;
				}

				if (gl_list_nx_add_last(blhash, (void *)tag) == NULL) {
					bt_error("Out of memory: %d", gl_list_size(blhash));
				}

				lo = data->buf[index] & 0x0f;
				hi = data->buf[index] >> 4;

				event = getEvent(x, y, data->buf[index], 
							cityIndex);

				citybldg_new(c,
					toLabel(x, y, data, pathData),
					getFace(0, lo),
					getFace(hi, lo),
					event
					);
					
			}
			index++;
		}
	}

	addMonsters(c->day->monsters);
	addItems(c->day->items);

	return c;
}

static void addMonsters(cnvList_t *cl)
{
	btstring_t	*data;

	data = getWildMonsters();
	getMonsterNameList(cl, data, M_WILD);
	bts_free(data);
}

static void addItems(cnvList_t *cl)
{
	uint32_t	i;

	for (i = 0; i < rewardItemRange[CITY_LEVELINDEX]; i++) {
		cnvList_add(cl, getItemName(rewardItemBase[CITY_LEVELINDEX]+i));
	}
}

/********************************/
/*				*/
/* Conversion function		*/
/*				*/
/********************************/

void convertCities(void)
{
	huffile_t	*huf;
	FILE		*fp;
	uint32_t	i;
	btstring_t	*data;
	btstring_t	*monsters;
	cnvList_t	*cities;

	/* Each city uses the same monster list as the wilds */
	monsters = getWildMonsters();

	fp	= xfopen(mkBardTwoPath("CITYS"), "rb");
	cities	= cityList_new();

	for (i = 1; i < 7; i++) {
		fp_moveToIndex32(fp, i, 0);
		huf	= dehuf_init(fp);
		data	= dehuf(huf, 0x200);

		cnvList_add(cities, convertCity(data, i));

		dehuf_free(huf);
		bts_free(data);
	}

	cityList_to_json(cities, mkJsonPath("cities.json"));

	cnvList_free(cities);
	fclose(fp);
}
