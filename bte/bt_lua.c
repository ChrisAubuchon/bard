#include <bte.h>


/********************************/
/*				*/
/* Static variables		*/
/*				*/
/********************************/

static lua_State *global_state = NULL;


/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/


/*
 * bt_lua_state()
 *
 * Initialize LUA
 */
lua_State *bt_lua_state(void)
{
	if (global_state == NULL) {
		global_state = luaL_newstate();

		luaL_openlibs(global_state);
		l_jansson_open(global_state);
		l_sys_open(global_state);
		l_sdl_open(global_state);
		l_random_open(global_state);
		l_sprintf_open(global_state);
	}

	return global_state;
}

/*
 * bt_lua_doFile()
 */
void bt_lua_doFile(lua_State *L, uint8_t *script)
{
	int error;

	error = luaL_loadfile(L, script) || lua_pcall(L, 0, 0, 0);
	if (error) {
		fprintf(stderr, "%s", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}
