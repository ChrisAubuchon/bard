#ifndef _L_INT_H
#define _L_INT_H

/********************************/
/*				*/
/* Helper macros		*/
/*				*/
/********************************/


#define mod_constant_s(state, name, c_str) do { \
	lua_pushstring(state, name);		\
	lua_pushstring(state, c_str);		\
	lua_rawset(state, -3);			\
	} while(0)

#define mod_function(state, name, function) do {\
	lua_pushstring(state, name);		\
	lua_pushcfunction(state, function);	\
	lua_rawset(state, -3);			\
	} while(0)

#define mod_constant_n(state, name, c_num) do {	\
	lua_pushstring(state, name);		\
	lua_pushnumber(state, c_num);		\
	lua_rawset(state, -3);			\
	} while(0)

#define mod_begin_table(state, name) do {	\
	lua_pushstring(state, name);		\
	lua_newtable(state);			\
	} while (0)

#define mod_end_table(state, name) do {		\
	lua_rawset(L, -3);			\
	} while (0)

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

void	global_begin(lua_State *L, const char *name);
void	global_end(lua_State *L);

void	mod_begin(lua_State *L, const char *name);
void	mod_end(lua_State *L);

void	class_begin(lua_State *L, const char *name);
void	class_end(lua_State *L);

int	mod_index_handler(lua_State *L);
int	mod_newindex_handler(lua_State *L);
void	mod_variable(lua_State *L, const char *name, 
			lua_CFunction get, lua_CFunction set);

int	l_var_readonly(lua_State *L);

#endif
