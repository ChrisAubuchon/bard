#include <cnvlib.h>
#include <cnv_class.h>


/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

static void btClass_free(const void *vbc);
static json_t *btClass_toJson(const void *vbc);
static btstring_t *btClass_toName(const void *vbc);

static json_t *xpReq_toJson(const void *vxp);


/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

static void btClass_free(const void *vbc)
{
	btClass_t	*bc = (btClass_t *)vbc;

	if (bc == NULL)
		return;

	bts_free(bc->name);
	bts_free(bc->pic);
	cnvList_free(bc->xpReq);
}

static json_t *btClass_toJson(const void *vbc)
{
	btClass_t	*bc = (btClass_t *)vbc;
	json_t		*root;

	root = json_object();

	JSON_BTSTRING(root, "name", bc->name);
	JSON_BTSTRING(root, "pic", bc->pic);
	JSON_NUMBER(root, "hpDice", bc->hpDice + 1);
	JSON_NUMBER(root, "meleeBonus", bc->meleeBonus);
	JSON_NUMBER_IF_NOT_ZERO(root, "saveBonus", bc->saveBonus);
	json_object_set_new(root, "xpreq", cnvList_toJsonArray(bc->xpReq));

	return root;
}

static btstring_t *btClass_toName(const void *vbc)
{
	btClass_t	*bc = (btClass_t *)vbc;

	return bc->name;
}

static json_t *xpReq_toJson(const void *vxp)
{
	uint32_t	xp = (uint32_t)vxp;

	return json_integer(xp);
}

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

cnvList_t *classList_new(void)
{
	return cnvList_new(btClass_free, btClass_toJson, btClass_toName);
}

void classList_toJson(cnvList_t *cl, btstring_t *fname)
{
	json_t		*root;

	root = cnvList_toJsonObject(cl);

	JSON_DUMP(root, fname);
}

btClass_t *btClass_new(void)
{
	btClass_t	*bc;

	bc = (btClass_t *)xzalloc(sizeof(btClass_t));
	bc->xpReq = cnvList_new(NULL, xpReq_toJson, NULL);

	return bc;
}
