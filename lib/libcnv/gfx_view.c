#include <cnvlib.h>
#include <cnv_int.h>
#include <cnv_gfx.h>
#include <gl_xlist.h>
#include <gl_linked_list.h>

/****************************************/
/*					*/
/* Static variables			*/
/*					*/
/****************************************/

typedef struct {
	btstring_t	*label;
	gl_list_t	facets;
} viewquad_t;

typedef struct {
	btstring_t	*label;
	uint16_t	x;
	uint16_t	y;
	uint16_t	w;
	uint16_t	h;
} viewface_t;

struct bt_view_t {
	gl_list_t	quadList;
};

/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/

static bool quad_equals(const void *vq1, const void *vq2);
static size_t quad_hashcode(const void *vq);
static void quad_free(const void *vq);
static bool facet_equals(const void *vf1, const void *vf2);
static size_t facet_hashcode(const void *vf);
static void facet_free(const void *vf);

/****************************************/
/*					*/
/* Local functions			*/
/*					*/
/****************************************/

static size_t quad_hashcode(const void *vq)
{
	viewquad_t *quad = (viewquad_t *)vq;

	return bts_hashcode(quad->label);
}

static bool quad_equals(const void *vq1, const void *vq2)
{
	viewquad_t *quad;
	btstring_t *label;

	quad = (viewquad_t *)vq1;
	label = (btstring_t *)vq2;

	return (!strcmp(quad->label->buf, label->buf));
}

static void quad_free(const void *vq)
{
	viewquad_t *quad = (viewquad_t *)vq;

	if (quad == NULL)
		return;

	bts_free(quad->label);
	gl_list_free(quad->facets);
	free(quad);
}

static size_t facet_hashcode(const void *vf)
{
	viewface_t *facet = (viewface_t *)vf;

	return bts_hashcode(facet->label);
}

static bool facet_equals(const void *vf1, const void *vf2)
{
	viewface_t *f1 = (viewface_t *)vf1;
	viewface_t *f2 = (viewface_t *)vf2;

	return (!strcmp(f1->label->buf, f2->label->buf));
}
	

static void facet_free(const void *vf)
{
	viewface_t *facet = (viewface_t *)vf;

	if (facet == NULL)
		return;

	bts_free(facet->label);
	free(facet);
}

static viewquad_t *bt_view_new_quadrant(bt_view_t *view, btstring_t *label)
{
	viewquad_t *new;
	viewface_t sface;
	gl_list_node_t node;

	sface.label = label;

	node = gl_list_search(view->quadList, &sface);

	if (node == NULL) {
		new = (viewquad_t *)xzalloc(sizeof(viewquad_t));

		new->label = label;
		new->facets = gl_list_create_empty(GL_LINKED_LIST,
			facet_equals, facet_hashcode, facet_free, 0);

		gl_list_add_last(view->quadList, new);
	} else {
		bts_free(label);
		new = (viewquad_t *)gl_list_node_value(view->quadList, node);
	}

	return new;
}	

/****************************************/
/*					*/
/* Public functions			*/
/*					*/
/****************************************/

void bt_view_new_facet(bt_view_t *view, btstring_t *quad, btstring_t *label, \
			uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
	viewquad_t *q;
	viewface_t *f;

	q = bt_view_new_quadrant(view, quad);

	f = (viewface_t *)xzalloc(sizeof(viewface_t));
	f->label = label;
	f->x = x;
	f->y = y;
	f->w = w;
	f->h = h;

	gl_list_add_last(q->facets, f);
}


bt_view_t *bt_view_new(void)
{
	bt_view_t *rval;

	rval = xzalloc(sizeof(bt_view_t));
	rval->quadList = gl_list_create_empty(GL_LINKED_LIST,
		facet_equals, facet_hashcode, facet_free, 0);

	return rval;
}

void bt_view_free(bt_view_t *view)
{
	gl_list_free(view->quadList);
	free(view);
}

void bt_view_to_json(bt_view_t *view, btstring_t *path)
{
	viewquad_t *q;
	viewface_t *f;
	uint32_t i,j;
	json_t *root, *quad, *facet;
	gl_list_node_t 		qnode,	fnode;
	gl_list_iterator_t	qiter,	fiter;

	root = json_object();

	qiter = gl_list_iterator(view->quadList);
	while (gl_list_iterator_next(&qiter, (const void **)&q, &qnode)) {
		quad = json_object();

		fiter = gl_list_iterator(q->facets);
		while (gl_list_iterator_next(&fiter, (const void **)&f, &fnode)) {

			facet = json_object();

			JSON_NUMBER(facet, "x", f->x);
			JSON_NUMBER(facet, "y", f->y);
			JSON_NUMBER(facet, "w", f->w);
			JSON_NUMBER(facet, "h", f->h);

			json_object_set_new(quad, f->label->buf, facet);
		}

		json_object_set_new(root, q->label->buf, quad);
	}

	json_dump_file(root, path->buf, JSON_PRESERVE_ORDER | JSON_INDENT(2));
	json_decref(root);

	bts_free(path);
}
