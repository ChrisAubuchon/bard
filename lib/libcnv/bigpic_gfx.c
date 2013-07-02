#include <cnvlib.h>
#include <cnv_int.h>
#include <cnv_gfx.h>
#include <jansson.h>
#include <gl_xlist.h>
#include <gl_array_list.h>

/****************************************/
/*					*/
/* Static variables			*/
/*					*/
/****************************************/

struct bt_bigpicList_t {
	gl_list_t	bplist;
};

typedef struct {
	btstring_t *name;
	btstring_t *path;
	uint8_t type;
} bigpic_t;

/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/

static void _bigpic_free(const void *vbp);


/****************************************/
/*					*/
/* Local functions			*/
/*					*/
/****************************************/

static void _bigpic_free(const void *vbp)
{
	bigpic_t *bp = (bigpic_t *)vbp;

	if (bp) {
		bts_free(bp->name);
		bts_free(bp->path);
		free(bp);
	}
}

/****************************************/
/*					*/
/* Public functions			*/
/*					*/
/****************************************/


bt_bigpicList_t *bigpic_list_new(uint32_t size)
{
	bt_bigpicList_t *new;
	void *data;

	new = (bt_bigpicList_t *)xzalloc(sizeof(bt_bigpicList_t));
	new->bplist = gl_list_create_empty(GL_ARRAY_LIST,
		NULL, NULL, _bigpic_free, 1);

	return new;
}

void bigpic_list_free(bt_bigpicList_t *bpl)
{
	if (bpl == NULL) {
		return;
	}
	gl_list_free(bpl->bplist);
	free(bpl);
}

void bigpic_list_set(bt_bigpicList_t *bpl, btstring_t *name, \
		     btstring_t *path, uint8_t type)
{
	bigpic_t *new;

	new = (bigpic_t *)xzalloc(sizeof(bigpic_t));
	new->name = name;
	new->path = path;
	new->type = type;

	gl_list_add_last(bpl->bplist, new);
}

void bigpic_list_to_json(bt_bigpicList_t *bpl, btstring_t *fname)
{
	uint32_t i;
	bigpic_t *bp;
	json_t *root, *js;

	/* Create the base json object */
	root = json_object();

	for (i = 0; i < gl_list_size(bpl->bplist); i++) {
		bp = (bigpic_t *)gl_list_get_at(bpl->bplist, i);

		js = json_object();
		json_object_set_new(js, "path", json_string(bp->path->buf));

		switch (bp->type) {
		case BTA_PNG:
			json_object_set_new(js, "type", json_string("png"));
			break;
		case BTA_TYPE_SIMPLELOOP:
		case BTA_TYPE_XORLOOP:
			json_object_set_new(js, "type", json_string("bta"));
			break;
		default:
			printf("Invalid bigpic type: %d\n", bp->type);
			exit(1);
		}

		json_object_set(root, bp->name->buf, js);
	}

	json_dump_file(root, fname->buf, JSON_PRESERVE_ORDER | JSON_INDENT(2));
	json_decref(root);

	bts_free(fname);
}
