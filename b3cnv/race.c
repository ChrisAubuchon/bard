#include <b3lib.h>
#include <cnv_race.h>

static uint8_t *raceName[] = {
	"Human",
	"Elf",
	"Dwarf",
	"Hobbit",
	"Half-Elf",
	"Half-Orc",
	"Gnome"
};

typedef struct {
	uint8_t		base_st;
	uint8_t		base_iq;
	uint8_t		base_dx;
	uint8_t		base_cn;
	uint8_t		base_lk;
} baseStats_t;

static baseStats_t raceStats[] = {
	{ 10, 6, 8, 8, 5	},
	{ 8, 9, 9, 6, 6		},
	{ 12, 6, 7, 10, 3	},
	{ 4, 6, 12, 5, 10	},
	{ 9, 8, 9, 7, 6		},
	{ 11, 3, 8, 11, 4	},
	{ 9, 10, 7, 3, 4	}
};

typedef struct {
	uint8_t		warrior;
	uint8_t		wizard;
	uint8_t		sorcerer;
	uint8_t		conjurer;
	uint8_t		magician;
	uint8_t		rogue;
	uint8_t		bard;
	uint8_t		paladin;
	uint8_t		hunter;
	uint8_t		monk;
} startingClasses_t;

static startingClasses_t raceClasses[] = {
	{ 1, 0, 0, 1, 1, 1, 1, 1, 1, 1 },	/* Human	*/
	{ 1, 0, 0, 1, 1, 1, 1, 1, 0, 1 },	/* Elf		*/
	{ 1, 0, 0, 0, 0, 1, 1, 1, 1, 1 },	/* Dwarf	*/
	{ 1, 0, 0, 1, 1, 1, 1, 0, 0, 1 },	/* Hobbit	*/
	{ 1, 0, 0, 1, 1, 1, 1, 0, 0, 1 },	/* Half-Elf	*/
	{ 1, 0, 0, 1, 1, 1, 0, 0, 1, 0 },	/* Half-Orc	*/
	{ 1, 0, 0, 1, 1, 1, 0, 0, 1, 1 },	/* Gnome	*/
};

void convertRaces(void)
{
	uint32_t	i,j;
	cnvList_t	*rl;
	btRace_t	*br;

	rl = raceList_new();
	for (i = 0; i < 7; i++) {
		br = btRace_new();

		br->name = bts_strcpy(raceName[i]);
		br->base_st = raceStats[i].base_st;
		br->base_iq = raceStats[i].base_iq;
		br->base_dx = raceStats[i].base_dx;
		br->base_cn = raceStats[i].base_cn;
		br->base_lk = raceStats[i].base_lk;

		if (raceClasses[i].warrior)
			cnvList_add(br->startingClasses, bts_strcpy("Warrior"));
		if (raceClasses[i].paladin)
			cnvList_add(br->startingClasses, bts_strcpy("Paladin"));
		if (raceClasses[i].rogue)
			cnvList_add(br->startingClasses, bts_strcpy("Rogue"));
		if (raceClasses[i].bard)
			cnvList_add(br->startingClasses, bts_strcpy("Bard"));
		if (raceClasses[i].hunter)
			cnvList_add(br->startingClasses, bts_strcpy("Hunter"));
		if (raceClasses[i].monk)
			cnvList_add(br->startingClasses, bts_strcpy("Monk"));
		if (raceClasses[i].conjurer)
			cnvList_add(br->startingClasses,bts_strcpy("Conjurer"));
		if (raceClasses[i].magician)
			cnvList_add(br->startingClasses,bts_strcpy("Magician"));

		cnvList_add(rl, br);
	}

	raceList_toJson(rl, mkJsonPath("races.json"));
	cnvList_free(rl);
}
