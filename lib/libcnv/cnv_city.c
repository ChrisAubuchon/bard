#include <cnvlib.h>
#include <cnv_city.h>
#include <cnv_int.h>
#include <cnv_item.h>
#include <gl_xlist.h>
#include <gl_linked_list.h>
#include <gl_array_list.h>

/********************************/
/*				*/
/* Internal structures		*/
/*				*/
/********************************/

typedef struct {
	btstring_t *label;
	btstring_t *north;
	btstring_t *south;
	btstring_t *east;
	btstring_t *west;
	btstring_t *onEnter;
	btstring_t *location;
} citypath_t;

typedef struct {
	btstring_t *label;
	btstring_t *dist_face;
	btstring_t *near_face;
	btstring_t *onEnter;
} citybldg_t;

/********************************/
/*				*/
/* Static variables		*/
/*				*/
/********************************/

/********************************/
/*				*/
/* Local prototypes		*/
/*				*/
/********************************/

static void citypath_free(const void *vbtc);
static void citybldg_free(const void *vbl);

static json_t *citypath_toJson(const void *vcp);
static json_t *citybldg_toJson(const void *vcb);

static btstring_t *citypath_toName(const void *vcp);
static btstring_t *citybldg_toname(const void *vcb);

/********************************/
/*				*/
/* Internal functions		*/
/*				*/
/********************************/

static void citypath_free(const void *vbtc)
{
	citypath_t *btc = (citypath_t *)vbtc;

	if (btc == NULL)
		return;

	bts_free(btc->label);
	bts_free(btc->north);
	bts_free(btc->south);
	bts_free(btc->east);
	bts_free(btc->west);
	bts_free(btc->onEnter);
	bts_free(btc->location);
}

static void citybldg_free(const void *vbl)
{
	citybldg_t *bl = (citybldg_t *)vbl;

	if (bl == NULL)
		return;

	bts_free(bl->label);
	bts_free(bl->dist_face);
	bts_free(bl->near_face);
	bts_free(bl->onEnter);
}

static citydata_t *citydata_new(void)
{
	citydata_t 	*rval;

	rval = (citydata_t *)xzalloc(sizeof(citydata_t));
	rval->items = cnvList_btstring();
	rval->monsters = cnvList_btstring();

	return rval;
}

static void citydata_free(const void *vcd)
{
	citydata_t 	*cd = (citydata_t *)vcd;

	cnvList_free(cd->items);
	cnvList_free(cd->monsters);
	free(cd);
}

static json_t *citypath_toJson(const void *vcp)
{
	citypath_t	*cp = (citypath_t *)vcp;
	json_t		*node;

	node = json_object();
	JSON_STRING(node,	"north",	cp->north->buf);
	JSON_STRING(node,	"south",	cp->south->buf);
	JSON_STRING(node,	"west",		cp->west->buf);
	JSON_STRING(node,	"east",		cp->east->buf);
	JSON_STRING(node,	"location",	cp->location->buf);
	if (cp->onEnter == NULL) {
		JSON_NULL(node, "enterFunction");
	} else {
		JSON_STRING(node, "enterFunction", cp->onEnter->buf);
	}

	return node;
}

static json_t *citybldg_toJson(const void *vcb)
{
	citybldg_t	*cb = (citybldg_t *)vcb;
	json_t		*node;

	node = json_object();
	JSON_STRING(node,	"distFace",	cb->dist_face->buf);
	JSON_STRING(node,	"nearFace",	cb->near_face->buf);
	JSON_STRING(node,	"enterFunction",cb->onEnter->buf);

	return node;
}

static btstring_t *citypath_toName(const void *vcp)
{
	citypath_t	*cp = (citypath_t *)vcp;

	return cp->label;
}

static btstring_t *citybldg_toName(const void *vcb)
{
	citybldg_t	*cb = (citybldg_t *)vcb;

	return cb->label;
}


/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

btcity_t *btcity_new(btstring_t *name)
{
	btcity_t *rval;

	rval = (btcity_t *)xzalloc(sizeof(btcity_t));

	rval->name = name;
	rval->sqs = cnvList_new(citypath_free, citypath_toJson,
				citypath_toName);
	rval->bls = cnvList_new(citybldg_free, citybldg_toJson,
				citybldg_toName);
	rval->day = citydata_new();
	rval->night = citydata_new();

	return rval;
}

void btcity_free(btcity_t *btc)
{
	if (btc == NULL) {
		return;
	}

	cnvList_free(btc->sqs);
	cnvList_free(btc->bls);
	citydata_free(btc->day);
	citydata_free(btc->night);

	bts_free(btc->name);
	bts_free(btc->title);
	free(btc);
}

void citypath_new(
	btcity_t *btc,
	btstring_t *label,
	btstring_t *n, btstring_t *s, btstring_t *e, btstring_t *w,
	btstring_t *o, btstring_t *l
	)
{
	citypath_t *cp;

	cp = (citypath_t *)xzalloc(sizeof(citypath_t));

	cp->label = label;
	cp->north = n;
	cp->south = s;
	cp->east = e;
	cp->west = w;
	cp->onEnter = o;
	cp->location = l;

	cnvList_add(btc->sqs, cp);
}

void citybldg_new(btcity_t *btc, btstring_t *l, btstring_t *d, btstring_t *n, btstring_t *o)
{
	citybldg_t *bl;

	bl = (citybldg_t *)xzalloc(sizeof(citybldg_t));

	bl->label = l;
	bl->dist_face = d;
	bl->near_face = n;
	bl->onEnter = o;

	cnvList_add(btc->bls, bl);

}

void btcity_to_json(btstring_t *fname, btcity_t *btc)
{
	json_t		*root;
	json_t		*node;

	root = json_object();

	JSON_BTSTRING(root, "title", btc->title);
	json_object_set_new(root, "squares", cnvList_toJsonObject(btc->sqs));
	json_object_set_new(root, "buildings", cnvList_toJsonObject(btc->bls));

	node = json_object();
	json_object_set_new(node, "items",
				cnvList_toJsonArray(btc->day->items));
	json_object_set_new(node, "monsters",
				cnvList_toJsonArray(btc->day->monsters));
	JSON_NUMBER(node, "poisonDamage", btc->day->poisonDmg);
	json_object_set_new(root, "day", node);

	node = json_object();
	json_object_set_new(node, "items",
				cnvList_toJsonArray(btc->night->items));
	json_object_set_new(node, "monsters",
				cnvList_toJsonArray(btc->night->monsters));
	JSON_NUMBER(node, "poisonDamage", btc->night->poisonDmg);
	json_object_set_new(root, "night", node);

	JSON_DUMP(root, fname);
}
