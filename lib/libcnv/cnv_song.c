#include <cnvlib.h>
#include <cnv_song.h>



/********************************/
/*				*/
/* Internal prototypes		*/
/*				*/
/********************************/

static void btSong_free(const void *vbs);
static json_t *btSong_toJson(const void *vbs);
static btstring_t *btSong_toName(const void *vbs);

/********************************/
/*				*/
/* Internal functions		*/
/*				*/
/********************************/

static void btSong_free(const void *vbs)
{
	btSong_t	*bs = (btSong_t *)vbs;

	if (bs == NULL)
		return;

	bts_free(bs->name);
	btFunction_free(bs->activate);
	btFunction_free(bs->deactivate);
	btFunction_free(bs->combatFunction);
	cnvList_free(bs->combatData);
	cnvList_free(bs->nonCombatData);
}

static json_t *btSong_toJson(const void *vbs)
{
	btSong_t	*bs = (btSong_t *)vbs;
	json_t		*root;

	if (bs == NULL)
		return NULL;

	root = json_object();
	JSON_BTSTRING(root, "name", bs->name);
	json_object_set_new(root, "activate", btFunction_toJson(bs->activate));
	json_object_set_new(root, "deactivate", 
					btFunction_toJson(bs->deactivate));
	json_object_set_new(root, "combatFunction", 
				btFunction_toJson(bs->combatFunction));
	json_object_set_new(root, "toCombat",
				btFunction_toJson(bs->toCombat));
	json_object_set_new(root, "combatData",
				cnvList_toJsonArray(bs->combatData));
	json_object_set_new(root, "nonCombatData",
				cnvList_toJsonArray(bs->nonCombatData));

	return root;
}

static btstring_t *btSong_toName(const void *vbs)
{
	btSong_t	*bs = (btSong_t *)vbs;

	if (bs == NULL)
		return NULL;

	return bs->name;
}

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

cnvList_t *songList_new(void)
{
	return cnvList_new(btSong_free, btSong_toJson, btSong_toName);
}

void songList_toJson(cnvList_t *sl, btstring_t *fname)
{
	json_t		*root;

	root = cnvList_toJsonArray(sl);

	JSON_DUMP(root, fname);
}

btSong_t *btSong_new(void)
{
	btSong_t	*bs;

	bs = (btSong_t *)xzalloc(sizeof(btSong_t));

	bs->combatData = cnvList_new(btEffect_free, btEffect_toJson, NULL);
	bs->nonCombatData = cnvList_new(btEffect_free, btEffect_toJson, NULL);

	return bs;
}

















