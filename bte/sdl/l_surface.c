#include <bte.h>
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

	sprintf(buf, "w: %4d, h: %4d\n", sur->w, sur->h);
	luaL_addstring(&b, buf);
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

static int l_surface_set_color_key(lua_State *L)
{
	SDL_Surface	*sur;
	int		key;
	int		rval;

	sur = l_checkSurface(L, 1);
	key = luaL_checkint(L, 2);

	rval = SDL_SetColorKey(sur, SDL_TRUE, key);

	return 0;
}

static int l_surface_clear_color_key(lua_State *L)
{
	SDL_Surface	*sur;
	int		key;
	int		rval;

	sur = l_checkSurface(L, 1);

	rval = SDL_SetColorKey(sur, SDL_FALSE, 0);

	return 0;
}

static int l_surface_fill_rect(lua_State *L)
{
	SDL_Surface	*sur;
	SDL_Rect	*rect;
	uint8_t		r, g, b, a;

	sur = l_checkSurface(L, 1);
	rect = l_testRect(L, 2);
	r = (uint8_t)luaL_checkint(L, 3);
	g = (uint8_t)luaL_checkint(L, 4);
	b = (uint8_t)luaL_checkint(L, 5);
	a = (uint8_t)luaL_optint(L, 6, 255);

	SDL_FillRect(sur, rect, SDL_MapRGBA(sur->format, r, g, b, a));

	return 0;
}

static int l_surface_blit(lua_State *L)
{
	SDL_Surface	*src;
	SDL_Surface	*dest;
	SDL_Rect	*sr;
	SDL_Rect	*dr;

	dest = l_checkSurface(L, 1);
	dr = l_testRect(L, 2);
	src = l_checkSurface(L, 3);
	sr = l_testRect(L, 4);

	if (SDL_BlitSurface(src, sr, dest, dr)) {
		sdl_error(L);
	}

	return 0;
}

static int l_surface_create_rgb(lua_State *L)
{
	/* Args */
	int		width;
	int		height;
	int		depth;

	SDL_Surface	**s;
	uint32_t	rmask, gmask, bmask, amask;

	/*
	 * There are some endian-ness issues. SDL2_image reads in
	 * PNG files in ABGR8888 on x86. PNG is in RGBA8888. So we
	 * swap the endian check here. I'm thinking that it's an
	 * issue with SDL_image. If it is going to create an ABGR
	 * image then it needs to run a transform on the png file.
	 */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif
	width = luaL_checkint(L, 1);
	height = luaL_checkint(L, 2);
	depth = luaL_optint(L, 3, 32);

	l_surface_new(L);
	s = l_checkSurfaceP(L, -1);

	*s = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, depth, \
				rmask, gmask, bmask, amask);

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
	mod_variable(L, "w",		l_surface_get_w, 	l_var_readonly);
	mod_variable(L, "h",		l_surface_get_h, 	l_var_readonly);
	mod_variable(L, "pitch",	l_surface_get_pitch,	l_var_readonly);
	mod_function(L, "FillRect",	l_surface_fill_rect);
	mod_function(L, "Blit",		l_surface_blit);
	mod_function(L, "SetColorKey",	l_surface_set_color_key);
	mod_function(L, "ClearColorKey",l_surface_clear_color_key);
	class_end(L);
}
