#include <btlib.h>
#include <l_int.h>


/********************************/
/*				*/
/* Static function prototypes	*/
/*				*/
/********************************/


/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/


void class_begin(lua_State *L, const char *name)
{
	luaL_newmetatable(L, name);
	L_REGFUNC(L,	"__index",	class_index_handler);
}

void class_end(lua_State *L)
{
	lua_pop(L, 1);
}

void class_variable(lua_State *L, const char *name, \
		    lua_CFunction get, lua_CFunction set)
		  lua_CFunction get, lua_CFunction set)
{
	lua_pushstring(L, ".get");

	/* stack: obj mt ".get" */
	lua_rawget(L, -2);
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		lua_newtable(L);
		lua_pushstring(L, ".get");
		lua_pushvalue(L, -2);
		lua_rawset(L, -4);
	}

	/* stack: obj mt gett */
	lua_pushstring(L, name);
	lua_pushcfunction(L, get);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	if (set) {
		lua_pushstring(L, ".set");

		lua_rawget(L, -2);
		if (!lua_istable(L, -1)) {
			lua_pop(L, 1);
			lua_newtable(L);
			lua_pushstring(L, ".set");
			lua_pushvalue(L, -2);
			lua_rawset(L, -4);
		}

		/* stack: obj mt sett */
		lua_pushstring(L, name);
		lua_pushcfunction(L, set);
		lua_rawset(L, -3);
		lua_pop(L, 1);
	}

			
}

void class_function(lua_State *L, const char *name, lua_CFunction f)
{
	lua_pushstring(L, name);
	lua_pushcfunction(L, f);
	lua_rawset(L, -3);
}

/*
 * mod_index_handler()
 * Stack upon entry:
 * object key
 */
int class_index_handler(lua_State *L)
{
/*
	if (lua_type(L, 1) != LUA_TUSERDATA) {
		lua_pushnil(L);
		return 1;
	}
*/

	L_GETMETATABLE(L, 1);

	/* stack: object key */
	lua_pushstring(L,".get");	/* stack: obj key mt ".get" */
	lua_rawget(L, -2);		/* stack: obj key mt gett */

	/* stack: object key gett */
	if (lua_istable(L, -1)) {
		lua_pushvalue(L, 2);	/* stack: obj key gett key */

		lua_rawget(L, -2);	/* stack: obj key gett func */
		lua_remove(L, -2);		/* stack: obj key func */
		if (lua_iscfunction(L, -1)) {
			lua_pushvalue(L, 1);
			lua_call(L, 1, 1);
			return 1;
		}
	}

	

	lua_pushnil(L);
	return 1;
}

/*
 * class_newindex_handler()
 * 
 * Stack at entry:
 * object key value
 */
int class_newindex_handler(lua_State *L)
{
	if (lua_type(L, 1) != LUA_TUSERDATA) 
		return 0;

	L_GETMETATABLE(L, 1);

	lua_pushstring(L, ".set");
	lua_rawget(L, -2);		/* stack: obj key value sett */

	if (lua_istable(L, -1)) {
		lua_pushvalue(L, 2);	/* stack: obj key v sett key */

		lua_rawget(L, -2);	/* stack: obj key v sett func */
		lua_remove(L, -2);	/* stack: obj key v func */
		if (lua_iscfunction(L, -1)) {
			lua_pushvalue(L, 1);
			lua_pushvalue(L, 3);
			lua_call(L, 2, 0);
			return 0;
		}
	}

	return 0;
}

