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

/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

/* Meta methods */
static int l_surface_tostring(lua_State *L);

/* Member access functions */
static int l_surface_get_isanim(lua_State *L);
static int l_surface_get_w(lua_State *L);
static int l_surface_get_h(lua_State *L);
static int l_surface_get_pitch(lua_State *L);

/* Methods */
static int l_surface_update_rect(lua_State *L);
static int l_surface_fill_rect(lua_State *L);
static int l_surface_set_color(lua_State *L);
static int l_surface_set_color_key(lua_State *L);
static int l_surface_clear_color_key(lua_State *L);

static int l_surface_create_rgb(lua_State *L);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

static int l_surface_free(lua_State *L)
{
	SDL_Surface *s = l_checkSurface(L, 1);

	SDL_FreeSurface(s);

	return 0;
}

static int l_surface_tostring(lua_State *L)
{
	SDL_Surface *sur = l_checkSurface(L, 1);
	int x, y;
	luaL_Buffer b;
	uint8_t *pixels;
	uint8_t buf[512];

	luaL_buffinit(L, &b);

	for (x = 0; x < 256; x++) {
		sprintf(buf, "colors[%3d]: r = %3d, g = %3d, b = %3d\n",
			x, 
			sur->format->palette->colors[x].r,
			sur->format->palette->colors[x].g,
			sur->format->palette->colors[x].b
			);
		luaL_addstring(&b, buf);
	}

	if (SDL_MUSTLOCK(sur))
		SDL_LockSurface(sur);

	pixels = (uint8_t *)sur->pixels;
	for (y = 0; y < sur->h; y++) {
		for (x = 0; x < sur->w; x++) {
			luaL_addchar(&b, *pixels + '0');
			pixels++;
		}
		luaL_addchar(&b, '\n');
	}

	if (SDL_MUSTLOCK(sur))
		SDL_UnlockSurface(sur);

	luaL_pushresult(&b);


	return 1;
}

static int l_surface_get_isanim(lua_State *L)
{
/*	lua_pushnumber(L, 0);*/
	lua_pushboolean(L, 0);
	
	return 1;
}

static int l_surface_get_w(lua_State *L)
{
	SDL_Surface *s = l_checkSurface(L, 1);

	lua_pushnumber(L, s->w);

	return 1;
}

static int l_surface_get_h(lua_State *L)
{
	SDL_Surface *s = l_checkSurface(L, 1);

	lua_pushnumber(L, s->h);

	return 1;
}

static int l_surface_get_pitch(lua_State *L)
{
	SDL_Surface *s = l_checkSurface(L, 1);

	lua_pushnumber(L, s->pitch);

	return 1;
}

static int l_surface_update_rect(lua_State *L)
{
	SDL_Surface *sur;
	SDL_Rect *r;
	int index = 2;

	sur = l_checkSurface(L, 1);
	r = sdl_rect_arg(L, &index);

	SDL_UpdateRect(sur, r->x, r->y, r->w, r->h);

	free(r);

	return 0;
}

static int l_surface_set_color(lua_State *L)
{
	SDL_Surface *sur;
	SDL_Color *c;
	int index = 3;
	int color;
	int rval;
	

	sur = l_checkSurface(L, 1);
	color = luaL_checkint(L, 2);
	c = sdl_color_arg(L, &index);

	rval = SDL_SetPalette(sur, SDL_LOGPAL | SDL_PHYSPAL, c, color, 1);

	return 0;
}

static int l_surface_set_color_key(lua_State *L)
{
	SDL_Surface	*sur;
	int		key;
	int		rval;

	sur = l_checkSurface(L, 1);
	key = luaL_checkint(L, 2);

	rval = SDL_SetColorKey(sur, SDL_SRCCOLORKEY, key);

	return 0;
}

static int l_surface_clear_color_key(lua_State *L)
{
	SDL_Surface	*sur;
	int		key;
	int		rval;

	sur = l_checkSurface(L, 1);

	rval = SDL_SetColorKey(sur, 0, 0);

	return 0;
}

static int l_surface_fill_rect(lua_State *L)
{
	SDL_Surface *sur;
	SDL_Rect *r;
	SDL_Color *c;
	int index = 2;
	uint32_t color;

	sur = l_checkSurface(L, 1);
	r = sdl_rect_arg(L, &index);
	c = sdl_color_arg(L, &index);

	color = SDL_MapRGB(sur->format, c->r, c->g, c->b);

	SDL_FillRect(sur, r, color);

	free(r);
	free(c);

	return 0;
}

static int l_surface_blit(lua_State *L)
{
	SDL_Surface *src;
	SDL_Surface *dest;
	SDL_Rect *sr;
	SDL_Rect *dr;
	int index = 1;

	dest = l_checkSurface(L, index++);
	dr = sdl_rect_arg(L, &index);
	src = l_checkSurface(L, index++);
	sr = sdl_rect_arg(L, &index);

	if (SDL_BlitSurface(src, sr, dest, dr)) {
		sdl_error(L);
	}

	free(sr);
	free(dr);

	return 0;
}

static int l_surface_flip(lua_State *L)
{
	SDL_Surface *sur;

	sur = l_checkSurface(L, 1);

	SDL_Flip(sur);

	return 0;
}

static int l_surface_create_rgb(lua_State *L)
{
	int width;
	int height;
	int depth;
	SDL_Surface **s;

	width = luaL_checkint(L, 1);
	height = luaL_checkint(L, 2);
	depth = luaL_optint(L, 3, 8);	/* Default to 8-bit depth */

	l_surface_new(L);
	s = l_checkSurfaceP(L, -1);

	*s = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, depth, \
					0,0,0,0);
	SDL_SetColors(*s, egapal, 0, 16);

	if (*s == NULL)
		sdl_error(L);

	return 1;
}


/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

int l_surface_new(lua_State *L)
{
	SDL_Surface **s;

	s = lua_newuserdata(L, sizeof(SDL_Surface *));
	luaL_getmetatable(L, "l_sdl_surface");
	lua_setmetatable(L, -2);

	return 1;
}

SDL_Surface *l_testSurface(lua_State *L, int index)
{
	return *(SDL_Surface **)luaL_testudata(L, index, "l_sdl_surface");
}

SDL_Surface *l_checkSurface(lua_State *L, int index)
{
	return *(SDL_Surface **)luaL_checkudata(L, index, "l_sdl_surface");
}

SDL_Surface **l_checkSurfaceP(lua_State *L, int index)
{
	return (SDL_Surface **)luaL_checkudata(L, index, "l_sdl_surface");
}

void l_sdl_surface_open(lua_State *L)
{
	mod_function(L, "CreateRGBSurface",	l_surface_create_rgb);
	class_begin(L, "l_sdl_surface");
	mod_function(L,	"__gc",		l_surface_free);
	mod_function(L, "__tostring",	l_surface_tostring);
	mod_variable(L, "isAnim",	l_surface_get_isanim,	l_var_readonly);
	mod_variable(L, "w",		l_surface_get_w, 	l_var_readonly);
	mod_variable(L, "h",		l_surface_get_h, 	l_var_readonly);
	mod_variable(L, "pitch",	l_surface_get_pitch,	l_var_readonly);
	mod_function(L, "UpdateRect",	l_surface_update_rect);
	mod_function(L, "FillRect",	l_surface_fill_rect);
	mod_function(L, "Blit",		l_surface_blit);
	mod_function(L, "Flip",		l_surface_flip);
	mod_function(L, "SetColor",	l_surface_set_color);
	mod_function(L, "SetColorKey",	l_surface_set_color_key);
	mod_function(L, "ClearColorKey",l_surface_clear_color_key);
	class_end(L);
}
