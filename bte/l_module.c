#include <btlib.h>
#include <l_int.h>

#define DEBUG 1
#include <debug.h>

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

void mod_function(lua_State *L, const char *name, lua_CFunction func)
{
	lua_pushstring(L, name);
	lua_pushcfunction(L, func);
	lua_rawset(L, -3);
}

void mod_constant(lua_State *L, const char *name, lua_Number value)
{
	lua_pushstring(L, name);
	lua_pushnumber(L, value);
	lua_rawset(L, -3);
}

/*
 * mod_variable()
 *
 * Stack at entry:
 * ... mt
 */
void mod_variable(lua_State *L, const char *name, \
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

/*
 * mod_index_handler()
 * Stack upon entry:
 * object key
 */
int mod_index_handler(lua_State *L)
{
	if (lua_type(L, 1) != LUA_TUSERDATA) {
		lua_pushnil(L);
		return 1;
	}

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


	/* Not a .get member. Check the metatable */
	lua_settop(L, 3);
	lua_pushvalue(L, 2);
	lua_gettable(L, -2);

	return 1;
}

/*
 * mod_newindex_handler()
 * 
 * Stack at entry:
 * object key value
 */
int mod_newindex_handler(lua_State *L)
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
			lua_pushvalue(L, 2);
			lua_call(L, 3, 0);
			return 0;
		}
	}

	lua_settop(L, 4);
	lua_pushvalue(L, 2);
	lua_pushvalue(L, 3);
	lua_settable(L, -3);

	return 0;
}


void class_begin(lua_State *L, const char *name)
{
	luaL_newmetatable(L, name);

	L_REGFUNC(L, "__index", mod_index_handler);
	L_REGFUNC(L, "__newindex", mod_newindex_handler);
}

void class_end(lua_State *L)
{
	lua_pop(L, 1);
}

void global_begin(lua_State *L, const char *name)
{
	/* Create the global table ... */
	lua_newtable(L);
	lua_setglobal(L, name);

	/* and put it at the top of the stack */
	lua_getglobal(L, name);
}

void global_end(lua_State *L)
{
	/* Pop the table off of the stack */
	lua_pop(L, 1);
}

void mod_begin(lua_State *L, const char *name)
{
	lua_pushstring(L, name);
	lua_newtable(L);
}

void mod_end(lua_State *L)
{
	lua_rawset(L, -3);
}
