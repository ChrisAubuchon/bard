#include <b3lib.h>
#include <cnv_class.h>
#include <class.h>

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
