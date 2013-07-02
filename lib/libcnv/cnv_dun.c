#include <cnvlib.h>
#include <cnv_dun.h>
#include <cnv_int.h>
#include <cnv_item.h>
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
	gl_list_t	edges;
};

/********************************/
/*				*/
/* Local function prototypes	*/
/*				*/
/********************************/

static bool dunVertex_equals(const void *vv1, const void *vv2);
static size_t dunVertex_hash(const void *vv);
static bool dunEdge_equals(const void *ve1, const void *ve2);
static size_t dunEdge_hash(const void *ve);
static size_t dunEdge_hash(const void *ve);
static void dunEdge_free(const void *ve);

static json_t *edges_to_json(dunLevel_t *dl);
static json_t *vertices_to_json(dunLevel_t *dl);
static void dunLevel_to_json(dunLevel_t *dl, btstring_t *fname);

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
	bts_free(v->northEdge);
	bts_free(v->southEdge);
	bts_free(v->eastEdge);
	bts_free(v->westEdge);
	bts_free(v->isTeleport);
	bts_free(v->isStairs);
	bts_free(v->isMessage);

	free(v);
}

static bool dunEdge_equals(const void *ve1, const void *ve2)
{
	dunEdge_t *e1 = (dunEdge_t *)ve1;
	dunEdge_t *e2 = (dunEdge_t *)ve2;

	return (e1->value == e2->value);
}

static size_t dunEdge_hash(const void *ve)
{
	dunEdge_t *edge = (dunEdge_t *)ve;

	return edge->value;
}

static void dunEdge_free(const void *ve)
{
	dunEdge_t *edge = (dunEdge_t *)ve;

	if (edge == NULL) {
		return;
	}

	bts_free(edge->sq1);
	bts_free(edge->gfx1);
	bts_free(edge->sq2);
	bts_free(edge->gfx2);
	free(edge);
}

static void dunLevel_free(const void *vdl)
{
	dunLevel_t *dl = (dunLevel_t *)vdl;

	if (dl == NULL)
		return;
	if (dl->graph != NULL) {
		gl_list_free(dl->graph->squares);
		gl_list_free(dl->graph->edges);
		free(dl->graph);
	}
	cnvList_free(dl->items);
	cnvList_free(dl->monsters);
#if 0
	cnvlist_free(dl->chestTraps);
	cnvList_free(dl->floorTraps);
#endif
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
	json_object_set_new(root,"edges",	edges_to_json(dl));
	json_object_set_new(root,"squares",	vertices_to_json(dl));

	json_object_set_new(root,"items", cnvList_toJsonArray(dl->items));
	json_object_set_new(root,"monsters", cnvList_toJsonArray(dl->monsters));

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

static json_t *edges_to_json(dunLevel_t *dl)
{
	json_t *edges;
	json_t *edgeNode;
	json_t *sqNode;
	gl_list_iterator_t iter;
	gl_list_node_t node;
	dunEdge_t *edge;

	edges = json_object();
	iter = gl_list_iterator(dl->graph->edges);
	while (gl_list_iterator_next(&iter, (const void **)&edge, &node)) {

		edgeNode = json_object();

		sqNode = json_object();
		if (edge->gfx1 == NULL) {
			json_object_set_new(sqNode, "gfx", json_false());
		} else {
			JSON_STRING(sqNode, "gfx", edge->gfx1->buf);
		}
		JSON_STRING(sqNode,	"path",		edge->sq2->buf);
		JSON_TRUE_IF(sqNode,	"secret",	edge->secret1);
		JSON_TRUE_IF(sqNode,	"isWall",	edge->isWall1);
		JSON_TRUE_IF(sqNode,	"canPhase",	edge->canPhase1);
		json_object_set_new(edgeNode,	edge->sq1->buf, sqNode);

		sqNode = json_object();
		if (edge->gfx2 == NULL) {
			json_object_set_new(sqNode, "gfx", json_false());
		} else {
			JSON_STRING(sqNode, "gfx", edge->gfx2->buf);
		}
		JSON_STRING(sqNode,	"path",		edge->sq1->buf);
		JSON_TRUE_IF(sqNode,	"secret",	edge->secret2);
		JSON_TRUE_IF(sqNode,	"isWall",	edge->isWall2);
		JSON_TRUE_IF(sqNode,	"canPhase",	edge->canPhase2);
		json_object_set_new(edgeNode,	edge->sq2->buf, sqNode);

		json_object_set_new(edges, edge->label->buf, edgeNode);
	}

	return edges;
}

static json_t *vertices_to_json(dunLevel_t *dl)
{
	json_t *vertices;
	json_t *vertexNode;
	gl_list_iterator_t iter;
	gl_list_node_t node;
	dunVertex_t *vertex;

	vertices = json_object();
	iter = gl_list_iterator(dl->graph->squares);
	while (gl_list_iterator_next(&iter, (const void **)&vertex, &node)) {
		vertexNode = json_object();
		JSON_STRING(vertexNode,	"north",	vertex->northEdge->buf);
		JSON_STRING(vertexNode,	"south",	vertex->southEdge->buf);
		JSON_STRING(vertexNode,	"east",		vertex->eastEdge->buf);
		JSON_STRING(vertexNode,	"west",		vertex->westEdge->buf);
		JSON_TRUE_IF(vertexNode,"isSpinner",	vertex->isSpinner);
		JSON_TRUE_IF(vertexNode,"isSmoke",	vertex->isSmoke);
		JSON_TRUE_IF(vertexNode,"isNoSpptRegen",vertex->isNoSpptRegen);
		JSON_TRUE_IF(vertexNode,"isAntiMagic",	vertex->isAntiMagic);
		JSON_TRUE_IF(vertexNode,"isLifeDrain",	vertex->isLifeDrain);
		JSON_TRUE_IF(vertexNode,"isDarkness",	vertex->isDarkness);
		JSON_TRUE_IF(vertexNode,"isTrap",	vertex->isTrap);
		JSON_TRUE_IF(vertexNode,"isStairs",	vertex->isStairs);
		JSON_TRUE_IF(vertexNode,"isRandomBattle",vertex->isRandomBattle);
		JSON_TRUE_IF(vertexNode,"isStuck",	vertex->isStuck);
		JSON_TRUE_IF(vertexNode,"isSpecial",	vertex->isSpecial);
		JSON_TRUE_IF(vertexNode,"hasCeilPortal",vertex->hasCeilPortal);
		JSON_TRUE_IF(vertexNode,"hasFloorPortal",vertex->hasFloorPortal);
		JSON_BTSTRING_IF(vertexNode,"isMessage",vertex->isMessage);
		JSON_BTSTRING_IF(vertexNode,"isStairs",	vertex->isStairs);
		JSON_BTSTRING_IF(vertexNode,"isTeleport", vertex->isTeleport);
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

dunEdge_t *dunEdge_new(dunLevel_t *dl, uint32_t value)
{
	dunEdge_t *tmp;
	gl_list_node_t node;

	tmp = (dunEdge_t *)xzalloc(sizeof(dunEdge_t));
	tmp->value	= value;

	node = gl_list_search(dl->graph->edges, (void *)tmp);
	if (node == NULL) {
		tmp->label	= bts_sprintf("x%08x", value);
		tmp->secret1	= 0;
		tmp->canPhase1	= 0;
		tmp->isWall1	= 0;
		tmp->secret2	= 0;
		tmp->canPhase2	= 0;
		tmp->isWall2	= 0;

		node = gl_list_add_last(dl->graph->edges, (void *)tmp);
	} else {
		free(tmp);
	}
	return (dunEdge_t *)gl_list_node_value(dl->graph->edges, node);
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
	rval->graph->edges = gl_list_create_empty(GL_LINKEDHASH_LIST,
		dunEdge_equals, dunEdge_hash, dunEdge_free, 0);

	rval->items = cnvList_btstring();
	rval->monsters = cnvList_btstring();
#if 0
	rlva->floorTraps = cnvList_new(levelTrap_free, levelTrap_toJson,
					levelTrap_toName);
	rval->chestTraps = cnvList_new(levelTrap_free, levelTrap_toJson,
					levelTrap_toName);
#endif

	return rval;
}

void dunList_to_json(cnvList_t *dl, btstring_t *fname)
{
	json_t 			*root;

	root = cnvList_toJsonObject(dl);
	JSON_DUMP(root, fname);

	cnvList_runForEach(dl, dunList_toLevels);
}
