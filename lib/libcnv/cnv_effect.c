#include <cnvlib.h>
#include <cnv_common.h>
#include <cnv_effect.h>

#define DEBUG
#include <debug.h>

/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

static bteAttack_t *bteAttack_new(void);
static json_t *bteAttack_toJson(const void *vba);
static void bteAttack_free(const void *vba);

static btePassive_t *btePassive_new(void);
static json_t *btePassive_toJson(const void *vbp);
static void btePassive_free(const void *vbp);

static bteDisbelieve_t *bteDisbelieve_new(void);
static json_t *bteDisbelieve_toJson(const void *vbd);
static void bteDisbelieve_free(const void *vbd);

static bteBonus_t *bteBonus_new(void);
static json_t *bteBonus_toJson(const void *vbb);
static void bteBonus_free(const void *vbb);

static bteHeal_t *bteHeal_new(void);
static json_t *bteHeal_toJson(const void *vbh);
static void bteHeal_free(const void *vbh);

static bteSummon_t *bteSummon_new(void);
static json_t *bteSummon_toJson(const void *vbs);
static void bteSummon_free(const void *vbs);

static bteGeneric_t *bteGeneric_new(void);
static json_t *bteGeneric_toJson(const void *vbg);
static void bteGeneric_free(const void *vbg);

/********************************/
/*				*/
/* btEffect_t functions		*/
/*				*/
/********************************/

struct btEffect_t {
	btFunc_free	free;
	btFunc_toJson	toJson;
	void		*data;
};


btEffect_t *btEffect_new(uint8_t type)
{
	btEffect_t	*rval;

	rval = (btEffect_t *)xzalloc(sizeof(btEffect_t));
	switch (type) {
	case EFFECT_NONE:
		break;
		rval->free	= NULL;
		rval->toJson	= NULL;
		rval->data	= NULL;
	case EFFECT_ATTACK:
		rval->free	= bteAttack_free;
		rval->toJson	= bteAttack_toJson;
		rval->data	= bteAttack_new();
		break;
	case EFFECT_PASSIVE:
		rval->free	= btePassive_free;
		rval->toJson	= btePassive_toJson;
		rval->data	= btePassive_new();
		break;
	case EFFECT_DISBELIEVE:
		rval->free	= bteDisbelieve_free;
		rval->toJson	= bteDisbelieve_toJson;
		rval->data	= bteDisbelieve_new();
		break;
	case EFFECT_BONUS:
		rval->free	= bteBonus_free;
		rval->toJson	= bteBonus_toJson;
		rval->data	= bteBonus_new();
		break;
	case EFFECT_HEAL:
		rval->free	= bteHeal_free;
		rval->toJson	= bteHeal_toJson;
		rval->data	= bteHeal_new();
		break;
	case EFFECT_SUMMON:
		rval->free	= bteSummon_free;
		rval->toJson	= bteSummon_toJson;
		rval->data	= bteSummon_new();
		break;
	case EFFECT_GENERIC:
		rval->free	= bteGeneric_free;
		rval->toJson	= bteGeneric_toJson;
		rval->data	= bteGeneric_new();
		break;
	}

	return rval;
}

void btEffect_free(const void *vbe)
{
	btEffect_t *be = (btEffect_t *)vbe;

	if (be == NULL)
		return;

	be->free(be->data);
	free(be);
}

json_t *btEffect_toJson(const void *vbe)
{
	btEffect_t *be = (btEffect_t *)vbe;

	if (be == NULL)
		return NULL;

	return be->toJson(be->data);
}

void *btEffect_getDataPointer(btEffect_t *be)
{
	if (be == NULL)
		return NULL;

	return be->data;
}


/********************************/
/*				*/
/* repel_t functions		*/
/*				*/
/********************************/

json_t *repel_toJson(repel_t *r)
{
	json_t		*root, *node;

	root = json_object();
	node = json_object();

	JSON_TRUE_IF(node, "evil", r->evil);
	JSON_TRUE_IF(node, "demon", r->demon);
	JSON_TRUE_IF(node, "spellcaster", r->spellcaster);
	JSON_TRUE_IF(node, "unk1", r->unk1);
	JSON_TRUE_IF(node, "unk2", r->unk2);
	JSON_TRUE_IF(node, "unk3", r->unk3);
	JSON_TRUE_IF(node, "unk4", r->unk4);
	JSON_TRUE_IF(node, "unk5", r->unk5);

	json_object_set_new(root, "repel", node);

	return root;
}

/********************************/
/*				*/
/* elements_t functions		*/
/*				*/
/********************************/

json_t *elements_toJson(elements_t *e)
{
	json_t		*root, *node;

	root = json_object();
	node = json_object();

	JSON_TRUE_IF(node, "fire", e->fire);
	JSON_TRUE_IF(node, "unk1", e->unk1);
	JSON_TRUE_IF(node, "holy", e->holy);
	JSON_TRUE_IF(node, "ice", e->ice);
	JSON_TRUE_IF(node, "lightning", e->lightning);
	JSON_TRUE_IF(node, "spell", e->spell);
	JSON_TRUE_IF(node, "unk2", e->unk2);
	JSON_TRUE_IF(node, "unk3", e->unk3);

	json_object_set_new(root, "elements", node);

	return root;
}

/********************************/
/*				*/
/* btTargetting_t functions	*/
/*				*/
/********************************/

json_t *btTargetting_toJson(btTargetting_t *bto)
{
	json_t		*root, *node;

	if (bto->targetted) {
		root = json_object();
		node = json_object();

		JSON_TRUE_IF(node, "party",	bto->targetParty);
		JSON_TRUE_IF(node, "summon",	bto->targetSummon);
		JSON_TRUE_IF(node, "monster",	bto->targetMonster);
		JSON_TRUE_IF(node, "melee",	bto->targetMelee);
		json_object_set_new(root, "targetted", node);

		return root;
	}

	root = json_object();
	JSON_FALSE(root, "targetted");
	return root;
}

/********************************/
/*				*/
/* bteAttack functions		*/
/*				*/
/********************************/

static bteAttack_t *bteAttack_new(void)
{
	bteAttack_t	*rval;

	rval = (bteAttack_t *)xzalloc(sizeof(bteAttack_t));

	return rval;
}

static void bteAttack_free(const void *vba)
{
	bteAttack_t	*ba = (bteAttack_t *)vba;

	if (ba == NULL)
		return;

	free(ba);
}

static json_t *bteAttack_toJson(const void *vba)
{
	bteAttack_t	*ba = (bteAttack_t *)vba;
	json_t		*node, *inData;

	node = json_object();
	inData = json_object();

	JSON_NUMBER_IF_NOT_ZERO(inData, "ndice", ba->ndice);
	JSON_NUMBER_IF_NOT_ZERO(inData, "dieval", ba->dieval);
	JSON_TRUE_IF(inData, "allFoes", ba->allFoes);
	JSON_TRUE_IF(inData, "group", ba->group);
	JSON_TRUE_IF(inData, "levelMultiply", ba->levelMult);
	JSON_TRUE_IF(inData, "isSpell", ba->isSpell);
	JSON_TRUE_IF(inData, "isBreath", ba->isBreath);
	JSON_TRUE_IF(inData, "outOfRange", ba->outOfRange);
	JSON_NUMBER_IF_NOT_ZERO(inData, "range", ba->range);
	JSON_TRUE_IF(inData, "addDistance", ba->addDistance);
	JSON_NUMBER_IF_NOT_ZERO(inData, "distance", ba->addDistance);
	JSON_STRING_IF(inData, "specialAttack", getSpecialAttack(ba->spAttack));
	JSON_STRING(inData, "atype", getAttypeString(ba->attype));
	if (ba->meleeString) {
		json_object_set_new(inData, "meleeString",
				meleeString_toJson(ba->meleeString));
	}

	json_object_update(inData, repel_toJson(&ba->rflags));
	json_object_update(inData, elements_toJson(&ba->elem));

	json_object_set_new(node, "inData", inData);

	return node;
}


/********************************/
/*				*/
/* btePassive functions		*/
/*				*/
/********************************/

static btePassive_t *btePassive_new(void)
{
	btePassive_t	*rval;

	rval = (btePassive_t *)xzalloc(sizeof(btePassive_t));

	return rval;
}

static void btePassive_free(const void *vbp)
{
	btePassive_t	*bp = (btePassive_t *)vbp;

	if (bp == NULL)
		return;

	free(bp);
}

static json_t *btePassive_toJson(const void *vbp)
{
	btePassive_t	*bp = (btePassive_t *)vbp;
	json_t		*node;
	json_t		*inData;

	node = json_object();
	inData = json_object();

	JSON_NUMBER(inData, "duration", 
			(bp->duration == 255 ? -1 : bp->duration));

	switch (bp->type) {
	case PASS_LIGHT:
		JSON_NUMBER(inData, "distance", bp->lightDistance);
		JSON_TRUE_IF(inData, "detectSecret", bp->detectSecret);
		break;
	case PASS_DETECT:
		JSON_TRUE_IF(inData, "detectStairs", bp->detectStairs);
		JSON_TRUE_IF(inData, "detectTraps", bp->detectTraps);
		JSON_TRUE_IF(inData, "detectSpecial", bp->detectSpecial);
		break;
	case PASS_SHIELD:
		JSON_NUMBER(inData, "acBonus", bp->acBonus);
		break;
	case PASS_LEVITATE:
	case PASS_COMPASS:
		break;
	}

	json_object_set_new(node, "inData", inData);

	return node;
}

/********************************/
/*				*/
/* bteDisbelieve functions	*/
/*				*/
/********************************/

static bteDisbelieve_t *bteDisbelieve_new(void)
{
	bteDisbelieve_t	*rval;

	rval = (bteDisbelieve_t *)xzalloc(sizeof(bteDisbelieve_t));

	return rval;
}

static void bteDisbelieve_free(const void *vbd)
{
	bteDisbelieve_t	*bd = (bteDisbelieve_t *)vbd;

	if (bd == NULL)
		return;

	free(bd);
}

static json_t *bteDisbelieve_toJson(const void *vbd)
{
	bteDisbelieve_t	*bd = (bteDisbelieve_t *)vbd;
	json_t		*node, *inData;

	node = json_object();
	inData = json_object();

	JSON_TRUE_IF(inData, "disbelieve", bd->disbelieve);
	JSON_TRUE_IF(inData, "allBattle", bd->allBattle);
	JSON_TRUE_IF(inData, "revealDoppelganger", bd->revealDoppelganger);
	JSON_TRUE_IF(inData, "noSummon", bd->noSummon);

	json_object_set_new(node, "inData", inData);

	return node;
}

/********************************/
/*				*/
/* bteBonus functions		*/
/*				*/
/********************************/

static bteBonus_t *bteBonus_new(void)
{
	bteBonus_t	*rval;

	rval = (bteBonus_t *)xzalloc(sizeof(bteBonus_t));

	return rval;
}

static void bteBonus_free(const void *vbb)
{
	bteBonus_t	*bb = (bteBonus_t *)vbb;

	if (bb == NULL)
		return;

	free(bb);
}

static json_t *bteBonus_toJson(const void *vbb)
{
	bteBonus_t	*bb = (bteBonus_t *)vbb;
	json_t		*node, *inData;

	node = json_object();
	inData = json_object();

	JSON_TRUE_IF(inData, "group", bb->group);
	JSON_NUMBER_IF_NOT_ZERO(inData, "antiMagic", bb->antiMagic);

	JSON_TRUE_IF(inData, "acBonus", bb->acBonus);
	JSON_TRUE_IF(inData, "acPenalty", bb->acPenalty);
	JSON_TRUE_IF(inData, "acStack", bb->acStack);
	JSON_TRUE_IF(inData, "acRandom", bb->acRandom);
	JSON_NUMBER_IF_NOT_ZERO(inData, "acAmount", bb->acAmount);
	JSON_NUMBER_IF_NOT_ZERO(inData, "acDice", bb->acDice);

	JSON_TRUE_IF(inData, "toHitBonus", bb->toHitBonus);
	JSON_TRUE_IF(inData, "toHitPenalty", bb->toHitPenalty);
	JSON_TRUE_IF(inData, "toHitStack", bb->toHitStack);
	JSON_TRUE_IF(inData, "toHitRandom", bb->toHitRandom);
	JSON_NUMBER_IF_NOT_ZERO(inData, "toHitAmount", bb->toHitAmount);
	JSON_NUMBER_IF_NOT_ZERO(inData, "toHitDice", bb->toHitDice);

	JSON_TRUE_IF(inData, "damageBonus", bb->damageBonus);
	JSON_TRUE_IF(inData, "damagePenalty", bb->damagePenalty);
	JSON_TRUE_IF(inData, "damageStack", bb->damageStack);
	JSON_TRUE_IF(inData, "damageRandom", bb->damageRandom);
	JSON_NUMBER_IF_NOT_ZERO(inData, "damageAmount", bb->damageAmount);
	JSON_NUMBER_IF_NOT_ZERO(inData, "damageDice", bb->damageDice);

	json_object_set_new(node, "inData", inData);

	return node;
}

/********************************/
/*				*/
/* bteHeal functions		*/
/*				*/
/********************************/

static bteHeal_t *bteHeal_new(void)
{
	bteHeal_t	*rval;

	rval = (bteHeal_t *)xzalloc(sizeof(bteHeal_t));

	return rval;
}

static void bteHeal_free(const void *vbh)
{
	bteHeal_t	*bh = (bteHeal_t *)vbh;

	if (bh == NULL)
		return;

	free(bh);
}

static json_t *bteHeal_toJson(const void *vbh)
{
	bteHeal_t	*bh = (bteHeal_t *)vbh;
	json_t		*node, *inData;

	node = json_object();
	inData = json_object();

	JSON_NUMBER_IF_NOT_ZERO(inData, "ndice", bh->ndice);
	JSON_NUMBER_IF_NOT_ZERO(inData, "dieval", bh->dieval);
	JSON_TRUE_IF(inData, "randomHeal", bh->randomHeal);
	JSON_TRUE_IF(inData, "fullHeal", bh->fullHeal);
	JSON_TRUE_IF(inData, "groupHeal", bh->groupHeal);
	JSON_TRUE_IF(inData, "levelMultiply", bh->levelMultiply);
	JSON_TRUE_IF(inData, "old", bh->old);
	JSON_TRUE_IF(inData, "dispossess", bh->dispossess);
	JSON_TRUE_IF(inData, "resurrect", bh->resurrect);
	JSON_TRUE_IF(inData, "poison", bh->poison);
	JSON_TRUE_IF(inData, "paralysis", bh->paralysis);
	JSON_TRUE_IF(inData, "insanity", bh->insanity);
	JSON_TRUE_IF(inData, "stoned", bh->stoned);
	JSON_NUMBER_IF_NOT_ZERO(inData, "quenchBard", bh->quenchBard);

	json_object_set_new(node, "inData", inData);

	return node;
}

/********************************/
/*				*/
/* bteSummon functions		*/
/*				*/
/********************************/

static bteSummon_t *bteSummon_new(void)
{
	bteSummon_t	*rval;

	rval = (bteSummon_t *)xzalloc(sizeof(bteSummon_t));

	return rval;
}

static void bteSummon_free(const void *vbs)
{
	bteSummon_t	*bs = (bteSummon_t *)vbs;

	if (bs == NULL)
		return;

	bts_free(bs->sumOne);
	bts_free(bs->sumZero);

	free(bs);
}

static json_t *bteSummon_toJson(const void *vbs)
{
	bteSummon_t	*bs = (bteSummon_t *)vbs;
	json_t		*node, *inData;

	node = json_object();
	inData = json_object();

	JSON_TRUE_IF(inData, "isRandom", bs->isRandom);
	JSON_TRUE_IF(inData, "isIllusion", bs->isIllusion);
	JSON_BTSTRING_IF(inData, "sumOne", bs->sumZero);
	JSON_BTSTRING_IF(inData, "sumTwo", bs->sumOne);

	json_object_set_new(node, "inData", inData);

	return node;
}

/********************************/
/*				*/
/* bteGeneric functions		*/
/*				*/
/********************************/

static bteGeneric_t *bteGeneric_new(void)
{
	bteGeneric_t	*rval;

	rval = (bteGeneric_t *)xzalloc(sizeof(bteGeneric_t));

	return rval;
}

static void bteGeneric_free(const void *vbg)
{
	bteGeneric_t	*bg = (bteGeneric_t *)vbg;

	if (bg == NULL)
		return;

	if (bg->type == GENS_STRING) {
		bts_free(bg->funcString);
	}

	free(bg);
}

static json_t *bteGeneric_toJson(const void *vbg)
{
	bteGeneric_t	*bg = (bteGeneric_t *)vbg;
	json_t		*node, *inData;

	node = json_object();
	inData = json_object();

	JSON_NUMBER_IF_NOT_ZERO(inData, "data", bg->flags);

	json_object_set_new(node, "inData", inData);

	return node;
}
