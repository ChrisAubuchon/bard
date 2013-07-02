#include <btlib.h>
#include <l_int.h>


/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

static const char *_l_getDataPath(lua_State *L);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

static const char *_l_getDataPath(lua_State *L)
{
	int top;
	const char *datapath;

	top = lua_gettop(L);

	lua_getglobal(L, "sys");

	lua_getfield(L, top + 1, "dataPath");
	datapath = lua_tostring(L, top + 2);

	lua_settop(L, top);

	return datapath;
}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

btstring_t *_l_getFilePath(lua_State *L, uint8_t index)
{
	btstring_t *path;
	const char *fname;
	const char *datapath;


	datapath = _l_getDataPath(L);
	fname = luaL_checkstring(L, index);
	path = bts_sprintf("%s/%s", datapath, fname);
	
	return path;
}

FILE *_l_openFileFP(lua_State *L, uint8_t index, char *flags)
{
	FILE *fp;
	btstring_t *full;

	full = _l_getFilePath(L, index);

	fp = fopen(full->buf, flags);
	if (fp == NULL) {
		luaL_error(L, "Error opening file");
		bts_free(full);
		return NULL;
	}

	bts_free(full);

	return fp;
}

int l_var_readonly(lua_State *L)
{
	const char *key = luaL_checkstring(L, 3);

	luaL_error(L, "Field '%s' is read-only.\n", key);

	return 0;
}
