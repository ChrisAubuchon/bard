#include <btlib.h>
#include <l_int.h>
#include <jansson.h>


/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

/* Helper functions */
static void json_to_lua(lua_State *L, json_t *j);
static void lua_to_json(lua_State *L, json_t *j);
static json_t *_lua_value(lua_State *L, int index);
static void _json_value(lua_State *L, json_t *j);

/* Module functions */
static int l_json_to_lua(lua_State *L);
static int l_lua_to_json(lua_State *L);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

static void json_to_lua(lua_State *L, json_t *j)
{
	const char *key;
	json_t *value;

	if (j == NULL)
		return;

	if (json_is_array(j)) {
		int i;

		for (i = 0; i < json_array_size(j); i++) {
			lua_pushnumber(L, i + 1);

			_json_value(L, json_array_get(j, i));

			lua_rawset(L, -3);
		}
	} else if (json_is_object(j)) {
		json_object_foreach(j, key, value) {
			if (!strcmp(key, "__records")) {
				json_to_lua(L, value);
			} else {
				lua_pushstring(L, key);

				_json_value(L, value);

				lua_rawset(L, -3);
			}
		}
	}
}

static void _json_value(lua_State *L, json_t *j)
{
	if (json_is_object(j)) {
		lua_newtable(L);

		json_to_lua(L, j);
	} else if (json_is_string(j)) {
		lua_pushstring(L, json_string_value(j));
		/* 
		 * If the value can be converted to a number,
		 * convert it to a number
		 */
		if (lua_isnumber(L, -1)) {
			lua_Number num;

			num = luaL_checknumber(L, -1);
			lua_pop(L, 1);
			lua_pushnumber(L, num);
		}
	} else if (json_is_number(j)) {
		lua_pushnumber(L, json_number_value(j));
	} else if (json_is_array(j)) {
		json_to_lua(L, j);
	} else if (json_is_true(j)) {
		lua_pushboolean(L, 1);
	} else if (json_is_false(j)) {
		lua_pushboolean(L, 0);
	} else if (json_is_null(j)) {
		lua_pushnil(L);
	} else {
		luaL_error(L, "Invalid JSON type");
	}
}


static json_t *_lua_value(lua_State *L, int index)
{
	switch (lua_type(L, index)) {
	case LUA_TSTRING:
		return json_string(lua_tostring(L, -1));
	case LUA_TNUMBER:
		return json_integer(lua_tonumber(L, -1));
	case LUA_TTABLE:
	{
		json_t *rval;

		rval = json_object();
		lua_to_json(L, rval);

		return rval;
	}
	case LUA_TBOOLEAN:
	{
		int b = lua_toboolean(L, -1);

		if (b) 
			return json_true();
		else
			return json_false();
	}
	case LUA_TNIL:
		return json_null();
	default:
		luaL_error(L, "Not implemented: Type %d", lua_type(L, index));
	}
}

static void lua_to_json(lua_State *L, json_t *j)
{
	json_t *value;
	const char *key;

	int top = lua_gettop(L);
	
	lua_pushnil(L);
	while (lua_next(L, top) != 0) {
		/*
		 * -2: Table key
		 * -1: Value
		 */

		if (lua_type(L, -2) == LUA_TNUMBER) {
			json_t *records;
			int index = lua_tonumber(L, -2) - 1;

			records = json_object_get(j, "__records");
			if (records == NULL) {
				records = json_array();

				json_object_set(j, "__records", records);
			}

			value = _lua_value(L, -1);

			json_array_append_new(records, value);
		} else if (lua_type(L, -2) == LUA_TSTRING) {
			key = lua_tostring(L, -2);

			value = _lua_value(L, -1);

			json_object_set_new_nocheck(j, key, value);
		}

		lua_pop(L, 1);

	}
}

static int l_json_to_lua(lua_State *L)
{
	FILE *fp;
	json_t *j;
	json_error_t error;

	fp = _l_openFileFP(L, 1, "r");
	if (fp == NULL)
		luaL_error(L, "Error opening file");

	/* Return an empty table if the JSON file is empty */
	fseek(fp, 0, SEEK_END);
	if (ftell(fp) == 0) {
		lua_newtable(L);
		return 1;
	}
	fseek(fp, 0, SEEK_SET);

	j = json_loadf(fp, 0, &error);
	if (j == NULL)
		luaL_error(L, error.text);

	/* Create the base table */
	lua_newtable(L);

	json_to_lua(L, j);

	fclose(fp);

	json_decref(j);

	return 1;
}

static int l_lua_to_json(lua_State *L)
{
	FILE *fp;
	json_t *j;

	if (!lua_istable(L, 1))
		luaL_error(L, "lua2json: Arg #1 must be a table");


	fp = _l_openFileFP(L, 2, "w");
	if (fp == NULL)
		luaL_error(L, "Error opening file");

	/* Remove the filename from the stack */
	lua_pop(L, 1);

	/* Create the root JSON object */
	j = json_object();

	/* Convert a Lua table to a json_t object */
	lua_to_json(L, j);

	/* Output the file */
	json_dumpf(j, fp, JSON_INDENT(2) | JSON_PRESERVE_ORDER);

	/* Free memory */
	json_decref(j);

	fclose(fp);

	return 0;
}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/


void l_jansson_open(lua_State *L)
{
	global_begin(L, "jansson");
	mod_function(L,	"json2lua",	l_json_to_lua);
	mod_function(L, "lua2json",	l_lua_to_json);
	global_end(L);
}
