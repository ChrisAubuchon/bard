#include <bte.h>

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

#if 0
	TTF_Init();
#endif

	atexit(SDL_Quit);

	return 0;
}

/*
 * l_get_key()
 */
static int l_get_key(lua_State *L)
{
	SDL_Event	sdlevent;

	while (1) {
		SDL_WaitEvent(&sdlevent);
		switch (sdlevent.type) {
		case BT_ANIM_EVENT:
		{
			void (*p) (void*) = sdlevent.user.data1;
			p(sdlevent.user.data2);
			break;
		}
		case BT_SLEEP_EVENT:
			lua_pushnumber(L, 0);
			return 1;
		case BT_CB_EVENT:
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, 
					(int)sdlevent.user.data1);
			if (!lua_isfunction(L, 1)) {
				luaL_error(L, 
					"Stack corruption. Not a function: %d",
					lua_type(L, 1)
					);
			}
			lua_call(L, 0, 1);
			if (!lua_isnil(L, 1)) {
				return 1;
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
	global_begin(L, "SDL");

	mod_begin_table(L, "INIT");
	mod_constant_n(L, "TIMER",		SDL_INIT_TIMER);
	mod_constant_n(L, "AUDIO",		SDL_INIT_AUDIO);
	mod_constant_n(L, "VIDEO",		SDL_INIT_VIDEO);
	mod_constant_n(L, "JOYSTICK",		SDL_INIT_JOYSTICK);
	mod_constant_n(L, "HAPTIC",		SDL_INIT_HAPTIC);
	mod_constant_n(L, "GAMECONTROLLER",	SDL_INIT_GAMECONTROLLER);
	mod_constant_n(L, "EVENTS",		SDL_INIT_EVENTS);
	mod_constant_n(L, "EVERYTHING",		SDL_INIT_EVERYTHING);
	mod_constant_n(L, "NOPARACHUTE",	SDL_INIT_NOPARACHUTE);
	mod_end_table(L, "INIT");

	mod_begin_table(L, "PIXELFORMAT");
	mod_constant_n(L, "UNKNOWN",		SDL_PIXELFORMAT_UNKNOWN);
	mod_constant_n(L, "INDEX1LSB",		SDL_PIXELFORMAT_INDEX1LSB);
	mod_constant_n(L, "INDEX1MSB",		SDL_PIXELFORMAT_INDEX1MSB);
	mod_constant_n(L, "INDEX4LSB",		SDL_PIXELFORMAT_INDEX4LSB);
	mod_constant_n(L, "INDEX4MSB",		SDL_PIXELFORMAT_INDEX4MSB);
	mod_constant_n(L, "INDEX8",		SDL_PIXELFORMAT_INDEX8);
	mod_constant_n(L, "RGB332",		SDL_PIXELFORMAT_RGB332);
	mod_constant_n(L, "RGB444",		SDL_PIXELFORMAT_RGB444);
	mod_constant_n(L, "RGB555",		SDL_PIXELFORMAT_RGB555);
	mod_constant_n(L, "BGR555",		SDL_PIXELFORMAT_BGR555);
	mod_constant_n(L, "ARGB4444",		SDL_PIXELFORMAT_ARGB4444);
	mod_constant_n(L, "RGBA4444",		SDL_PIXELFORMAT_RGBA4444);
	mod_constant_n(L, "ABGR4444",		SDL_PIXELFORMAT_ABGR4444);
	mod_constant_n(L, "BGRA4444",		SDL_PIXELFORMAT_BGRA4444);
	mod_constant_n(L, "ARGB1555",		SDL_PIXELFORMAT_ARGB1555);
	mod_constant_n(L, "RGBA5551",		SDL_PIXELFORMAT_RGBA5551);
	mod_constant_n(L, "ABGR1555",		SDL_PIXELFORMAT_ABGR1555);
	mod_constant_n(L, "BGRA5551",		SDL_PIXELFORMAT_BGRA5551);
	mod_constant_n(L, "RGB565",		SDL_PIXELFORMAT_RGB565);
	mod_constant_n(L, "BGR565",		SDL_PIXELFORMAT_BGR565);
	mod_constant_n(L, "RGB24",		SDL_PIXELFORMAT_RGB24);
	mod_constant_n(L, "BGR24",		SDL_PIXELFORMAT_BGR24);
	mod_constant_n(L, "RGB888",		SDL_PIXELFORMAT_RGB888);
	mod_constant_n(L, "RGBX8888",		SDL_PIXELFORMAT_RGBX8888);
	mod_constant_n(L, "BGR888",		SDL_PIXELFORMAT_BGR888);
	mod_constant_n(L, "BGRX8888",		SDL_PIXELFORMAT_BGRX8888);
	mod_constant_n(L, "ARGB8888",		SDL_PIXELFORMAT_ARGB8888);
	mod_constant_n(L, "RGBA8888",		SDL_PIXELFORMAT_RGBA8888);
	mod_constant_n(L, "ABGR8888",		SDL_PIXELFORMAT_ABGR8888);
	mod_constant_n(L, "BGRA8888",		SDL_PIXELFORMAT_BGRA8888);
	mod_constant_n(L, "ARGB2101010",	SDL_PIXELFORMAT_ARGB2101010);
	mod_constant_n(L, "YV12",		SDL_PIXELFORMAT_YV12);
	mod_constant_n(L, "IYUV",		SDL_PIXELFORMAT_IYUV);
	mod_constant_n(L, "YUY2",		SDL_PIXELFORMAT_YUY2);
	mod_constant_n(L, "UYVY",		SDL_PIXELFORMAT_UYVY);
	mod_constant_n(L, "YVYU",		SDL_PIXELFORMAT_YVYU);
	mod_end_table(L, "PIXELFORMAT");

	mod_function(L,	"Init",			l_init);
	mod_function(L, "GetKey",		l_get_key);
	mod_function(L, "LoadPNG",		l_img_load_png);
	mod_function(L, "LoadBTA",		l_img_load_bta);

	l_sdl_window_open(L);
	l_sdl_renderer_open(L);
	l_sdl_texture_open(L);
	l_sdl_rect_open(L);
	l_sdl_anim_open(L);
	l_sdl_surface_open(L);
	l_sdl_font_open(L);
	l_sdl_textbox_open(L);

	mod_begin_table(L, "K");
	mod_constant_n(L,	"BACKSPACE",	SDLK_BACKSPACE);
	mod_constant_n(L,	"RETURN",	SDLK_RETURN);
	mod_constant_n(L,	"ESCAPE",	SDLK_ESCAPE);
	mod_constant_n(L,	"UP",		SDLK_UP);
	mod_constant_n(L,	"DOWN",		SDLK_DOWN);
	mod_constant_n(L,	"RIGHT",	SDLK_RIGHT);
	mod_constant_n(L,	"LEFT",		SDLK_LEFT);
	mod_constant_n(L,	"PAGEUP",	SDLK_PAGEUP);
	mod_constant_n(L,	"PAGEDOWN",	SDLK_PAGEDOWN);
	mod_constant_n(L,	"F1",		SDLK_F1);
	mod_constant_n(L,	"F2",		SDLK_F2);
	mod_constant_n(L,	"F3",		SDLK_F3);
	mod_constant_n(L,	"F4",		SDLK_F4);
	mod_constant_n(L,	"F5",		SDLK_F5);
	mod_constant_n(L,	"F6",		SDLK_F6);
	mod_constant_n(L,	"F7",		SDLK_F7);
	mod_constant_n(L,	"F8",		SDLK_F8);
	mod_constant_n(L,	"F9",		SDLK_F9);
	mod_constant_n(L,	"F10",		SDLK_F10);
	mod_constant_n(L,	"F11",		SDLK_F11);
	mod_constant_n(L,	"F12",		SDLK_F12);
	mod_end_table(L, "K");

#if 0
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
#endif

	global_end(L);

}

