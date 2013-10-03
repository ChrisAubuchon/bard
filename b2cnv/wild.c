#include <b2lib.h>
#include <cnv_city.h>
#include <dehuf.h>
#include <gl_list.h>
#include <gl_linkedhash_list.h>
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

static void		addEmptySpace(btcity_t *c, btstring_t *data, 
					uint32_t x, uint32_t y);
static void		addBuilding(btcity_t *c, gl_list_t oh, 
					btstring_t *data, uint32_t x, 
					uint32_t y, uint8_t sq);
static btstring_t	*toLabel(uint8_t dir, uint32_t x, uint32_t y, 
					btstring_t *data);
static btstring_t	*getBuildingLabel(uint32_t x, uint32_t y, uint8_t sq);
static btstring_t	*getFace(uint8_t sq);
static btstring_t	*getEvent(uint32_t x, uint32_t y, uint8_t sq);
static void		addMonsters(cnvList_t *cl);
static void		addItems(cnvList_t *cl);

/* Hash functions */
static uint32_t		objectGetHash(const void *vo);
static bool		objectCompare(const void *vo, const void *cmp);
static void		objectFree(const void *vo);

/********************************/
/*				*/
/* Internal functions		*/
/*				*/
/********************************/

static uint32_t objectGetHash(const void *vo)
{
	return (uint32_t)vo % 255;
}

static bool objectCompare(const void *vo, const void *cmp)
{
	if ((uint32_t)vo == (uint32_t)cmp)
		return 1;

	return 0;
}

static void objectFree(const void *vo)
{
}

static btstring_t *getBuildingLabel(uint32_t x, uint32_t y, uint8_t sq)
{
	switch (sq) {
	case 1:
		return bts_strcpy("hut");
	case 2:
		return bts_strcpy("citywall");
	case 3:
	case 4:
		return bts_strcpy("tree");
	case 5:
		return bts_strcpy("cityentrance");
	case 0xbc:
		return bts_strcpy("sagehut");
	case 0xd1:
		return bts_strcpy("fanskar");
	case 0xf1:
	{
		if (y == 28) {
			return bts_strcpy("kazdek");
		} else if (y == 45) {
			return bts_strcpy("narn");
		} else if (y == 16) {
			return bts_strcpy("greycrypt");
		}
	}
	default:
		return bts_strcpy("xxx-unknown-label");
	}
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
		return bts_strcpy("cityBuildings:enter(\"entercity\")");
	case 0xbc:
		return bts_strcpy("cityBuildings:enter(\"sagehut\")");
	case 0xd1:
		return bts_strcpy("cityBuildings:enter(\"fanskar\")");
	case 0xf1:
	{
		if (y == 28) {
			return bts_strcpy("cityBuildings:enter(\"kazdek\")");
		} else if (y == 45) {
			return bts_strcpy("cityBuildings:enter(\"narn\")");
		} else if (y == 16) {
			return bts_strcpy("cityBuildings:enter(\"greycrypt\")");
		}
	}
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
		l_y = (y == 0) ? 47 : l_y - 1;
		break;
	case INT_SOUTH:
		l_y = (y == 47) ? 0 : l_y + 1;
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

	sq = data->buf[(l_y * 32) + l_x];
	if (sq == 0) {
		return bts_sprintf("x%02d-%02d", l_x, l_y);
	} else {
		return getBuildingLabel(l_x, l_y, sq);
	}
}

static void addEmptySpace(btcity_t *c, btstring_t *data, uint32_t x, uint32_t y)
{
	citypath_new(c,
		toLabel(INT_NONE,  x, y, data),
		toLabel(INT_NORTH, x, y, data),
		toLabel(INT_SOUTH, x, y, data),
		toLabel(INT_EAST,  x, y, data),
		toLabel(INT_WEST,  x, y, data),
		NULL,
		NULL
		);
}

static void addBuilding(btcity_t *c, gl_list_t oh, btstring_t *data, 
				uint32_t x, uint32_t y, uint8_t sq)
{
	uint32_t	tag = sq;

	/* 3 and 4 both are trees. Use 3 as the tag */
	if (tag == 4)
		tag = 3;

	/* 
	 * tags & 0xf0 are unique buildings. No need to add them
	 * to the hash.
	 */
	if (!(tag & 0xf0)) {
		if (gl_list_search(oh, (void *)tag) != NULL) 
			return;

		if (gl_list_nx_add_last(oh, (void *)tag) == NULL) {
			bt_error("Out of memory: %d", gl_list_size(oh));
		}
	}

	/* xxx - add building event */
	citybldg_new(c, toLabel(INT_NONE, x, y, data),
			getFace(sq), NULL, getEvent(x, y, sq));
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
	gl_list_t	objectHash;

	fp = xfopen(mkBardTwoPath("CITYS"), "rb");

	fp_moveToIndex32(fp, 0, 0);
	huf = dehuf_init(fp);
	data = dehuf(huf, 0x600);
	fclose(fp);

	c = btcity_new(bts_strcpy("wild"));
	objectHash = gl_list_nx_create_empty(GL_LINKEDHASH_LIST,
		objectCompare, objectGetHash, objectFree, 0);

	c->title = bts_strcpy(locationTitleList[0]);

	for (y = 47; y > 0; y--) {
		for (x = 0; x < 32; x++) {
			index = (y * 32) + x;
			if (data->buf[index]) {
				addBuilding(c, objectHash, data, x, y, 
						data->buf[index]);
			} else {
				addEmptySpace(c, data, x, y);
			}
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
