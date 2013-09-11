#include <b2lib.h>
#include <cnv_city.h>
#include <dehuf.h>
#include <level.h>
#include <gl_list.h>
#include <gl_linkedhash_list.h>

#define DEBUG
#include <debug.h>

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#define LOC_OFFSET	256

/********************************/
/*				*/
/* Internal prototypes		*/
/*				*/
/********************************/

static btcity_t		*convertCity(btstring_t *data, uint32_t index);
static uint32_t		inCity(uint32_t index, int32_t x, int32_t y);

/********************************/
/*				*/
/* Internal functions		*/
/*				*/
/********************************/

static uint32_t inCity(uint32_t index, int32_t x, int32_t y)
{
	if ((x < 0) || (y < 0) || (x > 15) || (y > 15))
		return 0;

	switch (index) {
	case 1:
		/* Left corners */
		if ((x == 0) && ((y == 15) || (y == 0))) {
			return 0;
		}

		/* Right side */
		if ((x == 15) && ((y == 15) || (y == 0))) {
			return 0;
		}
		break;
	case 2:
		/* Left side */
		if ((x == 0) && ((y == 15) || (y == 1))) 
			return 0;

		/* Bottom */
		if (y == 0) {
			switch (x) {
				case 0: case 1: case 2:
				case 4: case 6: case 7:
				case 8: case 12: case 15:
					return 0;
			}
		}

		/* Right */
		if (x == 15) {
			switch (y) {
				case 2: case 5: case 7:
				case 11:
					return 0;
			}
		}

		break;
	}

	return 1;
}

static btcity_t *convertCity(btstring_t *data, uint32_t cityIndex)
{
	uint32_t		index;
	int32_t			x, y;
	gl_list_t		blhash;
	btcity_t		*c;

	index		= 0;
	c		= btcity_new(bts_strcpy(locationTitleList[cityIndex]));
	debug("cityIndex: %d\n", cityIndex);

	for (y = 15; y >= 0; y--) {
		for (x = 0; x < 16; x++) {
			if (!data->buf[index]) {
				if (!inCity(cityIndex, x, y)) {
					debug("x: %d, y: %d not inCity\n", x, y);
					index++;
					continue;
				}
			}
			index++;
		}
	}

	return c;
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

	fclose(fp);
}
