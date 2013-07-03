#include <cnvlib.h>
#include <cnv_race.h>


/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

static void 		btRace_free(const void *vbr);
static json_t 		*btRace_toJson(const void *vbr);
static json_t		*btRace_toJsonArray(const void *vbr);
static btstring_t	*btRace_toName(const void *vbr);


/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

static void btRace_free(const void *vbr)
{
	btRace_t	*br = (btRace_t *)vbr;

	if (br == NULL)
		return;

	bts_free(br->name);
	cnvList_free(br->startingClasses);
	free(br);
}

static json_t *btRace_toJson(const void *vbr)
{
	btRace_t	*br = (btRace_t *)vbr;
	json_t		*root;

	root = json_object();

	JSON_NUMBER(root, "base_st", br->base_st);
	JSON_NUMBER(root, "base_iq", br->base_iq);
	JSON_NUMBER(root, "base_dx", br->base_dx);
	JSON_NUMBER(root, "base_cn", br->base_cn);
	JSON_NUMBER(root, "base_lk", br->base_lk);
	JSON_BTSTRING(root, "name", br->name);
	json_object_set_new(root, "starting_classes", 
			cnvList_toJsonArray(br->startingClasses));

	return root;
}

static json_t *btRace_toJsonArray(const void *vbr)
{
	btRace_t	*br = (btRace_t *)vbr;

	return json_string(br->name->buf);
}

static btstring_t *btRace_toName(const void *vbr)
{
	btRace_t	*br = (btRace_t *)vbr;

	return br->name;
}

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

cnvList_t *raceList_new(void)
{
	return cnvList_new(btRace_free, btRace_toJson, btRace_toName);
}

void raceList_toJson(cnvList_t *rl, btstring_t *fname)
{
	json_t		*root;

	root = cnvList_toJsonObject(rl);
	cnvList_setToJson(rl, btRace_toJsonArray);
	json_object_update(root,cnvList_toJsonArray(rl));

	JSON_DUMP(root, fname);
}

btRace_t *btRace_new(void)
{
	btRace_t	*br;

	br = (btRace_t *)xzalloc(sizeof(btRace_t));
	br->startingClasses = cnvList_btstring();

	return br;
}










