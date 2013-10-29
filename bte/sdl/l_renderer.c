#include <l_int.h>
#include <l_sdl.h>


/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

static int	l_createRenderer(lua_State *L);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

/*
 * l_createRenderer()
 */
static int l_createRenderer(lua_State *L)
{
	SDL_Window	*window;
	int		index;
	uint32_t	flags = 0;

	SDL_Renderer	**renderer;

	int		top;
	int		i;

	top	= lua_gettop(L);

	window	= l_checkWindow(L, 1);
	index	= luaL_checkinteger(L, 2);

	for (i = 3; i < top; i++)
		flags |= (uint32_t)luaL_checkinteger(L, i);

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

/*
 * l_window_new()
 */
int l_renderer_new(lua_State *L)
{
	SDL_Renderer **w;

	w = lua_newuserdata(L, sizeof(SDL_Renderer *));
	L_SETMETATABLE(L, "l_sdl_renderer");

	return 1;
}

/*
 * l_testRenderer()
 */
SDL_Renderer *l_testRenderer(lua_State *L, int index)
{
	return *(SDL_Renderer **)luaL_testudata(L, index, "l_sdl_renderer");
}

/*
 * l_checkRenderer()
 */
SDL_Renderer *l_checkRenderer(lua_State *L, int index)
{
	return *(SDL_Renderer **)luaL_checkudata(L, index, "l_sdl_renderer");
}

/*
 * l_checkRendererP()
 */
SDL_Renderer **l_checkRendererP(lua_State *L, int index)
{
	return (SDL_Renderer **)luaL_checkudata(L, index, "l_sdl_renderer");
}
/*
 * l_sdl_renderer_open()
 */
void l_sdl_renderer_open(lua_State *L)
{
	mod_function(L, "CreateRenderer", l_createRenderer);

	mod_constant(L, "RENDERER_SOFTWARE", SDL_RENDERER_SOFTWARE);
	mod_constant(L, "RENDERER_ACCELERATED", SDL_RENDERER_ACCELERATED);
	mod_constant(L, "RENDERER_PRESENTVSYNC", SDL_RENDERER_PRESENTVSYNC);
	mod_constant(L, "RENDERER_TARGETTEXTURE", SDL_RENDERER_TARGETTEXTURE);

	class_begin(L, "l_sdl_renderer");
	class_end(L);
}
