#include <b1lib.h>
#include <dehuf.h>
#include <cnv_city.h>
#include <cnv_item.h>
#include <dun.h>
#include <gl_list.h>
#include <gl_linkedhash_list.h>

#define DEBUG 1
#include <debug.h>

/****************************************/
/*					*/
/* Static Variables			*/
/*					*/
/****************************************/

typedef struct {
	uint8_t x;
	uint8_t y;
} sq_t;

static uint8_t *specFace[] = {
	NULL,
	"PIC_GUILDEXT",
	"PIC_BAREXT",
	"PIC_GARTHEXT",
	"PIC_TEMPLEEXT",
 	NULL,NULL,NULL,NULL,NULL,NULL,NULL,
	"PIC_CITYSTATUE",
	"PIC_IRONGATE",
	"PIC_TEMPLEEXT",
	NULL,NULL,NULL,NULL,
	"PIC_CASTLEEXT",
	NULL,
	"PIC_CITYGATE",
 	NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL 
};

static uint8_t *specEvent[] = {
	"cityBuildings:enter(\"empty\")",
	"cityBuildings:enter(\"guild\")",
	"tavern:new",
	"garths:enter()",
	"temple:new",
	"review:enter()",
	"cityBuildings:enter(\"stables\")",
	"",
	"",
	"",
	"",
	"",
	"return cityGuardian:new",
	"return cityIrongate:new",
	"cityBuildings:enter(\"madgod\")",
	"cityBuildings:enter(\"sewerPortal\")",
	"city:printCredits()",
	"roscoes:enter()",
	"cityBuildings:enter(\"kylearan\")",
	"cityBuildings:enter(\"harkyn\")",
	"cityBuildings:enter(\"mangar\")",
	"cityBuildings:enter(\"cityGates\")"
};

uint8_t *bldgFace[] = {
	"",
	"building1",
	"building2",
	"building3",
	"building4"
};

static uint8_t *streets[] = {
	"Alley",
	"Rakhir",
	"Blacksmith",
	"Main",
	"Trumpet",
	"Gray Knife",
	"Stonework",
	"Emerald",
	"Hawk Scabard",
	"Bard Blazon",
	"Tempest",
	"Fargoer",
	"a Blue Highway",
	"Night Archer",
	"Serpentskin",
	"Corbomite",
	"Sinister",
	"Marksman",
	"Dilvish",
	"Death Archer"
};

static uint8_t *taverns[] = {
	"error",
	"Archmage Inn",
	"tavern",
	"tavern",
	"tavern",
	"Scarlet Bard",
	"Ask Y' Mother",
	"Dragonbreath",
	"Skull Tavern",
	"tavern",
	"tavern",
	"tavern",
	"tavern",
	"tavern",
	"tavern",
	"tavern",
	"tavern",
	"tavern",
	"Drawnblade",
	"Sinister Inn"
};

static uint8_t *temples[] = {
/* 0*/	NULL, NULL, NULL, NULL,
/* 4*/	"100 Blessings", NULL, NULL, NULL,
/* 8*/	NULL, NULL, NULL, "Great Gods",
/*12*/	NULL, NULL, NULL, NULL,
/*16*/	NULL, "Greater Gods", NULL, NULL,
/*20*/	"Thief Temple", NULL, NULL, NULL,
/*24*/	NULL, NULL, "Tmpl of Kiosk", NULL,
/*28*/	NULL, NULL, NULL, NULL,
};

static uint8_t *guardianDir[] = {
	"east",
	"east",
	"west",
	"south",
	"north",
	"north",
	"north",
	"north",
	"north",
	"east"
};

static uint8_t guardianType[] = {
	0x2b,
	0x5d,
	0x59,
	0x5d,
	0x4a,
	0x4a,
	0x4a,
	0x19,
	0x2b,
	0x2b
};

/****************************************/
/*					*/
/* Output Function Prototypes		*/
/*					*/
/****************************************/

static uint32_t inCity(int32_t x, int32_t y);
static uint32_t bldg_get_hash(const void *data);
static bool bldg_compare(const void *data, const void *cmp);
static void bldg_free(const void *data);
static btstring_t *toLabel(int32_t x, int32_t y, uint8_t sq);
static btstring_t *onEnter(int32_t x, int32_t y, uint8_t sq);
static btstring_t *getStreet(uint8_t sq);
static btstring_t *getFace(uint8_t hi, uint8_t lo);
static btstring_t *doGuardian(int32_t x, int32_t y, uint8_t hi);
static btstring_t *doIrongate(int32_t x, int32_t y, uint8_t hi);

static void addBuilding(btcity_t *city, int32_t x, int32_t y, uint8_t sq);
static void addItems(btcity_t *city);
static void addMonsters(btcity_t *city);

/****************************************/
/*					*/
/* Helper Functions			*/
/*					*/
/****************************************/

/*
 * inCity()
 *
 * return 1 if the square is in the city. This removes
 * the squares that are outside of the city walls
 */
static uint32_t inCity(int32_t x, int32_t y)
{
	if ((x < 0) || (y < 0)) {
		return 0;
	}

	if ((x == 0) && (y == 15)) {
		return 1;
	}

	if (((x == 0) || (y == 0) || (x == 29) || (y == 29)) ||
	     ((y == 1) && ((x > 6) && (x < 17)))) {
		return 0;
	}

	if ((x == 25) && (y <= 2)) {
		return 0;
	}

	return 1;
}

static uint32_t bldg_get_hash(const void *data)
{
	return (uint32_t)data % 255;
}

static bool bldg_compare(const void *data, const void *cmp)
{
	if ((uint32_t)data == (uint32_t)cmp)
		return 1;

	return 0;
}

static void bldg_free(const void *data)
{
}

static uint32_t getBuildingLabel(int32_t x, int32_t y, uint8_t sq)
{
	uint8_t event;
	uint32_t coords;

	coords = (x << 8) | y;
	if ((x == 4) && ((y == 14) || (y == 15))) {
		return coords | (sq << 16);
	}

	event = sq >> 3;
	switch (event) {
	case 1:
	case 2:
		return coords | (sq << 16);
		break;
	case 4:
		if (temples[29 - y] == NULL) {
			return (sq << 16) |  0xff00 | x;
		} else {
			return (sq << 16) | y;
		}
		break;
	default:
		return sq;
		break;
	}
}

static btstring_t *toLabel(int32_t x, int32_t y, uint8_t sq)
{
	if ((x == -1) && (y == 15)) {
			return bts_strcpy("nil");
	} else if (sq & 7) {
		return bts_sprintf("b%08x", getBuildingLabel(x, y, sq));
	} else {
		return bts_sprintf("%d-%d", x, y);
	}
}

static btstring_t *onEnter(int32_t x, int32_t y, uint8_t sq)
{
	uint8_t event;
	btstring_t *rval = NULL;
	uint8_t hi, lo;

	if ((x == 4) && ((y == 15) || (y == 14))) {
		return bts_strcpy(specEvent[6]);
	}

	hi = sq >> 3;
	lo = sq & 7;

	if (hi) {
		switch (hi) {
		case 2:
			rval = bts_sprintf("%s(\"%s\")", specEvent[hi],
					taverns[y]);
			break;
		case 4:
			rval = bts_sprintf("%s(\"%s\")", specEvent[hi],
					(temples[29 - y] == NULL) ?
						(uint8_t *)"Temple" :
						temples[29 - y]);
			break;
		case 12:
			return doGuardian(x, y, hi);
		case 13:
			return doIrongate(x, y, hi);
		default:
			rval = bts_strcpy(specEvent[hi]);
			break;
		}
	} else if (lo) {
		rval = bts_strcpy(specEvent[0]);
	}

	return rval;
}

static btstring_t *getStreet(uint8_t sq)
{
	if (sq == 0xff)
		return bts_strcpy("grand");

	return bts_strcpy(streets[sq]);
}

static btstring_t *getFace(uint8_t hi, uint8_t lo)
{
	if (!hi)
		return bts_strcpy(bldgFace[lo]);

	if (specFace[hi] != NULL)
		return bts_strcpy(specFace[hi]);
	else
		return bts_strcpy(bldgFace[lo]);
}

static void addItems(btcity_t *city)
{
	uint32_t	i;

	for (i = 0; i <= b1dun_randomMask[0]; i++) {
		cnvList_add(city->day->items,
			getItemName(b1dun_randomOffset[0] + i));
	}

	for (i = 0; i <= b1dun_randomMask[1]; i++) {
		cnvList_add(city->night->items,
				getItemName(b1dun_randomOffset[1] + i));
	}
}

static void addMonsters(btcity_t *city)
{
	uint32_t	i;

	/*
	 * The selection of the monster group uses an array
	 * that starts at [1]. If the random result is 0, BTI
	 * just increments.
	 */
	for (i = 1; i <= b1dun_randomMask[0]; i++) {
		cnvList_add(city->day->monsters,
			getMonsterName(b1dun_randomOffset[0] + i - 1));
	}

	for (i = 0; i <= b1dun_randomMask[1]; i++) {
		cnvList_add(city->night->monsters,
			getMonsterName(b1dun_randomOffset[1] + i - 1));
	}
}

static uint8_t guardian_x[] = {  4,  6,  6,  6,  3,  3,  6, 27, 22, 21 };
static uint8_t guardian_y[] = { 26, 26, 24, 22, 14,  6,  6,  6,  3,  2 };

static btstring_t *doGuardian(int32_t x, int32_t y, uint8_t hi)
{
	uint32_t found = 0;
	uint32_t index;

	for (index = 0; index < 10; index++) {
		if ((guardian_x[index] == x) && (guardian_y[index] == y)) {
			found = 1;
			break;
		}
	}

	if (!found) 
		index = 0;

	return bts_sprintf("%s(\"%s\", \"%s\")",
			specEvent[hi],
			guardianDir[index],
			getMonMacro(guardianType[index] - 1)
			);
}

static uint8_t gate_x[] = { 25,  2, 27,  4 };
static uint8_t gate_y[] = { 27,  4, 25,  2 };
static uint8_t *gateDir[] = {
	"east",
	"south",
	"north",
	"west"
};

static btstring_t *doIrongate(int32_t x, int32_t y, uint8_t hi)
{
	uint32_t index;
	uint32_t found	= 0;

	for (index = 0; index < 4; index++) {
		if ((gate_x[index] == x) && (gate_y[index] == y)) {
			found = 1;
			break;
		}
	}

	return bts_sprintf("%s(\"%s\", \"%s\")",
			specEvent[hi],
			gateDir[index],
			(x < 10) ? "Mangar's" : "Kylearan's"
			);
}

/****************************************/
/*					*/
/* Output Functions			*/
/*					*/
/****************************************/

void convertLevels(void)
{
	int32_t x,y;
	uint32_t index = 0;
	btstring_t *name;
	btstring_t *pat;
	sq_t *sq;
	gl_list_t blhash;

	btcity_t *skara;

	uint32_t bl;
	uint32_t *blp;

	blhash = gl_list_nx_create_empty(GL_LINKEDHASH_LIST,
		bldg_compare, bldg_get_hash, bldg_free, 0);

	name = dehufFile(mkBardOnePath("city.nam"), 0x384);
	pat = dehufFile(mkBardOnePath("city.pat"), 0x384);

	skara = btcity_new(bts_strcpy("skara"));
	skara->title = bts_strcpy("Skara Brae");

	for (y = 29; y >= 0; y--) {
		for (x = 0; x < 30; x++) {
			if (!(pat->buf[index] & 7)) {
				if (!inCity(x,y)) {
					index++;
					continue;
				}

				if ((x == 0) && (y = 15)) {
					citypath_new(skara,
					toLabel(x,y,pat->buf[index]),
					toLabel(x,y+1, pat->buf[index-30]),
					toLabel(x, y-1, pat->buf[index+30]),
					toLabel(x+1, y, pat->buf[index+1]),
					toLabel(x-1, y, pat->buf[index-1]),
					onEnter(x,y,pat->buf[index]),
					getStreet(name->buf[index])
					);
				} else if ((x == 25) && (y == 3)) {
					citypath_new(skara,
					toLabel(x,y,pat->buf[index]),
					toLabel(x,y+1, pat->buf[index-30]),
					toLabel(25,7,pat->buf[index]),
					toLabel(x+1, y, pat->buf[index+1]),
					toLabel(x-1, y, pat->buf[index-1]),
					onEnter(x,y,pat->buf[index]),
					getStreet(name->buf[index])
					);
				} else {
					citypath_new(skara,
					toLabel(x, y, pat->buf[index]),
					toLabel(x, y+1, pat->buf[index-30]),
					toLabel(x, y-1, pat->buf[index+30]),
					toLabel(x+1, y, pat->buf[index+1]),
					toLabel(x-1, y, pat->buf[index-1]),
					onEnter(x,y,pat->buf[index]),
					getStreet(name->buf[index])
					);
				}
			} else {
				uint32_t tag;
				uint8_t hi;
				uint8_t lo;
				btstring_t *event;


				tag = getBuildingLabel(x, y, pat->buf[index]);
				if (gl_list_search(blhash, (void *)tag) != NULL) {
					index++;
					continue;
				}
				if (gl_list_nx_add_last(blhash, (void *)tag) == NULL) {
					bt_error("Out of memory: %d", gl_list_size(blhash));
				}

				hi = pat->buf[index] >> 3;
				lo = pat->buf[index] & 7;

				if (hi) {
					event = onEnter(x,y,pat->buf[index]);
				} else if ((x == 4) && ((y == 15) || (y == 14))) {
					event = bts_strcpy(specEvent[6]);
				} else {	
					event = bts_strcpy(specEvent[0]);
				}

				citybldg_new(skara,
					toLabel(x, y, pat->buf[index]),
					bts_strcpy(bldgFace[lo]),
					getFace(hi, lo),
					event
					);
			}

			index++;
		}
	}

	skara->day->poisonDmg = b1dun_poisonDamage[0];
	skara->night->poisonDmg = b1dun_poisonDamage[1];
	addItems(skara);
	addMonsters(skara);

/*	btcity_to_lua(mkLuaPath("skara.lua"), skara);*/
	btcity_to_json(mkJsonPath("skara.json"), skara);

	gl_list_free(blhash);
	btcity_free(skara);
	bts_free(name);
	bts_free(pat);
}
