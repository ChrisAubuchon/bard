#include <bte.h>
#include <l_sdl.h>

/*#define DEBUG 1*/
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
static int l_renderer_tostring(lua_State *L);
static int l_renderer_free(lua_State *L);

/* Class methods */
static int l_renderer_clear(lua_State *L);
static int l_renderer_copy(lua_State *L);
static int l_renderer_fill_rect(lua_State *L);
static int l_renderer_present(lua_State *L);
static int l_renderer_set_viewport(lua_State *L);
static int l_renderer_get_color(lua_State *L);
static int l_renderer_set_color(lua_State *L);

/* sdl functions */
static int l_renderer_create_renderer(lua_State *L);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

/*
 * l_renderer_free()
 */
static int l_renderer_free(lua_State *L)
{
	SDL_Renderer	*renderer;

	renderer = l_checkRenderer(L, 1);

	SDL_DestroyRenderer(renderer);

	return 0;
}

/*
 * l_renderer_tostring()
 */
static int l_renderer_tostring(lua_State *L)
{
	luaL_Buffer	b;

	luaL_buffinit(L, &b);
	luaL_addstring(&b, "l_sdl_renderer");
	luaL_pushresult(&b);


	return 1;
}


/********************************/
/*				*/
/* Class Methods		*/
/*				*/
/********************************/

/*
 * l_renderer_clear()
 */
static int l_renderer_clear(lua_State *L)
{
	/* Args */
	SDL_Renderer	*renderer;

	renderer = l_checkRenderer(L, 1);

	if (SDL_RenderClear(renderer))
		sdl_error(L);

	return 0;
}

/*
 * l_renderer_copy()
 */
static int l_renderer_copy(lua_State *L)
{
	/* args */
	SDL_Renderer	*renderer;
	SDL_Texture	*texture;
	SDL_Rect	*srcrect;
	SDL_Rect	*dstrect;

	renderer = l_checkRenderer(L, 1);
	texture = l_checkTexture(L, 2);
	srcrect = l_testRect(L, 3);
	dstrect = l_testRect(L, 4);

	if (SDL_RenderCopy(renderer, texture, srcrect, dstrect))
		sdl_error(L);

	return 0;
}

/*
 * l_renderer_fill_rect()
 */
static int l_renderer_fill_rect(lua_State *L)
{
	/* Args */
	SDL_Renderer	*renderer;
	const SDL_Rect	*rect		= NULL;

	renderer = l_checkRenderer(L, 1);
	rect = l_testRect(L, 2);

	if (SDL_RenderFillRect(renderer, rect))
		sdl_error(L);

	return 0;
}

/*
 * l_renderer_present()
 */
static int l_renderer_present(lua_State *L)
{
	/* Args */
	SDL_Renderer	*renderer;

	renderer = l_checkRenderer(L, 1);
	SDL_RenderPresent(renderer);

	return 0;
}

/*
 * l_renderer_set_viewport()
 */
static int l_renderer_set_viewport(lua_State *L)
{
	/* args */
	SDL_Renderer	*renderer;
	SDL_Rect	*rect;

	renderer = l_checkRenderer(L, 1);
	rect = l_testRect(L, 2);

	if (SDL_RenderSetViewport(renderer, rect))
		sdl_error(L);

	return 0;
}

/*
 * l_renderer_set_color()
 */
static int l_renderer_set_color(lua_State *L)
{
	/* Args */
	SDL_Renderer	*renderer;
	uint8_t		r, g, b, a;

	renderer = l_checkRenderer(L, 1);

	lua_pushvalue(L, 2);

	lua_pushnumber(L, 1);
	lua_rawget(L, -2);
	r = (uint8_t)luaL_checkint(L, -1);
	lua_pop(L, 1);

	lua_pushnumber(L, 2);
	lua_rawget(L, -2);
	g = (uint8_t)luaL_checkint(L, -1);
	lua_pop(L, 1);

	lua_pushnumber(L, 3);
	lua_rawget(L, -2);
	b = (uint8_t)luaL_checkint(L, -1);
	lua_pop(L, 1);

	lua_pushnumber(L, 4);
	lua_rawget(L, -2);
	a = (uint8_t)luaL_optint(L, -1, 255);
	lua_pop(L, 1);
	lua_pop(L, 1);

	if (SDL_SetRenderDrawColor(renderer, r, g, b, a))
		sdl_error(L);

	return 0;
}

/*
 * l_renderer_get_color()
 */
static int l_renderer_get_color(lua_State *L)
{
	/* Args */
	SDL_Renderer	*renderer;

	uint8_t		r, g, b, a;

	if (SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a))
		sdl_error(L);

	lua_pushnumber(L, r);
	lua_pushnumber(L, g);
	lua_pushnumber(L, b);
	lua_pushnumber(L, a);

	return 4;
}

/********************************/
/*				*/
/* SDL functions		*/
/*				*/
/********************************/


/*
 * l_renderer_create_renderer()
 */
static int l_renderer_create_renderer(lua_State *L)
{
	/* Args */
	SDL_Window	*window;
	int		index;
	uint32_t	flags = 0;

	int		i;
	int		top;
	SDL_Renderer	**renderer;

	top = lua_gettop(L);

	window = l_checkWindow(L, 1);
	index = luaL_checkint(L, 2);

	for (i = 3; i < top; i++)
		flags |= luaL_checkint(L, i);

	l_renderer_new(L);
	renderer = l_checkRendererP(L, -1);

	*renderer = SDL_CreateRenderer(window, index, flags);
	if (*renderer == NULL)
		sdl_error(L);

	return 1;
}


/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

int l_renderer_new(lua_State *L)
{
	SDL_Renderer **s;

	s = lua_newuserdata(L, sizeof(SDL_Renderer *));
	luaL_getmetatable(L, "l_sdl_renderer");
	lua_setmetatable(L, -2);

	return 1;
}

SDL_Renderer *l_testRenderer(lua_State *L, int index)
{
	return *(SDL_Renderer **)luaL_testudata(L, index, "l_sdl_renderer");
}

SDL_Renderer *l_checkRenderer(lua_State *L, int index)
{
	return *(SDL_Renderer **)luaL_checkudata(L, index, "l_sdl_renderer");
}

SDL_Renderer **l_checkRendererP(lua_State *L, int index)
{
	return (SDL_Renderer **)luaL_checkudata(L, index, "l_sdl_renderer");
}

void l_sdl_renderer_open(lua_State *L)
{
	mod_begin_table(L, "RENDERER");
	mod_constant_n(L, "SOFTWARE",		SDL_RENDERER_SOFTWARE);
	mod_constant_n(L, "ACCELERATED",	SDL_RENDERER_ACCELERATED);
	mod_constant_n(L, "PRESENTVSYNC",	SDL_RENDERER_PRESENTVSYNC);
	mod_constant_n(L, "TARGETTEXTURE",	SDL_RENDERER_TARGETTEXTURE);
	mod_end_table(L, "RENDERER");

	mod_function(L, "CreateRenderer",	l_renderer_create_renderer);
	class_begin(L, "l_sdl_renderer");
	mod_function(L,	"__gc",		l_renderer_free);
	mod_function(L, "__tostring",	l_renderer_tostring);
	mod_function(L, "Clear",	l_renderer_clear);
	mod_function(L, "Copy",		l_renderer_copy);
	mod_function(L, "FillRect",	l_renderer_fill_rect);
	mod_function(L, "Present",	l_renderer_present);
	mod_function(L, "SetViewPort",	l_renderer_set_viewport);
	mod_variable(L, "DrawColor",	l_renderer_get_color,
					l_renderer_set_color);
	class_end(L);
}
