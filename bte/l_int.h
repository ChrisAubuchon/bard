#ifndef _L_INT_H
#define _L_INT_H

#include <btlib.h>
#include <xalloc.h>
#include <lua.h>
#include <lauxlib.h>


/********************************/
/*				*/
/* Helper macros		*/
/*				*/
/********************************/

#define L_REGSTRING(state, name, c_str) { \
	lua_pushstring(state, name);		\
	lua_pushstring(state, c_str);		\
	lua_rawset(state, -3);			\
	}

#define L_REGFUNC(state, name, function) { \
	lua_pushstring(state, name);		\
	lua_pushcfunction(state, function);	\
	lua_rawset(state, -3);			\
	}

#define L_REGNUMBER(state, name, c_num) {	\
	lua_pushstring(state, name);		\
	lua_pushnumber(state, c_num);		\
	lua_rawset(state, -3);			\
	}

#define L_GETMETATABLE(state, index) {		\
	lua_pushvalue(state, index);		\
	lua_getmetatable(state, -1);		\
	lua_remove(state, -2);			\
	}

#define L_SETMETATABLE(state, type) {		\
	luaL_getmetatable(state, type);		\
	lua_setmetatable(state, -2);		\
	}

#define L_NEWUSERDATA(state, dest, size, meta) {	\
	dest = lua_newuserdata(state, size);		\
	luaL_getmetatable(state, meta);			\
	lua_setmetatable(state, -2);			\
}
	


/********************************/
/*				*/
/* library open functions	*/
/*				*/
/********************************/

btstring_t *_l_getFilePath(lua_State *L, uint8_t index);
FILE *_l_openFileFP(lua_State *L, uint8_t index, char *flags);

void l_video_open(lua_State *L);
void l_xml_open(lua_State *L);
void l_sys_open(lua_State *L);

void global_begin(lua_State *L, const char *name);
void global_end(lua_State *L);
void mod_begin(lua_State *L, const char *name);
void mod_end(lua_State *L);
void class_begin(lua_State *L, const char *name);
void class_end(lua_State *L);
int mod_index_handler(lua_State *L);
int mod_newindex_handler(lua_State *L);
void mod_constant(lua_State *L, const char *name, lua_Number value);
void mod_variable(lua_State *L, const char *name, 
			lua_CFunction get, lua_CFunction set);
void mod_function(lua_State *L, const char *name, 
			lua_CFunction func);

int l_var_readonly(lua_State *L);

#endif
