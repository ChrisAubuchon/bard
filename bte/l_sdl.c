#include <btlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
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


static int l_init(lua_State *L);
static int l_set_video_mode(lua_State *L);

static int l_get_key(lua_State *L);
static int _l_io_loop(lua_State *L);

static int keyDownEventHandler(lua_State *L, SDL_Event *e);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

void sdl_error(lua_State *L)
{
	luaL_error(L, SDL_GetError());
	exit(1);
}

static int l_init(lua_State *L)
{
	int top, i;
	uint32_t flags = 0;

	top = lua_gettop(L);
	for (i = 1; i < top; i++)
		flags |= (uint32_t)luaL_checkinteger(L, i);

	if (SDL_Init(flags) < 0)
		sdl_error(L);

	freopen("CONOUT$", "wb", stdout);
	freopen("CONOUT$", "wb", stderr);

	TTF_Init();

	atexit(SDL_Quit);

	return 0;
}

static int l_set_video_mode(lua_State *L)
{
	SDL_Surface **sur;

	int top, i;
	int width;
	int height;
	int depth;
	uint32_t flags = 0;

	top = lua_gettop(L);

	width = luaL_checkint(L, 1);
	height = luaL_checkint(L, 2);
	depth = luaL_checkint(L, 3);

	for (i = 4; i < top; i++) 
		flags |= (uint32_t)luaL_checkinteger(L, i);

	l_surface_new(L);
	sur = l_checkSurfaceP(L, -1);

	*sur = SDL_SetVideoMode(width, height, depth, flags);
	if (*sur == NULL) 
		sdl_error(L);

	return 1;
}

static int l_get_key(lua_State *L)
{
	int key;

	key = _l_io_loop(L);

	lua_pushnumber(L, key);

	return 1;
}

static int keyDownEventHandler(lua_State *L, SDL_Event *e)
{
	if (e->key.keysym.unicode)
		return e->key.keysym.unicode;
	else
		return e->key.keysym.sym;
}

static int _l_io_loop(lua_State *L)
{
	SDL_Event sdlevent;


	SDL_EnableUNICODE(1);
	while (1) {
		SDL_WaitEvent(&sdlevent);
		switch(sdlevent.type) {
			case BT_ANIM_EVENT:
			{
				void (*p) (void*) = sdlevent.user.data1;
				p(sdlevent.user.data2);
				break;
			}
			case BT_SLEEP_EVENT:
				return 0;
			case BT_CB_EVENT:
			{
				lua_rawgeti(L, LUA_REGISTRYINDEX, (int)sdlevent.user.data1);
				if (!lua_isfunction(L,1))
					luaL_error(L, "Stack corruption. Not a function");
				lua_call(L, 0, 0);
				break;
			}
			case SDL_KEYDOWN:
			{
				int key;

				key = keyDownEventHandler(L, &sdlevent);
				if (key != 0)
					return key;
				break;
			}
			case SDL_QUIT:
				exit(0);
		}
	}
}

/********************************/
/*				*/
/* Lua interface to SDL		*/
/*				*/
/********************************/

void l_sdl_open(lua_State *L)
{
	global_begin(L, "sdl");

	L_REGNUMBER(L,	"SDL_INIT_TIMER",	SDL_INIT_TIMER);
	L_REGNUMBER(L,	"SDL_INIT_AUDIO",	SDL_INIT_AUDIO);
	L_REGNUMBER(L,	"SDL_INIT_VIDEO",	SDL_INIT_VIDEO);
	L_REGNUMBER(L,	"SDL_INIT_JOYSTICK",	SDL_INIT_JOYSTICK);
	L_REGNUMBER(L,	"SDL_INIT_EVERYTHING",	SDL_INIT_EVERYTHING);
	L_REGNUMBER(L,	"SDL_INIT_NOPARACHUTE",	SDL_INIT_NOPARACHUTE);

	L_REGNUMBER(L,	"SDL_SWSURFACE",	SDL_SWSURFACE);
	L_REGNUMBER(L,	"SDL_HWSURFACE",	SDL_HWSURFACE);
	L_REGNUMBER(L,	"SDL_ASYNCBLIT",	SDL_ASYNCBLIT);
	L_REGNUMBER(L,	"SDL_ANYFORMAT",	SDL_ANYFORMAT);
	L_REGNUMBER(L,	"SDL_HWPALETTE",	SDL_HWPALETTE);
	L_REGNUMBER(L,	"SDL_DOUBLEBUF",	SDL_DOUBLEBUF);
	L_REGNUMBER(L,	"SDL_FULLSCREEN",	SDL_FULLSCREEN);
	L_REGNUMBER(L,	"SDL_OPENGL",		SDL_OPENGL);
	L_REGNUMBER(L,	"SDL_OPENGLBLIT",	SDL_OPENGLBLIT);
	L_REGNUMBER(L,	"SDL_RESIZABLE",	SDL_RESIZABLE);
	L_REGNUMBER(L,	"SDL_NOFRAME",	SDL_NOFRAME);

	L_REGNUMBER(L,	"IMG_SURFACE",		IMG_SURFACE);
	L_REGNUMBER(L,	"IMG_PNG",		IMG_PNG);
	L_REGNUMBER(L,	"IMG_BTA_SIMPLE",	IMG_BTA_SIMPLE);
	L_REGNUMBER(L,	"IMG_BTA_XOR",		IMG_BTA_XOR);

	L_REGFUNC(L, "Init",		l_init);
	L_REGFUNC(L, "SetVideoMode",	l_set_video_mode);

	L_REGFUNC(L, "GetKey",		l_get_key);

	L_REGNUMBER(L,	"SDLK_BACKSPACE",	SDLK_BACKSPACE);
	L_REGNUMBER(L,	"SDLK_RETURN",		SDLK_RETURN);
	L_REGNUMBER(L, "SDLK_ESCAPE",		SDLK_ESCAPE);
	L_REGNUMBER(L, "SDLK_UP",		SDLK_UP);
	L_REGNUMBER(L, "SDLK_DOWN",		SDLK_DOWN);
	L_REGNUMBER(L, "SDLK_RIGHT",		SDLK_RIGHT);
	L_REGNUMBER(L, "SDLK_LEFT",		SDLK_LEFT);
	L_REGNUMBER(L, "SDLK_PAGEUP",		SDLK_PAGEUP);
	L_REGNUMBER(L, "SDLK_PAGEDOWN",	SDLK_PAGEDOWN);
	L_REGNUMBER(L,	"SDLK_F1",		SDLK_F1);
	L_REGNUMBER(L,	"SDLK_F2",		SDLK_F2);
	L_REGNUMBER(L,	"SDLK_F3",		SDLK_F3);
	L_REGNUMBER(L,	"SDLK_F4",		SDLK_F4);
	L_REGNUMBER(L,	"SDLK_F5",		SDLK_F5);
	L_REGNUMBER(L,	"SDLK_F6",		SDLK_F6);
	L_REGNUMBER(L,	"SDLK_F7",		SDLK_F7);
	L_REGNUMBER(L,	"SDLK_F8",		SDLK_F8);
	L_REGNUMBER(L,	"SDLK_F9",		SDLK_F9);
	L_REGNUMBER(L,	"SDLK_F10",		SDLK_F10);
	L_REGNUMBER(L,	"SDLK_F11",		SDLK_F11);
	L_REGNUMBER(L,	"SDLK_F12",		SDLK_F12);

	lua_pushstring(L, "img");
	lua_newtable(L);
	L_REGFUNC(L, "LoadPNG",	l_img_load_png);
	L_REGFUNC(L, "LoadBTA",	l_img_load_bta);
	lua_rawset(L, -3);

	l_sdl_timer_open(L);
	l_sdl_textbox_open(L);
	l_sdl_font_open(L);
	l_sdl_surface_open(L);
	l_sdl_color_open(L);
	l_sdl_anim_open(L);
	l_sdl_rect_open(L);

	global_end(L);

}

