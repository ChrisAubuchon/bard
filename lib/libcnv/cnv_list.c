#include <cnv_list.h>
#include <gl_xlist.h>
#include <gl_array_list.h>

/********************************/
/*				*/
/* Internal structures		*/
/*				*/
/********************************/

struct cnvList_t {
	btFunc_free	free;
	btFunc_toJson	toJson;
	btFunc_toName	toName;
	gl_list_t	list;
};


/********************************/
/*				*/
/* Internal prototypes		*/
/*				*/
/********************************/

static json_t *bts_toJson(const void *vbs);
static btstring_t *bts_toName(const void *vbs);

/********************************/
/*				*/
/* Internal functions		*/
/*				*/
/********************************/

static json_t *bts_toJson(const void *vbs)
{
	btstring_t 	*bs = (btstring_t *)vbs;

	return json_string(bs->buf);
}

static btstring_t *bts_toName(const void *vbs)
{
	btstring_t	*bs = (btstring_t *)vbs;

	return bs;
}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

cnvList_t *cnvList_new(btFunc_free inFree, btFunc_toJson inToJson,
			btFunc_toName inToName)
{
	cnvList_t *cl;

	cl = (cnvList_t *)xzalloc(sizeof(cnvList_t));
	cl->free = inFree;
	cl->toJson = inToJson;
	cl->toName = inToName;
	cl->list = gl_list_create_empty(GL_ARRAY_LIST,
			NULL, NULL, inFree, 1);

	return cl;
}

void cnvList_free(cnvList_t *cl)
{
	if (cl == NULL)
		return;

	gl_list_free(cl->list);
	free(cl);
}

void cnvList_add(cnvList_t *cl, void *data)
{
	gl_list_add_last(cl->list, data);
}

json_t *cnvList_toJsonArray(cnvList_t *cl)
{
	json_t			*root;
	json_t			*recordRoot;
	gl_list_iterator_t	iter;
	gl_list_node_t		node;
	const void		*data;

	root = json_object();
	recordRoot = json_array();

	iter = gl_list_iterator(cl->list);
	while (gl_list_iterator_next(&iter, &data, &node)) {
		json_array_append_new(recordRoot, cl->toJson(data));
	}
	gl_list_iterator_free(&iter);

	json_object_set_new(root, "__records", recordRoot);

	return root;
}

json_t *cnvList_toJsonObject(cnvList_t *cl)
{
	json_t			*root;
	gl_list_iterator_t	iter;
	gl_list_node_t		node;
	const void		*data;

	root = json_object();
	iter = gl_list_iterator(cl->list);
	while (gl_list_iterator_next(&iter, &data, &node)) {
		json_object_set_new(root, cl->toName(data)->buf, 
					cl->toJson(data));
	}
	gl_list_iterator_free(&iter);

	return root;
}

void cnvList_runForEach(cnvList_t *cl, cnvList_forEach forEach)
{
	gl_list_iterator_t	iter;
	gl_list_node_t		node;
	const void		*data;

	iter = gl_list_iterator(cl->list);
	while (gl_list_iterator_next(&iter, &data, &node)) {
		forEach(data);
	}
	gl_list_iterator_free(&iter);
}

void cnvList_setToJson(cnvList_t *cl, btFunc_toJson inToJson)
{
	cl->toJson = inToJson;
}

void cnvList_setToName(cnvList_t *cl, btFunc_toName inToName)
{
	cl->toName = inToName;
}

btFunc_toJson cnvList_getToJson(cnvList_t *cl)
{
	return cl->toJson;
}

btFunc_toName cnvList_getToName(cnvList_t *cl)
{
	return cl->toName;
}

cnvList_t *cnvList_btstring(void)
{
	return cnvList_new(bts_free, bts_toJson, bts_toName);
}
