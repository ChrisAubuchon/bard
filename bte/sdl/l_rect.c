#include <bte.h>
#include <l_sdl.h>

#define DEBUG 1
#include <debug.h>

/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

/* Member access functions */
static int sdl_rect_get_x(lua_State *L);
static int sdl_rect_get_y(lua_State *L);
static int sdl_rect_get_w(lua_State *L);
static int sdl_rect_get_h(lua_State *L);
static int sdl_rect_set_x(lua_State *L);
static int sdl_rect_set_y(lua_State *L);
static int sdl_rect_set_w(lua_State *L);
static int sdl_rect_set_h(lua_State *L);

static int l_new_rect(lua_State *L);
static int l_rect_tostring(lua_State *L);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

static int sdl_rect_get_x(lua_State *L)
{
	SDL_Rect	*r;

	r = l_checkRect(L, 1);

	lua_pushnumber(L, r->x);
	return 1;
}

static int sdl_rect_get_y(lua_State *L)
{
	SDL_Rect	*r;

	r = l_checkRect(L, 1);

	lua_pushnumber(L, r->y);
	return 1;
}

static int sdl_rect_get_w(lua_State *L)
{
	SDL_Rect	*r;

	r = l_checkRect(L, 1);

	lua_pushnumber(L, r->w);
	return 1;
}

static int sdl_rect_get_h(lua_State *L)
{
	SDL_Rect	*r;

	r = l_checkRect(L, 1);

	lua_pushnumber(L, r->h);
	return 1;
}

static int sdl_rect_set_x(lua_State *L)
{
	SDL_Rect	*r;
	uint8_t		val;

	r = l_checkRect(L, 1);
	val = (uint8_t)luaL_checknumber(L, 2);

	r->x = val;

	return 0;
}

static int sdl_rect_set_y(lua_State *L)
{
	SDL_Rect	*r;
	uint8_t		val;

	r = l_checkRect(L, 1);
	val = (uint8_t)luaL_checknumber(L, 2);

	r->y = val;

	return 0;
}

static int sdl_rect_set_w(lua_State *L)
{
	SDL_Rect	*r;
	uint8_t		val;

	r = l_checkRect(L, 1);
	val = (uint8_t)luaL_checknumber(L, 2);

	r->w = val;

	return 0;
}

static int sdl_rect_set_h(lua_State *L)
{
	SDL_Rect	*r;
	uint8_t		val;

	r = l_checkRect(L, 1);
	val = (uint8_t)luaL_checknumber(L, 2);

	r->h = val;

	return 0;
}

static int l_new_rect(lua_State *L)
{
	SDL_Rect	*r;
	int		index = 1;

	l_rect_new(L);
	r = l_checkRect(L, -1);

	/* Handle the Rect:new() case */
	if (lua_istable(L, 1))
		index = 2;


	r->x = (int16_t)luaL_checkinteger(L, index++);
	r->y = (int16_t)luaL_checkinteger(L, index++);
	r->w = (uint16_t)luaL_checkinteger(L, index++);
	r->h = (uint16_t)luaL_checkinteger(L, index++);

	return 1;
}

static int l_rect_tostring(lua_State *L)
{
	SDL_Rect	*r;

	r = l_checkRect(L, 1);

	lua_pushfstring(L, "x: %d, y: %d, w: %d, h: %d\n",
			r->x, r->y, r->w, r->h);

	return 1;
}



static SDL_Rect *sdl_new_rect(int x, int y, int w, int h)
{
	SDL_Rect *r;

	r = (SDL_Rect *)xzalloc(sizeof(SDL_Rect));

	r->x = x;
	r->y = y;
	r->w = w;
	r->h = h;

	return r;
}

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

/*
 * l_rect_new()
 */
int l_rect_new(lua_State *L)
{
	lua_newuserdata(L, sizeof(SDL_Rect));
	luaL_getmetatable(L, "l_sdl_rect");
	lua_setmetatable(L, -2);

	return 1;
}

/*
 * l_testRect()
 */
SDL_Rect *l_testRect(lua_State *L, int index)
{
	return (SDL_Rect *)luaL_testudata(L, index, "l_sdl_rect");
}

/*
 * l_checkRect()
 */
SDL_Rect *l_checkRect(lua_State *L, int index)
{
	return (SDL_Rect *)luaL_checkudata(L, index, "l_sdl_rect");
}

void l_sdl_rect_open(lua_State *L)
{
	class_begin(L, "l_sdl_rect");
	mod_function(L,	"__tostring",	l_rect_tostring);
	mod_variable(L, "x",	sdl_rect_get_x, sdl_rect_set_x);
	mod_variable(L, "y",	sdl_rect_get_y, sdl_rect_set_y);
	mod_variable(L, "w",	sdl_rect_get_w, sdl_rect_set_w);
	mod_variable(L, "h",	sdl_rect_get_h, sdl_rect_set_h);
	class_end(L);

	mod_begin_table(L, "Rect");
	mod_function(L, "New", l_new_rect);
	mod_end_table(L, "Rect");
}
