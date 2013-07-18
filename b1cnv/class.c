#include <b1lib.h>
#include <cnv_class.h>

static uint8_t *className[] = {
	"Warrior",
	"Paladin",
	"Rogue",
	"Bard",
	"Hunter",
	"Monk",
	"Conjurer",
	"Magician",
	"Sorcerer",
	"Wizard"
};

static uint8_t *classPic[] = {
	"PIC_WARRIOR",
	"PIC_PALADIN",
	"PIC_ROGUE",
	"PIC_BARD",
	"PIC_HUNTER",
	"PIC_MONK",
	"PIC_CONJURER",
	"PIC_MAGICIAN",
	"PIC_SORCERER",
	"PIC_WIZARD"
};

static uint8_t classHpDice[] = {
	15,
	15,
	7,
	15,
	15,
	7,
	3,
	3,
	7,
	7
};

static uint8_t classMeleeBonus[] = {
	2,
	2,
	1,
	1,
	2,	
	3,
	0,
	0,
	0,
	0
};

static uint8_t classSaveBonus[] = {
	1,
	3,
	1,
	1,
	1,
	1,
	2,
	2,
	2,
	3
};

static uint32_t classXpReq[][13] = {
	{
        0, 2000, 4000, 7000, 10000, 15000, 20000, 30000, 50000, 80000,
        110000, 150000, 200000
	},
	{
        0, 2000, 4000, 7000, 10000, 15000, 20000, 30000, 50000, 80000,
        110000, 150000, 200000
	},
	{
        0, 2000, 4000, 7000, 10000, 15000, 20000, 30000, 50000, 80000,
        110000, 150000, 200000
	},
	{
        0, 2000, 4000, 7000, 10000, 15000, 20000, 30000, 50000, 80000,
        110000, 150000, 200000
	},
	{
        0, 2000, 4000, 7000, 10000, 15000, 20000, 30000, 50000, 80000,
        110000, 150000, 200000
	},
	{
        0, 1800, 4000, 6000, 10000, 14000, 19000, 29000, 50000, 90000,
        120000, 170000, 230000
	},
	{
        0, 1800, 4000, 6000, 10000, 14000, 19000, 29000, 50000, 90000,
        120000, 170000, 230000
	},
	{
        0, 1800, 4000, 6000, 10000, 14000, 19000, 29000, 50000, 90000,
        120000, 170000, 230000
	},
	{
        0, 7000, 25000, 40000, 60000, 80000, 100000, 130000, 170000,
        220000, 300000, 400000
	},
	{
        0, 20000, 50000, 80000, 120000, 200000, 250000, 300000, 400000,
        600000, 900000, 1300000
	}
};

void convertClasses(void)
{
	uint32_t 	i,j;
	cnvList_t	*classes;
	btClass_t	*bc;

	classes = classList_new();

	for (i = 0; i < 10; i++)
	{
		bc = btClass_new();
		bc->name = bts_strcpy(className[i]);
		bc->pic = bts_strcpy(classPic[i]);
		bc->hpDice = classHpDice[i];
		bc->meleeBonus = classMeleeBonus[i];
		bc->saveBonus = classSaveBonus[i];

		for (j = 0; j < 13; j++) {
			cnvList_add(bc->xpReq, (void *)classXpReq[i][j]);
		}

		cnvList_add(classes, bc);
	}

	classList_toJson(classes, mkJsonPath("classes.json"));
	cnvList_free(classes);
}
