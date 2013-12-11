#include <b2lib.h>
#include <cnv_city.h>
#include <dehuf.h>
#include <mon.h>
#include <level.h>

#define DEBUG
#include <debug.h>


/********************************/
/*				*/
/* Internal macros		*/
/*				*/
/********************************/

#define INT_NONE	0
#define INT_NORTH	1
#define INT_SOUTH	2
#define INT_EAST	3
#define INT_WEST	4

#define WILD_LEVELINDEX	1

/********************************/
/*				*/
/* Internal function prototypes	*/
/*				*/
/********************************/

static void		addSquare(btcity_t *c, btstring_t *data,
				uint32_t x, uint32_t y, uint8_t sq);
static btstring_t	*toLabel(uint8_t dir, uint32_t x, uint32_t y, 
					btstring_t *data);
static btstring_t	*getFace(uint8_t sq);
static btstring_t	*getEvent(uint32_t x, uint32_t y, uint8_t sq);
static btstring_t	*enterCity(uint32_t y);
static void		addMonsters(cnvList_t *cl);
static void		addItems(cnvList_t *cl);

/********************************/
/*				*/
/* Internal functions		*/
/*				*/
/********************************/

static btstring_t *enterCity(uint32_t y)
{
	uint32_t	cityIndex = 0;

	switch (y) {
	case 23:
		cityIndex = 1;
		break;
	case 41:
		cityIndex = 2;
		break;
	case 14:
		cityIndex = 3;
		break;
	case 32:
		cityIndex = 4;
		break;
	case 5:
		cityIndex = 5;
		break;
	case 7:
		cityIndex = 6;
		break;
	default:
		printf("Unknown city y coordinate: %d\n", y);
		exit(1);
	}

	return bts_sprintf("currentLevel:toCity(\"%s\", %d, %d)",
		locationTitleList[cityIndex],
		wildToCityX[cityIndex], wildToCityY[cityIndex]
		);
}

static btstring_t *getEvent(uint32_t x, uint32_t y, uint8_t sq)
{
	switch (sq) {
	case 1:
		return bts_strcpy("cityBuildings:enter(\"emptyhut\")");
	case 2:
		return bts_strcpy("cityBuildings:enter(\"wall\")");
	case 3:
	case 4:
		return bts_strcpy("cityBuildings:enter(\"tree\")");
	case 5:
		return enterCity(y);
	case 0xbc:
		return bts_strcpy("cityBuildings:enter(\"sagehut\")");
	case 0xd1:
		return bts_strcpy("cityBuildings:enter(\"fanskar\")");
	case 0xf1:
	{
		if (y == 19) {
			return bts_strcpy("cityBuildings:enter(\"kazdek\")");
		} else if (y == 2) {
			return bts_strcpy("cityBuildings:enter(\"narn\")");
		} else if (y == 31) {
			return bts_strcpy("cityBuildings:enter(\"greycrypt\")");
		} 
	}
	case 0:
		return NULL;
	default:
		return bts_strcpy("xxx-unknown-label");
	}
}

static btstring_t *getFace(uint8_t sq)
{
	switch (sq & 0x0f) {
	case 1:
	case 12:
		return bts_strcpy("hut");
	case 3:
	case 4:
		return bts_strcpy("tree");
	case 2:
		return bts_strcpy("wall");
	case 5:
		return bts_strcpy("entr");
	default:
		return NULL;
	}

	return bts_strcpy("xxx-unknown-face");
}

static btstring_t *toLabel(uint8_t dir, uint32_t x, uint32_t y, 
				btstring_t *data)
{
	uint32_t	l_x = x;
	uint32_t	l_y = y;
	uint8_t		sq;

	switch (dir) {
	case INT_NORTH:
		l_y = (y == 47) ? 0 : l_y + 1;
		break;
	case INT_SOUTH:
		l_y = (y == 0) ? 47 : l_y - 1;
		break;
	case INT_EAST:
		l_x = (x == 31) ? 0 : l_x + 1;
		break;
	case INT_WEST:
		l_x = (x == 0) ? l_x = 31 : l_x - 1;
		break;
	case INT_NONE:
		break;
	}

	return bts_sprintf("x%02d-%02d", l_x, l_y);
}

static void addSquare(btcity_t *c, btstring_t *data, uint32_t x, uint32_t y,
		uint8_t sq)
{
	citypath_new(c,
		toLabel(INT_NONE,  x, y, data),
		toLabel(INT_NORTH, x, y, data),
		toLabel(INT_SOUTH, x, y, data),
		toLabel(INT_EAST,  x, y, data),
		toLabel(INT_WEST,  x, y, data),
		getEvent(x, y, sq),
		NULL,
		getFace(sq)
	);
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

	for (i = 0; i < rewardItemRange[WILD_LEVELINDEX]; i++) {
		cnvList_add(cl, getItemName(rewardItemBase[WILD_LEVELINDEX]+i));
	}
}

/********************************/
/*				*/
/* convertWild			*/
/*				*/
/********************************/

btstring_t *getWildMonsters(void)
{
	FILE		*fp;
	btstring_t	*data;
	huffile_t	*huf;

	fp = xfopen(mkBardTwoPath("CITYS"), "rb");
	fp_moveToIndex32(fp, 0, 0);
	huf = dehuf_init(fp);
	data = dehuf(huf, 0x600);
	bts_free(data);
	data = dehuf(huf, 0x300);
	dehuf_free(huf);
	fclose(fp);

	return data;
}

void convertWild(void)
{
	btcity_t	*c;
	huffile_t	*huf;
	FILE		*fp;
	btstring_t	*data;
	uint32_t	i, x;
	int32_t		y;
	uint32_t	index;

	fp = xfopen(mkBardTwoPath("CITYS"), "rb");

	fp_moveToIndex32(fp, 0, 0);
	huf = dehuf_init(fp);
	data = dehuf(huf, 0x600);
	fclose(fp);

	c = btcity_new(bts_strcpy("wild"));
	c->title = bts_strcpy(locationTitleList[0]);

	for (y = 0; y < 48; y++) {
		for (x = 0; x < 32; x++) {
			index = ((47 - y) * 32) + x;
			addSquare(c, data, x, y, data->buf[index]);
		}
	}

	c->day->poisonDmg	= poisonDamageList[WILD_LEVELINDEX];
	c->day->level		= WILD_LEVELINDEX + 1;
	addMonsters(c->day->monsters);
	addItems(c->day->items);

	btcity_to_json(mkJsonPath("wild.json"), c);

	dehuf_free(huf);
	btcity_free(c);
	bts_free(data);
}
