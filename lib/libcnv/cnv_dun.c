#include <cnvlib.h>
#include <cnv_dun.h>
#include <cnv_int.h>
#include <cnv_item.h>
#include <cnv_trap.h>
#include <gl_xlist.h>
#include <gl_array_list.h>
#include <gl_linkedhash_list.h>

/********************************/
/*				*/
/* Internal Structures		*/
/*				*/
/********************************/

struct dunGraph_t {
	gl_list_t	squares;
};

/********************************/
/*				*/
/* Local function prototypes	*/
/*				*/
/********************************/

static bool dunVertex_equals(const void *vv1, const void *vv2);
static size_t dunVertex_hash(const void *vv);

static json_t *vertices_to_json(dunLevel_t *dl);
static void dunLevel_to_json(dunLevel_t *dl, btstring_t *fname);

static json_t		*dunPath_toJson(dunPath_t *dp);

static void		dunList_toLevels(const void *vd);

static void	 	dunLevel_free(const void *vdl);
static void 		dunLevel_toJson(const void *vdl);
static btstring_t	*dunLevel_toName(const void *vdl);
static json_t		*dunLevel_toDunJson(const void *vdl);

static void 		dun_free(const void *vd);
static json_t		*dun_toJson(const void *vd);
static btstring_t	*dun_toName(const void *vd);

/********************************/
/*				*/
/* Internal functions		*/
/*				*/
/********************************/

static bool dunVertex_equals(const void *vv1, const void *vv2)
{
	dunVertex_t *v = (dunVertex_t *)vv1;
	dunVertex_t *vertex = (dunVertex_t *)vv2;

	return (v->value == vertex->value);
}

static size_t dunVertex_hash(const void *vv)
{
	dunVertex_t *v = (dunVertex_t *)vv;

	return v->value;
}

static void dunVertex_free(const void *vv)
{
	dunVertex_t *v = (dunVertex_t *)vv;

	bts_free(v->label);
	bts_free(v->isTeleport);
	bts_free(v->isStairs);
	bts_free(v->isMessage);
	bts_free(v->isForcedBattle);
	bts_free(v->code);
	bts_free(v->north.dstSquare);
	bts_free(v->north.gfx);
	bts_free(v->south.dstSquare);
	bts_free(v->south.gfx);
	bts_free(v->east.dstSquare);
	bts_free(v->east.gfx);
	bts_free(v->west.dstSquare);
	bts_free(v->west.gfx);

	free(v);
}

static void dunLevel_free(const void *vdl)
{
	dunLevel_t *dl = (dunLevel_t *)vdl;

	if (dl == NULL)
		return;
	if (dl->graph != NULL) {
		gl_list_free(dl->graph->squares);
		free(dl->graph);
	}
	cnvList_free(dl->items);
	cnvList_free(dl->monsters);
	cnvList_free(dl->chestTraps);
	cnvList_free(dl->floorTraps);
	bts_free(dl->name);
	bts_free(dl->title);
	bts_free(dl->path);
	free(dl);
}

static void dunLevel_toJson(const void *vdl)
{
	dunLevel_t 	*dl = (dunLevel_t *)vdl;
	json_t		*root;
	json_t		*tnode;
	btstring_t	*fname;

	root = json_object();

	JSON_BTSTRING(root,	"title",	dl->title);
	JSON_NUMBER(root,	"tileSet",	dl->tileSet);
	JSON_NUMBER(root,	"level",	dl->level);
	JSON_NUMBER(root,	"dungeonLevel",	dl->dungeonLevel);
	JSON_NUMBER(root,	"poisonDamage",	dl->poisonDmg);
	JSON_BOOL(root,		"canTeleportFrom",dl->canTeleportFrom);
	JSON_STRING(root,	"dungeonDirection", dl->dungeonDirection == DIR_DOWNWARD ? "below" : "above");
	json_object_set_new(root,"squares",	vertices_to_json(dl));

	json_object_set_new(root,"items", cnvList_toJsonArray(dl->items));
	json_object_set_new(root,"monsters", cnvList_toJsonArray(dl->monsters));
	json_object_set_new(root,"floorTraps",cnvList_toJsonArray(dl->floorTraps));
	json_object_set_new(root,"chestTraps",cnvList_toJsonArray(dl->chestTraps));

	fname = bts_sprintf("%s/%s.json", dl->path->buf, dl->name->buf);

	JSON_DUMP(root, fname);
}

static json_t *dunLevel_toDunJson(const void *vdl)
{
	dunLevel_t 	*dl = (dunLevel_t *)vdl;
	json_t		*root;

	root = json_object();
	JSON_STRING(root, "level", dl->name->buf);
	JSON_BOOL(root, "canTeleportTo", dl->canTeleportTo);

	return root;
}


static btstring_t *dunLevel_toName(const void *vdl)
{
	dunLevel_t *dl = (dunLevel_t *)vdl;

	return dl->name;
}

static json_t *dunPath_toJson(dunPath_t *dp)
{
	json_t		*node;

	node = json_object();
	JSON_BTSTRING(node, "path", dp->dstSquare);
	if (dp->gfx == NULL) {
		JSON_FALSE(node, "gfx");
	} else {
		JSON_BTSTRING(node, "gfx", dp->gfx);
	}
	JSON_TRUE_IF(node, "isSecret", dp->isSecret);
	JSON_TRUE_IF(node, "canPhase", dp->canPhase);
	JSON_TRUE_IF(node, "isWall", dp->isWall);
	JSON_TRUE_IF(node, "isDoor", dp->isDoor);

	return node;
}

static json_t *vertices_to_json(dunLevel_t *dl)
{
	json_t			*vertices;
	json_t			*vertexNode;
	gl_list_iterator_t	iter;
	gl_list_node_t		node;
	dunVertex_t		*vertex;

	vertices = json_object();
	iter = gl_list_iterator(dl->graph->squares);
	while (gl_list_iterator_next(&iter, (const void **)&vertex, &node)) {
		vertexNode = json_object();
		json_object_set_new(vertexNode, "north", 
				dunPath_toJson(&vertex->north));
		json_object_set_new(vertexNode, "south", 
				dunPath_toJson(&vertex->south));
		json_object_set_new(vertexNode, "east", 
				dunPath_toJson(&vertex->east));
		json_object_set_new(vertexNode, "west", 
				dunPath_toJson(&vertex->west));
		
		JSON_TRUE_IF(vertexNode,"isSpinner",	vertex->isSpinner);
		JSON_TRUE_IF(vertexNode,"isSmoke",	vertex->isSmoke);
		JSON_TRUE_IF(vertexNode,"isNoSpptRegen",vertex->isNoSpptRegen);
		JSON_TRUE_IF(vertexNode,"isSpptRegen",	vertex->isSpptRegen);
		JSON_TRUE_IF(vertexNode,"isAntiMagic",	vertex->isAntiMagic);
		JSON_TRUE_IF(vertexNode,"isLifeDrain",	vertex->isLifeDrain);
		JSON_TRUE_IF(vertexNode,"isDarkness",	vertex->isDarkness);
		JSON_TRUE_IF(vertexNode,"isTrap",	vertex->isTrap);
		JSON_TRUE_IF(vertexNode,"isStairs",	vertex->isStairs);
		JSON_TRUE_IF(vertexNode,"isRandomBattle",vertex->isRandomBattle);
		JSON_TRUE_IF(vertexNode,"isOdd",	vertex->isOdd);
		JSON_TRUE_IF(vertexNode,"isMakeHostile",vertex->isMakeHostile);
		JSON_TRUE_IF(vertexNode,"isSilent",	vertex->isSilent);
		JSON_TRUE_IF(vertexNode,"isStuck",	vertex->isStuck);
		JSON_TRUE_IF(vertexNode,"isSpecial",	vertex->isSpecial);
		JSON_TRUE_IF(vertexNode,"hasCeilPortal",vertex->hasCeilPortal);
		JSON_TRUE_IF(vertexNode,"hasFloorPortal",vertex->hasFloorPortal);
		JSON_NUMBER_IF_NOT_ZERO(vertexNode,"isSpptDrain", vertex->isSpptDrain);
		JSON_BTSTRING_IF(vertexNode,"isMessage",vertex->isMessage);
		JSON_BTSTRING_IF(vertexNode,"isStairs",	vertex->isStairs);
		JSON_BTSTRING_IF(vertexNode,"isTeleport", vertex->isTeleport);
		JSON_BTSTRING_IF(vertexNode,"isForcedBattle", vertex->isForcedBattle);
		JSON_BTSTRING_IF(vertexNode,"code", vertex->code);
		json_object_set_new(vertices, vertex->label->buf, vertexNode);
	}

	return vertices;
}

static btstring_t *dun_toName(const void *vd)
{
	dun_t		*d = (dun_t *)vd;
	
	return d->name;
}

static json_t *dun_toJson(const void *vd)
{
	json_t		*root;
	dun_t 		*d = (dun_t *)vd;

	root = cnvList_toJsonArray(d->levels);

	return root;
}

static void dunList_toLevels(const void *vd)
{
	dun_t		*d = (dun_t *)vd;

	cnvList_runForEach(d->levels, dunLevel_toJson);
}

static void dun_free(const void *vd)
{
	dun_t *d = (dun_t *)vd;

	if (d == NULL)
		return;

	cnvList_free(d->levels);
	bts_free(d->name);
	free(d);
}

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

dun_t *dun_new(btstring_t *name)
{
	dun_t *rval;

	rval = xzalloc(sizeof(dun_t));
	rval->levels = cnvList_new(dunLevel_free, dunLevel_toDunJson, 
			dunLevel_toName);
	rval->name = name;

	return rval;
}

cnvList_t *dunList_new(void)
{
	return cnvList_new(dun_free, dun_toJson, dun_toName);
}

dunVertex_t *dunVertex_get(dunLevel_t *dl, uint32_t value)
{
	gl_list_node_t node;
	dunVertex_t v;

	v.value = value;

	node = gl_list_search(dl->graph->squares, (void *)&v);
	if (node == NULL) {
		return NULL;
	} else{
		return (dunVertex_t *)gl_list_node_value(dl->graph->squares,
						node);
	}
}

dunVertex_t *dunVertex_new(dunLevel_t *dl, uint32_t value)
{
	dunVertex_t *rval;

	rval = (dunVertex_t *)xzalloc(sizeof(dunVertex_t));
	rval->value = value;
	rval->label = bts_sprintf("x%04x", value);

	gl_list_add_last(dl->graph->squares, rval);

	return rval;
}

dunLevel_t *dunLevel_new(void)
{
	dunLevel_t *rval;

	rval = (dunLevel_t *)xzalloc(sizeof(dunLevel_t));

	rval->graph = (dunGraph_t *)xzalloc(sizeof(dunGraph_t));
	rval->graph->squares = gl_list_create_empty(GL_ARRAY_LIST,
		dunVertex_equals, dunVertex_hash, dunVertex_free, 0);

	rval->items = cnvList_btstring();
	rval->monsters = cnvList_btstring();
	rval->floorTraps = trapList_new();
	rval->chestTraps = trapList_new();

	return rval;
}

void dunList_to_json(cnvList_t *dl, btstring_t *fname)
{
	json_t 			*root;

	root = cnvList_toJsonObject(dl);
	JSON_DUMP(root, fname);

	cnvList_runForEach(dl, dunList_toLevels);
}
