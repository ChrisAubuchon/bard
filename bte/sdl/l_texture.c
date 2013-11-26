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
static int l_texture_tostring(lua_State *L);

static int l_texture_update(lua_State *L);

static int l_texture_create_texture(lua_State *L);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

static int l_texture_free(lua_State *L)
{
	SDL_Texture	*texture;

	texture = l_checkTexture(L, 1);

	SDL_DestroyTexture(texture);

	return 0;
}

static int l_texture_tostring(lua_State *L)
{
	luaL_Buffer	b;

	luaL_buffinit(L, &b);
	luaL_addstring(&b, "l_sdl_texture");
	luaL_pushresult(&b);


	return 1;
}

static int l_texture_update(lua_State *L)
{
	/* Args */
	SDL_Texture	*texture;
	const SDL_Rect	*rect;
	SDL_Surface	*surface;

	texture = l_checkTexture(L, 1);
	rect = l_testRect(L, 2);
	surface = l_checkSurface(L, 3);

	if (SDL_UpdateTexture(texture, rect, surface->pixels, surface->pitch))
		sdl_error(L);

	return 0;
}

static int l_texture_create_texture(lua_State *L)
{
	/* Args*/
	SDL_Renderer	*renderer;
	uint32_t	format;
	int		access;
	int		w, h;

	SDL_Texture	**texture;

	renderer = l_checkRenderer(L, 1);
	format = luaL_checkint(L, 2);
	access = luaL_checkint(L, 3);
	w = luaL_checkint(L, 4);
	h = luaL_checkint(L, 5);

	l_texture_new(L);
	texture = l_checkTextureP(L, -1);

	*texture = SDL_CreateTexture(renderer, format, access, w, h);
	if (*texture == NULL)
		sdl_error(L);

	return 1;
}


/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

int l_texture_new(lua_State *L)
{
	SDL_Texture **s;

	s = lua_newuserdata(L, sizeof(SDL_Texture *));
	luaL_getmetatable(L, "l_sdl_texture");
	lua_setmetatable(L, -2);

	return 1;
}

SDL_Texture *l_testTexture(lua_State *L, int index)
{
	return *(SDL_Texture **)luaL_testudata(L, index, "l_sdl_texture");
}

SDL_Texture *l_checkTexture(lua_State *L, int index)
{
	return *(SDL_Texture **)luaL_checkudata(L, index, "l_sdl_texture");
}

SDL_Texture **l_checkTextureP(lua_State *L, int index)
{
	return (SDL_Texture **)luaL_checkudata(L, index, "l_sdl_texture");
}

void l_sdl_texture_open(lua_State *L)
{
	mod_begin_table(L, "TEXTUREACCESS");
	mod_constant_n(L, "STATIC",	SDL_TEXTUREACCESS_STATIC);
	mod_constant_n(L, "STREAMING",	SDL_TEXTUREACCESS_STREAMING);
	mod_end_table(L, "TEXTUREACCESS");

	mod_function(L, "CreateTexture",		l_texture_create_texture);
	class_begin(L, "l_sdl_texture");
	mod_function(L,	"__gc",		l_texture_free);
	mod_function(L, "__tostring",	l_texture_tostring);
	mod_function(L, "Update",	l_texture_update);
	class_end(L);
}
