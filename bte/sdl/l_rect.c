#include <btlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <l_int.h>
#include <l_sdl.h>

#define DEBUG 1
#include <debug.h>

/********************************/
/*				*/
/* Helper macros		*/
/*				*/
/********************************/

#define TO_SDL_RECT(state, index) (SDL_Rect *)luaL_checkudata(state, \
						index, "l_sdl_rect")
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
static SDL_Rect *sdl_new_rect(int x, int y, int w, int h);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

static int sdl_rect_get_x(lua_State *L)
{
	SDL_Rect *r = TO_SDL_RECT(L, 1);
	lua_pushnumber(L, r->x);
	return 1;
}

static int sdl_rect_get_y(lua_State *L)
{
	SDL_Rect *r = TO_SDL_RECT(L, 1);
	lua_pushnumber(L, r->y);
	return 1;
}

static int sdl_rect_get_w(lua_State *L)
{
	SDL_Rect *r = TO_SDL_RECT(L, 1);
	lua_pushnumber(L, r->w);
	return 1;
}

static int sdl_rect_get_h(lua_State *L)
{
	SDL_Rect *r = TO_SDL_RECT(L, 1);
	lua_pushnumber(L, r->h);
	return 1;
}

static int sdl_rect_set_x(lua_State *L)
{
	SDL_Rect *r = TO_SDL_RECT(L, 1);
	uint8_t val = (uint8_t)luaL_checknumber(L, 2);

	r->x = val;

	return 0;
}

static int sdl_rect_set_y(lua_State *L)
{
	SDL_Rect *r = TO_SDL_RECT(L, 1);
	uint8_t val = (uint8_t)luaL_checknumber(L, 2);

	r->y = val;

	return 0;
}

static int sdl_rect_set_w(lua_State *L)
{
	SDL_Rect *r = TO_SDL_RECT(L, 1);
	uint8_t val = (uint8_t)luaL_checknumber(L, 2);

	r->w = val;

	return 0;
}

static int sdl_rect_set_h(lua_State *L)
{
	SDL_Rect *r = TO_SDL_RECT(L, 1);
	uint8_t val = (uint8_t)luaL_checknumber(L, 2);

	r->h = val;

	return 0;
}

static int l_new_rect(lua_State *L)
{
	SDL_Rect *r;

	r = lua_newuserdata(L, sizeof(SDL_Rect));
	L_SETMETATABLE(L, "l_sdl_rect");

	r->x = (int16_t)luaL_checkinteger(L, 1);
	r->y = (int16_t)luaL_checkinteger(L, 2);
	r->w = (uint16_t)luaL_checkinteger(L, 3);
	r->h = (uint16_t)luaL_checkinteger(L, 4);

	return 1;
}

static int l_rect_tostring(lua_State *L)
{
	SDL_Rect *r = TO_SDL_RECT(L, 1);

	lua_pushfstring(L, "x: %d, y: %d, w: %d, h: %d\n", r->x, r->y, r->w, r->h);

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
 * sdl_rect_arg()
 * Returns a pointer to a SDL_Rect structure. 
 */
SDL_Rect *sdl_rect_arg(lua_State *L, int *index)
{
	SDL_Rect *r;

	if (lua_isnil(L, *index)) {
		*index = *index + 1;
		return NULL;
	} else if (lua_isnumber(L, *index)) {
		int x, y, w, h;

		x = (int16_t)luaL_checkinteger(L, *index);
		y = (int16_t)luaL_checkinteger(L, *index + 1);
		w = (uint16_t)luaL_checkinteger(L, *index + 2);
		h = (uint16_t)luaL_checkinteger(L, *index + 3);

		*index = *index + 4;

		return sdl_new_rect(x, y, w, h);
	} else if (lua_isuserdata(L, *index)) {
		/*
		 * Make a copy of the passed in l_sdl_rect userdata.
		 */
		r = (SDL_Rect *)luaL_checkudata(L, *index, "l_sdl_rect");

		*index = *index + 1;

		return sdl_new_rect(r->x, r->y, r->w, r->h);
	} else {
		luaL_error(L, "SDL_Rect argument expected at #%d\n", *index);
		return NULL;
	}
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

	mod_function(L, "NewRect", l_new_rect);
}
