#include <btlib.h>
#include <l_int.h>
#include <l_sdl.h>
#include <font.h>

/********************************/
/*				*/
/* Internal structures		*/
/*				*/
/********************************/

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/


/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

/* Meta-methods */

/* Methods */
static int l_font_open(lua_State *L);
static int l_font_close(lua_State *L);
static int l_font_render_text(lua_State *L);
static int l_font_size_text(lua_State *L);

/********************************/
/*				*/
/* Lua functions		*/
/*				*/
/********************************/

static int l_font_close(lua_State *L)
{
	font_t *font;

	font = l_checkFont(L, 1);

	font_close(font);

	return 0;
}

static int l_font_open(lua_State *L)
{
	btstring_t *fname;
	uint8_t type;
	int flags; 
	font_t **fontp;

	fname = _l_getFilePath(L, 1);
	type = (uint8_t)luaL_checknumber(L, 2);
	flags = luaL_checkint(L, 3);

	fontp = lua_newuserdata(L, sizeof(font_t *));
	luaL_getmetatable(L, "l_sdl_font");
	lua_setmetatable(L, -2);

	*fontp = font_open(fname, type, flags);
	if (!*fontp)
		sdl_error(L);

	return 1;
}

static int l_font_render_text(lua_State *L)
{
	font_t *font;
	const char *text;
	SDL_Color *col;
	SDL_Surface **sur;
	int index = 3;

	font = l_checkFont(L, 1);
	text = luaL_checkstring(L, 2);
	col = sdl_color_arg(L, &index);

	l_surface_new(L);
	sur = l_checkSurfaceP(L, -1);

	*sur = font_render(font, text, col);
	if (!*sur)
		sdl_error(L);

	free(col);

	return 1;
}

static int l_font_size_text(lua_State *L)
{
	font_t *font;
	const char *text;
	int w, h;

	font = l_checkFont(L, 1);
	text = luaL_checkstring(L, 2);

	if (font_size_text(font, text, &w, &h))
		sdl_error(L);

	lua_pushnumber(L, w);
	lua_pushnumber(L, h);

	return 2;
}

font_t *l_checkFont(lua_State *L, int index)
{
	return *(font_t **)luaL_checkudata(L, index, "l_sdl_font");
}

void l_sdl_font_open(lua_State *L)
{
	mod_begin(L, "font");
	mod_function(L,	"Open",		l_font_open);
	mod_function(L,	"Close",	l_font_close);
	mod_constant(L, "FONT_BTF",	FONT_BTF);
	mod_constant(L, "FONT_TTF",	FONT_TTF);

	mod_constant(L, "BTF_MONOSPACE",	BTF_MONOSPACE);
	mod_constant(L, "BTF_VARWIDTH",		BTF_VARWIDTH);
	mod_end(L);

	class_begin(L, "l_sdl_font");
	mod_function(L,	"__gc",		l_font_close);
	mod_function(L, "RenderText",	l_font_render_text);
	mod_function(L, "SizeText",	l_font_size_text);
	class_end(L);
}
