<<<<<<< HEAD
#include <bte.h>
#include <l_sdl.h>

/*#define DEBUG 1*/
#include <debug.h>

/********************************/
/*				*/
/* Helper macros		*/
/*				*/
/********************************/
=======
#include <l_int.h>
#include <l_sdl.h>

>>>>>>> 18c3a63c7e03ebf53e3f3d4d212b248963194f17

/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

<<<<<<< HEAD
/* Meta methods */
static int l_window_tostring(lua_State *L);

static int l_window_create_window(lua_State *L);
=======
static int	l_createWindow(lua_State *L);
static int	l_createWindowAndRenderer(lua_State *L);
>>>>>>> 18c3a63c7e03ebf53e3f3d4d212b248963194f17

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

<<<<<<< HEAD
static int l_window_free(lua_State *L)
{
	SDL_Window	*window;

	window = l_checkWindow(L, 1);

	SDL_DestroyWindow(window);

	return 0;
}

static int l_window_tostring(lua_State *L)
{
	luaL_Buffer	b;

	luaL_buffinit(L, &b);
	luaL_addstring(&b, "l_sdl_window");
	luaL_pushresult(&b);

=======
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
>>>>>>> 18c3a63c7e03ebf53e3f3d4d212b248963194f17

	return 1;
}

<<<<<<< HEAD
static int l_window_create_window(lua_State *L)
{
	const char	*title;
	int		x, y;
	int		w, h;
	uint32_t	flags		= 0;

	int		i;
	int		top;
	SDL_Window	**window;

	top = lua_gettop(L);

	title = luaL_checkstring(L, 1);
	x = luaL_checkint(L, 2);
	y = luaL_checkint(L, 3);
	w = luaL_checkint(L, 4);
	h = luaL_checkint(L, 5);

	for (i = 6; i < top; i++)
		flags |= luaL_checkint(L, i);
=======
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
>>>>>>> 18c3a63c7e03ebf53e3f3d4d212b248963194f17

	l_window_new(L);
	window = l_checkWindowP(L, -1);

<<<<<<< HEAD
	*window = SDL_CreateWindow(title, x, y, w, h, flags);
	if (*window == NULL)
		sdl_error(L);

	return 1;
}


=======
	l_renderer_new(L);
	renderer = l_checkRendererP(L, -1);

	if (SDL_CreateWindowAndRenderer(w, h, flags, window, renderer) < 0)
		sdl_error(L);

	return 2;
}

>>>>>>> 18c3a63c7e03ebf53e3f3d4d212b248963194f17
/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

<<<<<<< HEAD
int l_window_new(lua_State *L)
{
	SDL_Window **s;

	s = lua_newuserdata(L, sizeof(SDL_Window *));
	luaL_getmetatable(L, "l_sdl_window");
	lua_setmetatable(L, -2);
=======
/*
 * l_window_new()
 */
int l_window_new(lua_State *L)
{
	SDL_Window **w;

	w = lua_newuserdata(L, sizeof(SDL_Window *));
	L_SETMETATABLE(L, "l_sdl_window");
>>>>>>> 18c3a63c7e03ebf53e3f3d4d212b248963194f17

	return 1;
}

<<<<<<< HEAD
=======
/*
 * l_testWindow()
 */
>>>>>>> 18c3a63c7e03ebf53e3f3d4d212b248963194f17
SDL_Window *l_testWindow(lua_State *L, int index)
{
	return *(SDL_Window **)luaL_testudata(L, index, "l_sdl_window");
}

<<<<<<< HEAD
=======
/*
 * l_checkWindow()
 */
>>>>>>> 18c3a63c7e03ebf53e3f3d4d212b248963194f17
SDL_Window *l_checkWindow(lua_State *L, int index)
{
	return *(SDL_Window **)luaL_checkudata(L, index, "l_sdl_window");
}

<<<<<<< HEAD
=======
/*
 * l_checkWindowP()
 */
>>>>>>> 18c3a63c7e03ebf53e3f3d4d212b248963194f17
SDL_Window **l_checkWindowP(lua_State *L, int index)
{
	return (SDL_Window **)luaL_checkudata(L, index, "l_sdl_window");
}
<<<<<<< HEAD

void l_sdl_window_open(lua_State *L)
{
	mod_begin_table(L, "WINDOW");
	mod_constant_n(L, "FULLSCREEN",		SDL_WINDOW_FULLSCREEN);
	mod_constant_n(L, "FULLSCREEN_DESKTOP",	SDL_WINDOW_FULLSCREEN_DESKTOP);
	mod_constant_n(L, "OPENGL",		SDL_WINDOW_OPENGL);
	mod_constant_n(L, "HIDDEN",		SDL_WINDOW_HIDDEN);
	mod_constant_n(L, "BORDERLESS",		SDL_WINDOW_BORDERLESS);
	mod_constant_n(L, "RESIZABLE",		SDL_WINDOW_RESIZABLE);
	mod_constant_n(L, "MINIMIZED",		SDL_WINDOW_MINIMIZED);
	mod_constant_n(L, "MAXIMIZED",		SDL_WINDOW_MAXIMIZED);
	mod_constant_n(L, "INPUT_GRABBED",	SDL_WINDOW_INPUT_GRABBED);
	mod_constant_n(L, "ALLOW_HIGHDPI",	SDL_WINDOW_ALLOW_HIGHDPI);
	mod_end_table(L, "WINDOW");

	mod_begin_table(L, "WINDOWPOS");
	mod_constant_n(L, "CENTERED",		SDL_WINDOWPOS_CENTERED);
	mod_constant_n(L, "UNDEFINED",		SDL_WINDOWPOS_UNDEFINED);
	mod_end_table(L, "WINDOWPOS");

	mod_function(L, "CreateWindow",		l_window_create_window);
	class_begin(L, "l_sdl_window");
	mod_function(L,	"__gc",		l_window_free);
	mod_function(L, "__tostring",	l_window_tostring);
=======
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
>>>>>>> 18c3a63c7e03ebf53e3f3d4d212b248963194f17
	class_end(L);
}
