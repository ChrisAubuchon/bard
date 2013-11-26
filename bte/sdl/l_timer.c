#include <bte.h>
#include <l_sdl.h>

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/


/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

/* Meta-methods */

/* Methods */
static int l_add_timer(lua_State *L);
static int l_remove_timer(lua_State *L);
static int l_sleep(lua_State *L);

/* Member access functions */

/* Internal functions */
static uint32_t luaCallback(uint32_t interval, void *param);
static uint32_t sleepCallback(uint32_t interval, void *param);


/********************************/
/*				*/
/* Internal functions		*/
/*				*/
/********************************/

static uint32_t luaCallback(uint32_t interval, void *param)
{
	SDL_Event e;
	SDL_UserEvent u;

	u.type = BT_CB_EVENT;
	u.data1 = param;
	u.data2 = NULL;

	e.type = BT_CB_EVENT;
	e.user = u;

	SDL_PushEvent(&e);

	return interval;
}

static uint32_t sleepCallback(uint32_t interval, void *param)
{
	SDL_Event e;
	SDL_UserEvent u;

	u.type = BT_SLEEP_EVENT;
	u.data1 = param;
	u.data2 = NULL;

	e.type = BT_SLEEP_EVENT;
	e.user = u;

	SDL_PushEvent(&e);

	return 0;
}

/********************************/
/*				*/
/* Lua functions		*/
/*				*/
/********************************/

static int l_sleep(lua_State *L)
{
	uint32_t interval;
	SDL_TimerID *id;

	interval = (uint32_t)luaL_checkinteger(L, 1);

	L_NEWUSERDATA(L, id, sizeof(SDL_TimerID), "l_sdl_timer");
	*id = SDL_AddTimer(interval, sleepCallback, NULL);

	return 1;
}

static int l_add_timer(lua_State *L)
{
	int ref;
	uint32_t interval;
	SDL_TimerID *id;

	if (!lua_isfunction(L, 1)) 
		luaL_error(L, "Must pass a function to AddTimer");
	interval = (uint32_t)luaL_checkinteger(L, 2);

	lua_pop(L, 1);
	ref = luaL_ref(L, LUA_REGISTRYINDEX);

	L_NEWUSERDATA(L, id, sizeof(SDL_TimerID), "l_sdl_timer");
	*id = SDL_AddTimer(interval, luaCallback, (void *)ref);

	return 1;
}

static int l_remove_timer(lua_State *L)
{
	SDL_TimerID *id;

	id = (SDL_TimerID *)luaL_checkudata(L, 1, "l_sdl_timer");

	SDL_RemoveTimer(*id);

	return 0;
}

void l_sdl_timer_open(lua_State *L)
{
	class_begin(L, "l_sdl_timer");
	class_end(L);

	mod_function(L, "AddTimer",	l_add_timer);
	mod_function(L, "RemoveTimer",	l_remove_timer);
	mod_function(L, "Sleep",	l_sleep);
}
