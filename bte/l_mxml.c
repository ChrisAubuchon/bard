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

static mxml_node_t *my_mxmlFindPath(mxml_node_t *top, const char *path);

static mxml_node_t *l_checkMXML(lua_State *L, int index);
static mxml_node_t **l_checkMXMLP(lua_State *L, int index);
static int l_mxml_openfile(lua_State *L);
static int l_mxml_closefile(lua_State *L);
static int l_mxml_tostring(lua_State *L);

static int l_get_element(lua_State *L);
static int l_get_type(lua_State *L);
static int l_get_value(lua_State *L);
static int l_set_value(lua_State *L);
static int l_get_name(lua_State *L);
static int l_set_name(lua_State *L);

static int l_mxml_getnextsibling(lua_State *L);
static int l_mxml_haschildren(lua_State *L);
static int l_mxml_getfirstchild(lua_State *L);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

/*
 * The mxml library's mxmlFindPath returns the first child
 * if the first child is not an MXML_ELEMENT.  To me this is
 * counter intuitive to how I think it should work. I think it
 * should always return the found node.
 */
static mxml_node_t *my_mxmlFindPath(mxml_node_t *top, const char *path)
{
	mxml_node_t *node;
	char element[256];
	const char *pathsep;
	int descend;

	if (!top || !path || !*path)
		return NULL;

	node = top;
	while (*path) {
		if (!strncmp(path, "*/", 2)) {
			path += 2;
			descend = MXML_DESCEND;
		} else {
			descend = MXML_DESCEND_FIRST;
		}

		if ((pathsep = strchr(path, '/')) == NULL)
			pathsep = path + strlen(path);

		if (pathsep == path || (pathsep - path) >= sizeof(element))
			return NULL;

		memcpy(element, path, pathsep - path);
		element[pathsep - path] = '\0';

		if (*pathsep)
			path = pathsep + 1;
		else
			path = pathsep;

		if ((node = mxmlFindElement(node, node, element, NULL, NULL,
					    descend)) == NULL)
			return (NULL);
	}

	return (node);
}

static int l_mxml_tostring(lua_State *L)
{
	mxml_node_t *node = l_checkMXML(L, 1);

	lua_pushfstring(L, "mxml_node: %p", node);

	return 1;
}

static int l_get_element(lua_State *L)
{
	mxml_node_t *node = l_checkMXML(L, 1);

	lua_pushstring(L, mxmlGetElement(node));

	return 1;
}

static int l_get_type(lua_State *L)
{
	mxml_node_t *node = l_checkMXML(L, 1);
	mxml_type_t type;

	lua_pushnumber(L, mxmlGetType(node));
/*
	type = mxmlGetType(node);

	switch (type) {
	case MXML_CUSTOM:
		lua_pushstring(L, "MXML_CUSTOM");
		break;
	case MXML_ELEMENT:
		lua_pushstring(L, "MXML_ELEMENT");
		break;
	case MXML_IGNORE:
		lua_pushstring(L, "MXML_IGNORE");
		break;
	case MXML_INTEGER:
		lua_pushstring(L, "MXML_INTEGER");
		break;
	case MXML_OPAQUE:
		lua_pushstring(L, "MXML_OPAQUE");
		break;
	case MXML_REAL:
		lua_pushstring(L, "MXML_REAL");
		break;
	case MXML_TEXT:
		lua_pushstring(L, "MXML_TEXT");
		break;
	}
*/

	return 1;
}

static int l_get_value(lua_State *L)
{
	mxml_node_t *node = l_checkMXML(L, 1);

	lua_pushstring(L, mxmlGetOpaque(node));
	if (lua_isnumber(L, -1)) {
		double num;

		num = luaL_checknumber(L, -1);
		lua_pop(L, 1);
		lua_pushnumber(L, num);
	}

	return 1;
}

static int l_set_value(lua_State *L)
{
	mxml_node_t *node = l_checkMXML(L, 1);
	const char *data = luaL_checkstring(L, 2);

	mxmlSetOpaque(node, data);

	return 0;
}

static int l_get_name(lua_State *L)
{
	mxml_node_t *node = l_checkMXML(L, 1);

	lua_pushstring(L, mxmlGetElement(node));

	return 1;
}

static int l_set_name(lua_State *L)
{
	mxml_node_t *node = l_checkMXML(L, 1);
	const char *name = luaL_checkstring(L, 2);

	mxmlSetElement(node, name);

	return 0;
}

static mxml_node_t **_l_new_node(lua_State *L)
{
	mxml_node_t **node;

	node = lua_newuserdata(L, sizeof(mxml_node_t *));
	L_SETMETATABLE(L, "l_mxml_node");

	return node;
}

static mxml_node_t *l_checkMXML(lua_State *L, int index)
{
	return *(mxml_node_t **)luaL_checkudata(L, index, "l_mxml_node");
}

static mxml_node_t **l_checkMXMLP(lua_State *L, int index)
{
	return (mxml_node_t **)luaL_checkudata(L, index, "l_mxml_node");
}

static int l_mxml_openfile(lua_State *L)
{
	const char *top;
	mxml_node_t **node;
	FILE *fp;

	fp = _l_openFileFP(L, 1, "r");

	node = _l_new_node(L);

	*node = mxmlLoadFile(NULL, fp, MXML_OPAQUE_CALLBACK);
	if (*node == NULL)
		luaL_error(L, "mxml_error: mxmlLoadFile error");

	fclose(fp);

	return 1;
}

static int l_mxml_closefile(lua_State *L)
{
	mxml_node_t *tree;

	tree = l_checkMXML(L, 1);

	mxmlDelete(tree);
}

static int l_mxml_findpath(lua_State *L)
{
	mxml_node_t **rval;
	mxml_node_t *tree;
	const char *path;

	tree = l_checkMXML(L, 1);
	path = luaL_checkstring(L, 2);

	rval = _l_new_node(L);
	*rval = my_mxmlFindPath(tree, path);

	return 1;
}

static int l_mxml_write(lua_State *L)
{
	FILE *fp;
	mxml_node_t *node;

	node = l_checkMXML(L, 1);
	fp = _l_openFileFP(L, 2, "w");

	mxmlSaveFile(node, fp, MXML_NO_CALLBACK);

	fclose(fp);

	return 0;
}

static int next_iter(lua_State *L)
{
	mxml_node_t **parent;
	mxml_node_t **child;
	mxml_node_t **sib;

	child = (mxml_node_t **)lua_touserdata(L, lua_upvalueindex(2));

	/* First time through child == NULL */
	if (child == NULL) {
		parent = l_checkMXMLP(L, lua_upvalueindex(1));

		child = _l_new_node(L);

		*child = mxmlGetFirstChild(*parent);
		if (mxmlGetType(*child) == MXML_ELEMENT) {
			lua_pushvalue(L, -1);
			lua_replace(L, lua_upvalueindex(2));

			return 1;
		}
	}

	do {
		*child = mxmlGetNextSibling(*child);
	} while ((*child) && (mxmlGetType(*child) != MXML_ELEMENT));

	if (!*child) {
		lua_pop(L, 1);
		return 0;
	}

	lua_pushvalue(L, -1);
	lua_replace(L, lua_upvalueindex(2));

	return 1;

}

static int l_mxml_getnextsibling(lua_State *L)
{
	lua_pushnil(L);
	lua_pushcclosure(L, next_iter, 2);

	return 1;
}

static int l_mxml_haschildren(lua_State *L)
{
	mxml_node_t *node;

	node = l_checkMXML(L, 1);

	if (mxmlGetFirstChild(node) != NULL)
		lua_pushnumber(L, 1);
	else
		lua_pushnumber(L, 0);

	return 1;
}

static int l_mxml_getfirstchild(lua_State *L)
{
	mxml_node_t *node;
	mxml_node_t **rval;

	node = l_checkMXML(L, 1);

	if (mxmlGetFirstChild(node) != NULL) {
		rval = _l_new_node(L);
		*rval = mxmlGetFirstChild(node);
	} else {
		lua_pushnil(L);
	}

	return 1;
}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/


void l_mxml_open(lua_State *L)
{
	class_begin(L, "l_mxml_node");
/*	mod_function(L, "__gc",			l_mxml_closefile);*/
	mod_function(L, "__tostring",		l_mxml_tostring);
	mod_function(L, "Close",		l_mxml_closefile);
	mod_function(L, "FindPath",		l_mxml_findpath);
	mod_function(L, "Write",		l_mxml_write);
	mod_function(L, "GetNextSibling",	l_mxml_getnextsibling);
	mod_function(L, "GetFirstChild",	l_mxml_getfirstchild);
	mod_function(L, "HasChildren",		l_mxml_haschildren);
	mod_variable(L, "value",		l_get_value, 	l_set_value);
	mod_variable(L, "name",			l_get_name,	l_set_name);
	mod_variable(L, "element",		l_get_element,	NULL);
	mod_variable(L, "type",			l_get_type,	NULL);
	class_end(L);

	lua_newtable(L);

	mod_function(L,	"Open",		l_mxml_openfile);

	mod_constant(L, "MXML_CUSTOM",		MXML_CUSTOM);
	mod_constant(L, "MXML_ELEMENT",		MXML_ELEMENT);
	mod_constant(L, "MXML_IGNORE",		MXML_IGNORE);
	mod_constant(L, "MXML_INTEGER",		MXML_INTEGER);
	mod_constant(L, "MXML_OPAQUE",		MXML_OPAQUE);
	mod_constant(L, "MXML_REAL",		MXML_REAL);
	mod_constant(L, "MXML_TEXT",		MXML_TEXT);

	lua_setglobal(L, "mxml");
}
