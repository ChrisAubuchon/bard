#include <cnvlib.h>
#include <cnv_common.h>
#include <cnv_int.h>
#include <cnv_mon.h>
#include <assert.h>

/********************************/
/*				*/
/* Static data			*/
/*				*/
/********************************/

static uint8_t *spattackTypes[] = {
	NULL, "poison", "drain", "nuts", "old",
	"possess", "stone", "critical", "unequip", "spptdrain"
};

static uint8_t *brAttStr[] = {
	"fry", "freeze", "shock", "drain", "burn", "choke",
	"steam", "blast", "hit", "nuke", ""
};

static uint8_t *pronoun[] = {
	"it", "him", "her", "none"
};

static uint8_t *actTypes[] = {
	"", "melee", "spell", "dopple",
	"breath", "fire", "summonhelp", "tarjan",
	"unused"
};

static uint8_t *meleeStr[] = {
	"",		"",
	" swings at ",	" slashes at ",
	" kicks at ",	" punches at ",
	" claws at ",	" tears at ",
	" bites at ",	" bites at ",
	" stabs at ",	" slices at ",
	" slams ",	" strikes at ",
	" gropes at ",	" reaches toward ",
	" peers at ",	" stares at "

};

static uint8_t *fireType[] = {
	NULL,
	" shoots an arrow",
	" throws a spear",
	" throws an axe",
	" hurls a shuriken",
	" throws a boomerang",
	" hurls a hammer",
	" throws a blade",
	" makes a light"
};

static uint8_t *useType[] = {
  "", " casts a spell", " makes a light", " invokes a figurine",
  "USE_ATEIT", "USE_CASTWEAPON", " breathes", "USE_RENERGIZE",
  "USE_DRINK", "USE_SHOOTARROW", "USE_THROWSPEAR", "USE_THROWAXE",
  "USE_HURLSHURI", "USE_THROWBOOM", "USE_HURLHAMMER", "USE_THROWBLADE"
};

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

uint8_t *getSpecialAttack(uint8_t sp)
{
  assert(sp < 10);

  return spattackTypes[sp];
}

uint8_t *getAttypeString(uint8_t at)
{
  assert(at < 11);

  return brAttStr[at];
}

uint8_t *getActionString(uint8_t a)
{
  assert(a < 8);

  return actTypes[a];
}

uint8_t *getMeleeString(uint8_t m)
{
	assert(m < 9);

	return meleeStr[m];
}

json_t *meleeString_toJson(uint8_t index)
{
	json_t		*root, *array;

	root = json_object();
	array = json_array();
	json_array_append(array, json_string(meleeStr[index * 2]));
	json_array_append(array, json_string(meleeStr[(index * 2) + 1]));
	json_object_set_new(root, "__records", array);

	return root;
}

paramList_t *meleeString_toParam(uint8_t index)
{
	paramList_t	*pl;

	pl = paramList_new();

	param_add(pl, PARAM_STRING, "", meleeStr[index * 2]);
	param_add(pl, PARAM_STRING, "", meleeStr[(index * 2) + 1]);

	return pl;
}


uint8_t *getFireString(uint8_t f)
{
	assert(f < 14);

	return fireType[f];
}

uint8_t *getUseString(uint8_t u)
{
  assert(u < 16);

  return useType[u];
}

uint8_t *getPronounString(uint8_t p)
{
  assert(p < 4);

  return pronoun[p];
}

uint8_t *getSubtype(uint8_t type, uint8_t subtype)
{
  switch (type) {
    case ACT_MELEE:
	return getMeleeString(subtype);
    case ACT_FIRE:
	return getFireString(subtype);
    case ACT_USE:
	return getUseString(subtype);
    default:
	return "SUB_NONE";
  }

  return NULL;
}
