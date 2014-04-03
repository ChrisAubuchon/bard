#include <b3lib.h>
#include <cnv_class.h>

/********************************/
/*				*/
/* Local data			*/
/*				*/
/********************************/

static uint8_t *className[] = {
/* 0*/	"Warrior",
/* 1*/	"Wizard",
/* 2*/	"Sorcerer",
/* 3*/	"Conjurer",
/* 4*/	"Magician",
/* 5*/	"Rogue",
/* 6*/	"Bard",
/* 7*/	"Paladin",
/* 8*/	"Hunter",
/* 9*/	"Monk",
/*10*/	"Archmage",
/*11*/	"Chronomancer",
/*12*/	"Geomancer"
};

static uint8_t *classPic[] = {
	"warrior",
	"magician",
	"magician",
	"magician",
	"magician",
	"warrior",
	"warrior",
	"warrior",
	"warrior",
	"warrior",
	"warrior",
	"magician",
	"magician",
	"warrior"
};

static uint8_t classHpDice[] = {
	15,	
	7,	
	7,	
	3,	
	3,	
	7,	
	15,	
	15,	
	15,	
	7,	
	7,	
	7,	
	7,	
	0
};

static uint8_t classMeleeBonus[] = {
	3,
	1,
	1,
	1,
	1,
	0,
	0,
	3,
	3,
	4,
	1,
	1,
	4,
	0
};

static uint8_t classSaveBonus[] = {
	0,
	10,
	10,
	5,
	5,
	0,
	0,
	5,
	0,
	0,
	17,
	17,
	22,
	0
};

static uint32_t	warriorXpReq[] = {
        0, 2000, 4000, 7000, 10000, 15000, 20000, 30000, 50000, 80000,
        110000, 150000, 200000
};

static uint32_t wizardXpReq[] = {
	0, 20000, 50000, 80000, 120000, 160000, 200000, 250000,
	300000, 400000, 600000, 900000, 1300000
};

static uint32_t sorcererXpReq[] = {
	0, 7000, 15000, 25000, 40000, 60000, 80000, 100000, 130000,
	170000, 220000, 300000, 400000
};

static uint32_t magicianXpReq[] = {
	0, 1800, 4000, 6000, 10000, 14000, 19000, 29000, 50000,
	90000, 120000, 170000, 230000
};

static uint32_t archmageXpReq[] = {
	0, 70000, 140000, 240000, 340000, 540000, 740000, 1000000, 1400000,
	1800000, 2200000, 2600000, 3000000
};

static uint32_t geomancerXpReq[] = {
	0, 100000, 225000, 400000, 650000, 950000, 1400000, 1800000, 2200000,
	2600000, 3000000, 3400000, 3800000
};

static uint32_t *classXpReq[] = {
	warriorXpReq,
	wizardXpReq,
	sorcererXpReq,
	magicianXpReq,
	magicianXpReq,
	warriorXpReq,
	warriorXpReq,
	warriorXpReq,
	warriorXpReq,
	magicianXpReq,
	archmageXpReq,
	archmageXpReq,
	geomancerXpReq
};

void convertClasses(void)
{
	uint32_t 	i,j;
	cnvList_t	*classes;
	btClass_t	*bc;

	classes = classList_new();

	for (i = 0; i < 13; i++)
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
