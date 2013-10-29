#include <l_int.h>
#include <l_sdl.h>


/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

static int	l_createWindow(lua_State *L);
static int	l_createWindowAndRenderer(lua_State *L);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

/*
 * l_createWindow()
 */
static int l_createWindow(lua_State *L)
{
	const char	*title;
	int		x, y, w, h;
	uint32_t	flags = 0;
	int		top;
	int		i;

	SDL_Window	**window;

	top	= lua_gettop(L);

	title	= luaL_checkstring(L, 1);
	x	= luaL_checkinteger(L, 2);
	y	= luaL_checkinteger(L, 3);
	w	= luaL_checkinteger(L, 4);
	h	= luaL_checkinteger(L, 5);

	for (i = 6; i < top; i++)
		flags |= (uint32_t)luaL_checkinteger(L, i);

	l_window_new(L);
	window = l_checkWindowP(L, -1);

	*window = SDL_CreateWindow(title, x, y, w, h, flags);
	if (*window == NULL)
		sdl_error(L);

	return 1;
}

/*
 * l_createWindowAndRenderer()
 */
static int l_createWindowAndRenderer(lua_State *L)
{
	int		w, h;
	uint32_t	flags = 0;

	SDL_Window	**window;
	SDL_Renderer	**renderer;

	int		top;
	int		i;

	top = lua_gettop(L);

	w	= luaL_checkinteger(L, 1);
	h	= luaL_checkinteger(L, 2);
	
	for (i = 3; i < top; i++) 
		flags |= (uint32_t)luaL_checkinteger(L, i);

	l_window_new(L);
	window = l_checkWindowP(L, -1);

	l_renderer_new(L);
	renderer = l_checkRendererP(L, -1);

	if (SDL_CreateWindowAndRenderer(w, h, flags, window, renderer) < 0)
		sdl_error(L);

	return 2;
}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

/*
 * l_window_new()
 */
int l_window_new(lua_State *L)
{
	SDL_Window **w;

	w = lua_newuserdata(L, sizeof(SDL_Window *));
	L_SETMETATABLE(L, "l_sdl_window");

	return 1;
}

/*
 * l_testWindow()
 */
SDL_Window *l_testWindow(lua_State *L, int index)
{
	return *(SDL_Window **)luaL_testudata(L, index, "l_sdl_window");
}

/*
 * l_checkWindow()
 */
SDL_Window *l_checkWindow(lua_State *L, int index)
{
	return *(SDL_Window **)luaL_checkudata(L, index, "l_sdl_window");
}

/*
 * l_checkWindowP()
 */
SDL_Window **l_checkWindowP(lua_State *L, int index)
{
	return (SDL_Window **)luaL_checkudata(L, index, "l_sdl_window");
}
/*
 * l_sdl_window_open()
 */
void l_sdl_window_open(lua_State *L)
{
	mod_function(L, "CreateWindow", l_createWindow);
	mod_function(L, "CreateWindowAndRenderer", l_createWindowAndRenderer);

	mod_constant(L, "WINDOWPOS_CENTERED", SDL_WINDOWPOS_CENTERED);
	mod_constant(L, "WINDOWPOS_UNDEFINED", SDL_WINDOWPOS_UNDEFINED);
	class_begin(L, "l_sdl_window");
	class_end(L);
}
