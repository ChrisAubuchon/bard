#include <cnv_spell.h>
#include <cnv_int.h>
#include <string.h>
#include <gl_xlist.h>
#include <gl_array_list.h>

/********************************/
/*				*/
/* Internal structures		*/
/*				*/
/********************************/

struct spellList_t {
	gl_list_t	spells;
};

struct songList_t {
	gl_list_t	songs;
};

/********************************/
/*				*/
/* Internal prototypes		*/
/*				*/
/********************************/

static void mageSpell_free(const void *vms);
static void bardSong_free(const void *vbs);

static btstring_t *mageSpell_toName(const void *vms);
static btstring_t *bardSong_toName(const void *vbs);

static json_t *mageSpell_toJson(const void *vms);
static json_t *bardSong_toJson(const void *vbs);

/********************************/
/*				*/
/* Internal funtions		*/
/*				*/
/********************************/

static void mageSpell_free(const void *vms)
{
	mageSpell_t *ms = (mageSpell_t *)vms;

	if (ms == NULL)
		return;

	bts_free(ms->abbr);
	bts_free(ms->full);
	bts_free(ms->class);
	free(ms);
}

static void bardSong_free(const void *vbs)
{
	bardSong_t *bs = (bardSong_t *)vbs;

	if (bs == NULL) 
		return;

	cnvList_free(bs->nonCombat);
	cnvList_free(bs->combat);
	bts_free(bs->full);
	free(bs);
}

static btstring_t *mageSpell_toName(const void *vms)
{
	mageSpell_t		*ms = (mageSpell_t *)vms;

	return ms->abbr;
}

static btstring_t *bardSong_toName(const void *vbs)
{
	bardSong_t 	*bs = (bardSong_t *)vbs;

	return bs->full;
}

static json_t *mageSpell_toJson(const void *vms)
{
	json_t			*spellNode;
	mageSpell_t		*ms;
	btstring_t		*effect;

	ms = (mageSpell_t *)vms;
	spellNode = json_object();
	JSON_STRING(spellNode,	"abbreviation",	ms->abbr->buf);
	JSON_STRING(spellNode,	"fullName",	ms->full->buf);
	JSON_NUMBER(spellNode,	"sppt",		ms->sppt);
	JSON_STRING(spellNode,	"class",	ms->class->buf);
	JSON_NUMBER(spellNode,	"level",	ms->level);
	JSON_TRUE_IF(spellNode,	"combat",	ms->combat);
	JSON_TRUE_IF(spellNode, "noncombat",	ms->noncombat);
	json_object_update(spellNode, btTargetting_toJson(&ms->targetting));
#if 0
	json_object_set_new(spellNode, "action",
					btEffect_toJson(ms->effect));
#endif
	json_object_set_new(spellNode, "action",
					btAction_toJson(ms->action));

	return spellNode;
}

static json_t *bardSong_toJson(const void *vbs)
{
	bardSong_t	*bs = (bardSong_t *)vbs;
	json_t		*node;

	node = json_object();
	JSON_BTSTRING(node, "name", bs->full);
	json_object_set_new(node, "combat", 
				cnvList_toJsonArray(bs->combat));
	json_object_set_new(node, "nonCombat", 
				cnvList_toJsonArray(bs->nonCombat));

	return node;
}

/********************************/
/*				*/
/* Public Functions		*/
/*				*/
/********************************/

cnvList_t *spellList_new(void)
{
	return cnvList_new(mageSpell_free, mageSpell_toJson, mageSpell_toName);
}

#if 0
cnvList_t *songList_new(void)
{
	return cnvList_new(bardSong_free, bardSong_toJson, bardSong_toName);
}
#endif

bardSong_t *bardSong_new(void)
{
	bardSong_t *rval;

	rval = (bardSong_t *)xzalloc(sizeof(bardSong_t));
	rval->nonCombat = cnvList_new(btEffect_free, btEffect_toJson, NULL);
	rval->combat = cnvList_new(btEffect_free, btEffect_toJson, NULL);

	return rval;
}

mageSpell_t *mageSpell_new(uint8_t type)
{
	mageSpell_t *rval;

	rval = (mageSpell_t *) xzalloc(sizeof(mageSpell_t));

	return rval;
}

void spellList_to_json(cnvList_t *sl, btstring_t *fname)
{
	json_t *root;

	root = cnvList_toJsonObject(sl);

	JSON_DUMP(root, fname);
}

void songList_to_json(cnvList_t *sl, btstring_t *fname)
{
	json_t			*root, *songNode;

	root = cnvList_toJsonArray(sl);

	JSON_DUMP(root,fname);
}

