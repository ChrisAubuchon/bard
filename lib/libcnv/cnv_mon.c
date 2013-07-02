#include <cnvlib.h>
#include <cnv_mon.h>
#include <cnv_int.h>
#include <gl_xlist.h>
#include <gl_array_list.h>

/********************************/
/*				*/
/* Local Function Prototypes	*/
/*				*/
/********************************/

static void monster_free(const void *vm);
static json_t *monster_toJson(const void *vm);
static btstring_t *monster_toName(const void *vmon);

static void monsterAttack_free(const void *vma);
static json_t *monsterAttack_toJson(const void *vm);

/********************************/
/*				*/
/* Internal structures		*/
/*				*/
/********************************/

/********************************/
/*				*/
/* Local Functions		*/
/*				*/
/********************************/

static void monster_free(const void *vm)
{
	uint32_t i;

	monster_t *m = (monster_t *)vm;

	bts_free(m->singular);
	bts_free(m->plural);
	bts_free(m->macro);
	bts_free(m->picture);
	cnvList_free(m->attacks);
	free(m);
}

static btstring_t *monster_toName(const void *vmon)
{
	monster_t *mon = (monster_t *)vmon;

	return mon->macro;
}

static json_t *monster_toJson(const void *vm)
{
	json_t		*monRoot;
	monster_t	*m = (monster_t *)vm;
	uint32_t	i;

	monRoot = json_object();

	JSON_BTSTRING(monRoot, "singular",	m->singular);
	JSON_BTSTRING(monRoot, "plural",	m->plural);
	JSON_BTSTRING(monRoot, "picture",	m->picture);
	JSON_STRING(monRoot, "pronoun",		getPronounString(m->pronoun));
	JSON_NUMBER(monRoot, "hpBase",		m->hpBase);
	JSON_NUMBER(monRoot, "hpRndDie",	m->hpRndDie);
	JSON_NUMBER(monRoot, "hpRndNDice",	m->hpRndNdice);
	JSON_NUMBER(monRoot, "reward",		m->reward);
	JSON_NUMBER(monRoot, "ac",		m->baseAC);
	JSON_NUMBER(monRoot, "breathSaveLo",	m->breathSaveLo);
	JSON_NUMBER(monRoot, "breathSaveHi",	m->breathSaveHi);
	JSON_NUMBER(monRoot, "spellSaveLo",	m->spellSaveLo);
	JSON_NUMBER(monRoot, "spellSaveHi",	m->spellSaveHi);
	JSON_NUMBER(monRoot, "toHitLo",		m->toHitLo);
	JSON_NUMBER(monRoot, "toHitHi",		m->toHitHi);
	JSON_NUMBER(monRoot, "priorityLo",	m->priorityLo);
	JSON_NUMBER(monRoot, "priorityHi",	m->priorityHi);
	JSON_BOOL(monRoot, "rndGroupSize",	m->rndGroupSize);
	JSON_NUMBER(monRoot, "groupSize",	m->groupSize);
	JSON_NUMBER(monRoot, "numAttacks",	m->numAttacks);
	/* XXX PRONOUN Handling */
	JSON_BOOL(monRoot, "willAdvance",	m->willAdvance);
	JSON_NUMBER(monRoot, "distance",	m->distance);
	JSON_NUMBER(monRoot, "advanceSpeed",	m->advSpeed);
	JSON_BOOL(monRoot, "isIllusion",	m->isIllusion);

	json_object_set_new(monRoot, "attacks", cnvList_toJsonArray(m->attacks));
	
	return monRoot;
}

static void monsterAttack_free(const void *vma)
{
	monsterAttack_t	*ma = (monsterAttack_t *)vma;

	if (vma == NULL)
		return;

	btEffect_free(ma->effect);
	free(ma);
}

static json_t *monsterAttack_toJson(const void *vma)
{
	monsterAttack_t	*ma = (monsterAttack_t *)vma;
	json_t		*node = NULL;

	if (ma->type) {
		node = json_object();
		JSON_STRING(node, "type", getActionString(ma->type));
		json_object_set_new(node, "action", 
			btEffect_toJson(ma->effect));
	}

	return node;
}

/********************************/
/*				*/
/* Public Functions		*/
/*				*/
/********************************/

cnvList_t *monList_new(void)
{
	return cnvList_new(monster_free, monster_toJson, monster_toName);
}

monster_t *monster_new(void)
{
	monster_t *rval;

	rval = (monster_t *) xzalloc(sizeof(monster_t));
	memset(rval, 0, sizeof(monster_t));

	rval->attacks = cnvList_new(monsterAttack_free, monsterAttack_toJson,
					NULL);

	return rval;
}

void monList_to_json(cnvList_t *ml, btstring_t *fname)
{
	json_t		*root;

	root = cnvList_toJsonObject(ml);

	JSON_DUMP(root, fname);
}

monsterAttack_t *monsterAttack_new(uint8_t type)
{
	monsterAttack_t	*rval;

	rval = (monsterAttack_t *)xzalloc(sizeof(monsterAttack_t));
	rval->type = type;

	return rval;
}
