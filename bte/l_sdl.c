#include <btlib.h>
#include <SDL2/SDL.h>
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


static int l_init(lua_State *L);

static int l_get_key(lua_State *L);
static int _l_io_loop(lua_State *L);

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

static int l_get_key(lua_State *L)
{
	int key;

	_l_io_loop(L);

	return 1;
}

static int _l_io_loop(lua_State *L)
{
	SDL_Event sdlevent;


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
				lua_pushnumber(L, 0);
				return 0;
			case BT_CB_EVENT:
			{
				lua_rawgeti(L, LUA_REGISTRYINDEX, (int)sdlevent.user.data1);
				if (!lua_isfunction(L,1))
					luaL_error(L, "Stack corruption. Not a function: %d", lua_type(L, 1));
				lua_call(L, 0, 1);
				if (!lua_isnil(L, 1)) {
					return;
				}
				lua_pop(L, 1);
				break;
			}
			case SDL_KEYDOWN:
			{
				lua_pushnumber(L, sdlevent.key.keysym.sym);
				return 1;
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

	/* Initilization flags */
	mod_constant(L,	"SDL_INIT_TIMER",	SDL_INIT_TIMER);
	mod_constant(L,	"SDL_INIT_AUDIO",	SDL_INIT_AUDIO);
	mod_constant(L,	"SDL_INIT_VIDEO",	SDL_INIT_VIDEO);
	mod_constant(L,	"SDL_INIT_JOYSTICK",	SDL_INIT_JOYSTICK);
	mod_constant(L,	"SDL_INIT_HAPTIC",	SDL_INIT_HAPTIC);
	mod_constant(L,	"SDL_INIT_GAMECONTROLLER",SDL_INIT_GAMECONTROLLER);
	mod_constant(L,	"SDL_INIT_EVENTS",	SDL_INIT_EVENTS);
	mod_constant(L,	"SDL_INIT_EVERYTHING",	SDL_INIT_EVERYTHING);
	mod_constant(L,	"SDL_INIT_NOPARACHUTE",	SDL_INIT_NOPARACHUTE);

	/* Window States */
	mod_constant(L, "SDL_WINDOW_FULLSCREEN",SDL_WINDOW_FULLSCREEN);
	mod_constant(L, "SDL_WINDOW_FULLSCREEN_DESKTOP",
				SDL_WINDOW_FULLSCREEN_DESKTOP);
	mod_constant(L, "SDL_WINDOW_OPENGL",SDL_WINDOW_OPENGL);
	mod_constant(L, "SDL_WINDOW_SHOWN",SDL_WINDOW_SHOWN);
	mod_constant(L, "SDL_WINDOW_HIDDEN",SDL_WINDOW_HIDDEN);
	mod_constant(L, "SDL_WINDOW_BORDERLESS",SDL_WINDOW_BORDERLESS);
	mod_constant(L, "SDL_WINDOW_RESIZABLE",SDL_WINDOW_RESIZABLE);
	mod_constant(L, "SDL_WINDOW_MINIMIZED",SDL_WINDOW_MINIMIZED);
	mod_constant(L, "SDL_WINDOW_MAXIMIZED",SDL_WINDOW_MAXIMIZED);
	mod_constant(L, "SDL_WINDOW_INPUT_GRABBED",SDL_WINDOW_INPUT_GRABBED);
	mod_constant(L, "SDL_WINDOW_INPUT_FOCUS",SDL_WINDOW_INPUT_FOCUS);
	mod_constant(L, "SDL_WINDOW_MOUSE_FOCUS",SDL_WINDOW_MOUSE_FOCUS);
	mod_constant(L, "SDL_WINDOW_FOREIGN",SDL_WINDOW_FOREIGN);
	mod_constant(L, "SDL_WINDOW_ALLOW_HIGHDPI",SDL_WINDOW_ALLOW_HIGHDPI);

#if 0
	mod_constant(L,	"SDL_SWSURFACE",	SDL_SWSURFACE);
	mod_constant(L,	"SDL_HWSURFACE",	SDL_HWSURFACE);
	mod_constant(L,	"SDL_ASYNCBLIT",	SDL_ASYNCBLIT);
	mod_constant(L,	"SDL_ANYFORMAT",	SDL_ANYFORMAT);
	mod_constant(L,	"SDL_HWPALETTE",	SDL_HWPALETTE);
	mod_constant(L,	"SDL_DOUBLEBUF",	SDL_DOUBLEBUF);
	mod_constant(L,	"SDL_FULLSCREEN",	SDL_FULLSCREEN);
	mod_constant(L,	"SDL_OPENGL",		SDL_OPENGL);
	mod_constant(L,	"SDL_OPENGLBLIT",	SDL_OPENGLBLIT);
	mod_constant(L,	"SDL_RESIZABLE",	SDL_RESIZABLE);
	mod_constant(L,	"SDL_NOFRAME",	SDL_NOFRAME);
#endif

	mod_constant(L,	"IMG_SURFACE",		IMG_SURFACE);
	mod_constant(L,	"IMG_PNG",		IMG_PNG);
	mod_constant(L,	"IMG_BTA_SIMPLE",	IMG_BTA_SIMPLE);
	mod_constant(L,	"IMG_BTA_XOR",		IMG_BTA_XOR);

	mod_function(L, "Init",		l_init);
#if 0
	mod_function(L, "SetVideoMode",	l_set_video_mode);
#endif

	mod_function(L, "GetKey",		l_get_key);

	mod_constant(L,	"SDLK_BACKSPACE",	SDLK_BACKSPACE);
	mod_constant(L,	"SDLK_RETURN",		SDLK_RETURN);
	mod_constant(L, "SDLK_ESCAPE",		SDLK_ESCAPE);
	mod_constant(L, "SDLK_UP",		SDLK_UP);
	mod_constant(L, "SDLK_DOWN",		SDLK_DOWN);
	mod_constant(L, "SDLK_RIGHT",		SDLK_RIGHT);
	mod_constant(L, "SDLK_LEFT",		SDLK_LEFT);
	mod_constant(L, "SDLK_PAGEUP",		SDLK_PAGEUP);
	mod_constant(L, "SDLK_PAGEDOWN",	SDLK_PAGEDOWN);
	mod_constant(L,	"SDLK_F1",		SDLK_F1);
	mod_constant(L,	"SDLK_F2",		SDLK_F2);
	mod_constant(L,	"SDLK_F3",		SDLK_F3);
	mod_constant(L,	"SDLK_F4",		SDLK_F4);
	mod_constant(L,	"SDLK_F5",		SDLK_F5);
	mod_constant(L,	"SDLK_F6",		SDLK_F6);
	mod_constant(L,	"SDLK_F7",		SDLK_F7);
	mod_constant(L,	"SDLK_F8",		SDLK_F8);
	mod_constant(L,	"SDLK_F9",		SDLK_F9);
	mod_constant(L,	"SDLK_F10",		SDLK_F10);
	mod_constant(L,	"SDLK_F11",		SDLK_F11);
	mod_constant(L,	"SDLK_F12",		SDLK_F12);

	lua_pushstring(L, "img");
	lua_newtable(L);
	mod_function(L, "LoadPNG",	l_img_load_png);
	mod_function(L, "LoadBTA",	l_img_load_bta);
	lua_rawset(L, -3);

	l_sdl_window_open(L);
	l_sdl_renderer_open(L);
	l_sdl_timer_open(L);
	l_sdl_textbox_open(L);
	l_sdl_font_open(L);
	l_sdl_surface_open(L);
	l_sdl_color_open(L);
	l_sdl_anim_open(L);
	l_sdl_rect_open(L);

	global_end(L);

}

