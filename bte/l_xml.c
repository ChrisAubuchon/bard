#include <l_int.h>
#include <mxml.h>
#include <bt_gds.h>


/********************************/
/*				*/
/* Private variables		*/
/*				*/
/********************************/

static bt_stack_t *nodetypes;

/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

static int l_xml2lua(lua_State *L);
static mxml_type_t bt_mxml_read_cb(mxml_node_t *node);
static void bt_sax_cb(mxml_node_t *node, mxml_sax_event_t event, void *data);
static mxml_type_t _xml_get_node_type(mxml_node_t *node);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

static void bt_sax_cb(mxml_node_t *node, mxml_sax_event_t event, void *data)
{
	lua_State *L;
	uint32_t nodetype;

	L = (lua_State *)data;

	if (event == MXML_SAX_ELEMENT_OPEN) {
		nodetype = bt_mxml_read_cb(node);

		bt_stack_push(nodetypes, (void *)nodetype);
		
		if (nodetype == MXML_IGNORE) 
			return;

		lua_pushstring(L, mxmlGetElement(node));
		if (nodetype == MXML_ELEMENT) {
			lua_newtable(L);
		}
	} else if (event == MXML_SAX_ELEMENT_CLOSE) {
		nodetype = (uint32_t)bt_stack_pop(nodetypes);

		if (nodetype != MXML_IGNORE) {
			lua_rawset(L, -3); 
		}
	} else if (event == MXML_SAX_DATA) {
		nodetype = (uint32_t)bt_stack_top(nodetypes);

		switch (nodetype) {
			case MXML_OPAQUE:
				lua_pushstring(L, mxmlGetOpaque(node));
				break;
			case MXML_INTEGER:
				lua_pushnumber(L, mxmlGetInteger(node));
				break;
		}
	}
}

static mxml_type_t bt_mxml_read_cb(mxml_node_t *node)
{
	const char *type;

	type = mxmlElementGetAttr(node, "type");
	if (type == NULL) 
		return MXML_IGNORE;

	if (!strcmp(type, "table")) 
		return MXML_ELEMENT;
	else if (!strcmp(type, "integer"))
		return MXML_INTEGER;
	else if (!strcmp(type, "string"))
		return MXML_OPAQUE;
	else
		return MXML_IGNORE;
}


static int l_xml2lua(lua_State *L)
{
	FILE *fp;
	mxml_node_t *tree;


	fp = _l_openFileFP(L, 1);

	lua_newtable(L);

	nodetypes = bt_stack_new();
	tree = mxmlSAXLoadFile(NULL, fp, bt_mxml_read_cb, bt_sax_cb, L);
	mxmlDelete(tree);
	bt_stack_free(nodetypes);

	fclose(fp);

	return 1;
}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/


void l_xml_open(lua_State *L)
{
	lua_newtable(L);

	L_REGFUNC(L, "xml2lua",		l_xml2lua);

	lua_setglobal(L, "xml");
}
