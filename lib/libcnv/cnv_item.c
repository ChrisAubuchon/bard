#include <cnvlib.h>
#include <cnv_item.h>
#include <cnv_int.h>

/********************************/
/*				*/
/* Internal structure		*/
/*				*/
/********************************/

/********************************/
/*				*/
/* Internal prototypes		*/
/*				*/
/********************************/

static void btitem_free(const void *vit);
static btstring_t *btitem_toName(const void *vit);
static json_t *btitem_toJson(const void *vit);
static json_t *shopitem_name_toJson(const void *vsi);
static json_t *shopitem_toJson(const void *vsi);

/********************************/
/*				*/
/* Static data			*/
/*				*/
/********************************/

static uint8_t *itemTypeStr[] = {
	"Item",		"Weapon",	"Shield",	"Armor",
	"Helm",		"Gloves",	"Instrument",	"Figurine",
	"Ring",		"Wand",		"Misc. Item",	"Bow",
	"Quiver",	"Container",	"Cloak"
};

static uint8_t *itemEquipClass[] = {
  "EC_ITEM", "EC_WEAPON", "EC_SHIELD", "EC_ARMOR",
  "EC_HELM", "EC_GLOVES", "EC_INSTRUMENT", "EC_FIGURINE",
  "EC_RING", "EC_WAND", "EC_SPECIAL", "EC_BOW",
  "EC_QUIVER", "EC_CONTAINER", "EC_CLOAK"
};

static uint8_t *itemEffectStr[] = {
	"hasBogusEffect", "hasRegenHP", "hasSpptRegen", "hasNoSpin",
	"hasHalfSpptUse", "hasFreeSinging", "hasAlwaysHide", "hasAlwaysRun",
	"hasEffectEight", "hasCalmMonster", "hasNoBreath", "hasEffectEleven",
	"hasQuatSpptUse", "hasRegenSppt", "hasResurrect"
};

/********************************/
/*				*/
/* Internal functions		*/
/*				*/
/********************************/

static void btitem_free(const void *vit)
{
	btitem_t *it = (btitem_t *)vit;

	if (it == NULL)
		return;

	bts_free(it->name);
	bts_free(it->macro);
	btAction_free(it->action);
	free(it);
}

static btstring_t *btitem_toName(const void *vit)
{
	btitem_t *it = (btitem_t *)vit;

	return it->name;
}

static json_t *btitem_toJson(const void *vit)
{
	btitem_t 	*it = (btitem_t *)vit;
	json_t		*item;
	json_t		*equip;

	item = json_object();
	JSON_STRING(item, "name", it->name->buf);
	JSON_STRING(item, "type", itemTypeStr[it->type]);
	JSON_STRING(item, "eclass", itemEquipClass[it->eclass]);
	JSON_TRUE_IF(item, itemEffectStr[it->ieffect], it->ieffect);
/*	JSON_TRUE_IF(item, "targetted", it->targetted);*/
	json_object_update(item, btTargetting_toJson(&it->targetting));
	JSON_TRUE_IF(item, "usable", it->canUse);
	JSON_TRUE_IF(item, "has_charges", it->hasCharges);
	JSON_TRUE_IF(item, "rnd_charges", it->rndCharges);
	JSON_TRUE_IF(item, "combat", it->combat);
	JSON_TRUE_IF(item, "noncombat", it->noncombat);
	JSON_NUMBER(item, "dmg_bonus", it->dmgBonus);
	JSON_NUMBER(item, "ac_bonus", it->acBonus);
	JSON_NUMBER(item, "ndice", it->ndice);
	JSON_NUMBER(item, "die", it->die);
	JSON_STRING_IF(item, "sp_attack", getSpecialAttack(it->spAttack));
	JSON_NUMBER_IF_NOT_ZERO(item, "max_charges", it->maxCount);
	JSON_NUMBER(item, "value", it->value);

	equip = json_object();
	JSON_TRUE_IF(equip, "Warrior",	it->canEquip.warrior);
	JSON_TRUE_IF(equip, "Paladin",	it->canEquip.paladin);
	JSON_TRUE_IF(equip, "Rogue",	it->canEquip.rogue);
	JSON_TRUE_IF(equip, "Bard",	it->canEquip.bard);
	JSON_TRUE_IF(equip, "Hunter",	it->canEquip.hunter);
	JSON_TRUE_IF(equip, "Monk",	it->canEquip.monk);
	JSON_TRUE_IF(equip, "Conjurer",	it->canEquip.conjurer);
	JSON_TRUE_IF(equip, "Magician",	it->canEquip.magician);
	JSON_TRUE_IF(equip, "Sorcerer",	it->canEquip.sorcerer);
	JSON_TRUE_IF(equip, "Wizard",	it->canEquip.wizard);
	JSON_TRUE_IF(equip, "Archmage",	it->canEquip.archmage);
	JSON_TRUE_IF(equip, "Chronomancer",it->canEquip.chronomancer);
	JSON_TRUE_IF(equip, "Geomancer",	it->canEquip.geomancer);

	json_object_set(item, "can_equip", equip);

	if (it->action != NULL) {
		JSON_STRING(item, "useString", getUseString(it->use));
/*		json_object_set_new(item, "action", btEffect_toJson(it->effect));*/
		json_object_set_new(item, "action", btAction_toJson(it->action));
	}

	return item;
}

static json_t *shopitem_toJson(const void *vsi)
{
	btitem_t	*it = (btitem_t *)vsi;
	json_t		*node;

	node = json_object();
	JSON_NUMBER(node, "count", it->shopCount);

	return node;
}

static json_t *shopitem_name_toJson(const void *vsi)
{
	btitem_t	*it = (btitem_t *)vsi;

	return json_string(it->name->buf);
}


/********************************/
/*				*/
/* Public Functions		*/
/*				*/
/********************************/

json_t *levelItem_toJson(const void *vli)
{
	levelItem_t 	*li = (levelItem_t *)vli;

	return json_integer(li->weight);
}

btstring_t *levelItem_toName(const void *vli)
{
	levelItem_t	*li = (levelItem_t *)vli;

	return li->name;
}

void levelItem_free(const void *vli)
{
	levelItem_t 	*li = (levelItem_t *)vli;

	if (li == NULL)
		return;

	bts_free(li->name);
	free(li);
}

levelItem_t *levelItem_new(btstring_t *name, uint32_t weight)
{
	levelItem_t	*li;

	li = xzalloc(sizeof(levelItem_t));
	li->name = name;
	li->weight = weight;

	return li;
}


cnvList_t *itemList_new(void)
{
	return cnvList_new(btitem_free, btitem_toJson, btitem_toName);
}

void itemList_to_json(cnvList_t *il, btstring_t *fname)
{
	json_t			*root;

	root = cnvList_toJsonObject(il);
	JSON_DUMP(root, fname);
}

void shopList_to_json(cnvList_t *il, btstring_t *fname)
{
	json_t			*root;
	json_t			*records;
	btFunc_toJson		save;

	save = cnvList_getToJson(il);

	cnvList_setToJson(il, shopitem_toJson);
	root = cnvList_toJsonObject(il);

	cnvList_setToJson(il, shopitem_name_toJson);
	records = cnvList_toJsonArray(il);

	json_object_update(root, records);

	JSON_DUMP(root, fname);
	cnvList_setToJson(il, save);
}

btitem_t *btitem_new(void) 
{
	btitem_t *rval;

	rval = (btitem_t *)xzalloc(sizeof(btitem_t));

	return rval;
}

