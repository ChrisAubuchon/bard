#include <btlib.h>
#include <l_int.h>
#include <l_sdl.h>

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#define TO_SDL_COLOR(state, index) (SDL_Color *)luaL_checkudata(state, \
						index, "l_sdl_color")

/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

/* Meta-methods */
static int l_sdl_color_tostring(lua_State *L);

/* Methods */
static int l_sdl_color_new(lua_State *L);

/* Member access functions */
static int sdl_color_get_r(lua_State *L);
static int sdl_color_get_g(lua_State *L);
static int sdl_color_get_b(lua_State *L);
static int sdl_color_set_r(lua_State *L);
static int sdl_color_set_g(lua_State *L);
static int sdl_color_set_b(lua_State *L);

/* Internal functions */
static SDL_Color *sdl_new_color(uint8_t r, uint8_t g, uint8_t b);


/********************************/
/*				*/
/* Lua functions		*/
/*				*/
/********************************/

static int l_sdl_color_tostring(lua_State *L)
{
	SDL_Color *c = TO_SDL_COLOR(L, 1);

	lua_pushfstring(L, "r: %d, g: %d, b: %d", c->r, c->g, c->b);

	return 1;
}

static int l_sdl_color_new(lua_State *L)
{
	SDL_Color *c;
	uint8_t r = 0, g = 0, b = 0;
	int top;

	top = lua_gettop(L);

	if (top == 3) {
		r = (uint8_t)luaL_checknumber(L, 1);
		g = (uint8_t)luaL_checknumber(L, 2);
		b = (uint8_t)luaL_checknumber(L, 3);
	} 

	L_NEWUSERDATA(L, c, sizeof(SDL_Color), "l_sdl_color");

	c->r = r;
	c->g = g;
	c->b = b;

	return 1;
}

static int sdl_color_get_r(lua_State *L)
{
	SDL_Color *c = TO_SDL_COLOR(L, 1);

	lua_pushnumber(L, c->r);
	return 1;
}

static int sdl_color_get_g(lua_State *L)
{
	SDL_Color *c = TO_SDL_COLOR(L, 1);

	lua_pushnumber(L, c->g);
	return 1;
}

static int sdl_color_get_b(lua_State *L)
{
	SDL_Color *c = TO_SDL_COLOR(L, 1);

	lua_pushnumber(L, c->b);
	return 1;
}

static int sdl_color_set_r(lua_State *L)
{
	SDL_Color *c = TO_SDL_COLOR(L, 1);
	uint8_t value = (uint8_t)luaL_checknumber(L, 2);

	c->r = value;

	return 0;
}

static int sdl_color_set_g(lua_State *L)
{
	SDL_Color *c = TO_SDL_COLOR(L, 1);
	uint8_t value = (uint8_t)luaL_checknumber(L, 2);

	c->g = value;

	return 0;
}

static int sdl_color_set_b(lua_State *L)
{
	SDL_Color *c = TO_SDL_COLOR(L, 1);
	uint8_t value = (uint8_t)luaL_checknumber(L, 2);

	c->b = value;

	return 0;
}


SDL_Color *sdl_color_new(uint8_t r, uint8_t g, uint8_t b)
{
	SDL_Color *c;

	c = (SDL_Color *)xzalloc(sizeof(SDL_Color));
	c->r = r;
	c->g = g;
	c->b = b;

	return c;
}

SDL_Color *sdl_color_arg(lua_State *L, int *index)
{
	SDL_Color *c;

	if (lua_isnil(L, *index)) {
		luaL_error(L, "Color argument expected at #%d", *index);
	} else if (lua_isnumber(L, *index)) {
		uint8_t r,g,b;

		r = (uint8_t)luaL_checknumber(L, *index);
		g = (uint8_t)luaL_checknumber(L, *index + 1);
		b = (uint8_t)luaL_checknumber(L, *index + 2);

		*index = *index + 3;

		return sdl_color_new(r, g, b);
	} else if (lua_isuserdata(L, *index)) {
		c = TO_SDL_COLOR(L, *index);

		*index = *index + 1;

		return sdl_color_new(c->r, c->g, c->b);
	} else {
		luaL_error(L, "Color argument expected at #%d", *index);
	}
}
		
void l_sdl_color_open(lua_State *L)
{
	class_begin(L, "l_sdl_color");
	mod_function(L, "__tostring", l_sdl_color_tostring);
	mod_variable(L, "r", sdl_color_get_r, sdl_color_set_r);
	mod_variable(L, "g", sdl_color_get_g, sdl_color_set_g);
	mod_variable(L, "b", sdl_color_get_b, sdl_color_set_b);
	class_end(L);

	mod_function(L,	"NewColor",	l_sdl_color_new);
}
