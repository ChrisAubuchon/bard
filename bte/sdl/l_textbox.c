#include <bte.h>
#include <font.h>
#include <l_sdl.h>

/*#define DEBUG 1*/
#include <debug.h>

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#define BG_INDEX	0
#define FG_INDEX	1
#define HG_INDEX	2

/********************************/
/*				*/
/* Textbox structure		*/
/*				*/
/********************************/

typedef struct {
	lua_State	*errorState;
	SDL_Renderer	*renderer;
	SDL_Texture	*texture;
	SDL_Surface	*screen;
	SDL_Rect	*rect;
	SDL_Surface	*s;
	font_t		*font;
	uint32_t	maxChar;
	uint32_t	x;		/* Cursor x coordinate		*/
	uint32_t	y;		/* Cursor y coordinate		*/
	uint32_t	ws;
	uint32_t	nchars;	
	uint32_t	max;
} l_textbox;

/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

/* Helper functions */
static void		set_color(l_textbox *tb, uint8_t index, SDL_Color *c);
static void		get_color(l_textbox *tb, uint8_t index, SDL_Color *c);
static l_textbox	*l_checkTextbox(lua_State *L, int index);
static void		_textbox_newline(l_textbox *tb);
static void		_textbox_scroll(l_textbox *tb);
static void		_textbox_highlight(l_textbox *tb, uint8_t srci,
				uint8_t desti, uint32_t lineno);
static uint32_t		_textbox_wrap(l_textbox *tb, btstring_t *str,
				uint32_t base);
static void		_update_screen(l_textbox *tb);

/* Class methods */
static int		l_textbox_free(lua_State *L);
static int		l_textbox_getbg(lua_State *L);
static int		l_textbox_setbg(lua_State *L);
static int		l_textbox_getfg(lua_State *L);
static int		l_textbox_setfg(lua_State *L);
static int		l_textbox_gethg(lua_State *L);
static int		l_textbox_sethg(lua_State *L);
static int		l_textbox_draw(lua_State *L);
static int		l_textbox_clear(lua_State *L);

static int		l_textbox_setcursor(lua_State *L);
static int		l_textbox_getcursor(lua_State *L);
static int		l_textbox_setfont(lua_State *L);
static int		l_textbox_print(lua_State *L);
static int		l_textbox_highlight(lua_State *L);
static int		l_textbox_unhighlight(lua_State *L);
static int		l_textbox_putc(lua_State *L);
static int		l_textbox_erasechar(lua_State *L);
static int		l_textbox_clearline(lua_State *L);

/* Module methods */
static int		l_textbox_new(lua_State *L);

/********************************/
/*				*/
/* Helper functions		*/
/*				*/
/********************************/

static l_textbox *l_checkTextbox(lua_State *L, int index)
{
	return (l_textbox *)luaL_checkudata(L, index, "l_sdl_textbox");
}

static void set_color(l_textbox *tb, uint8_t index, SDL_Color *c)
{
	if (SDL_SetPaletteColors(tb->s->format->palette, c, index, 1))
		sdl_error(tb->errorState);
}

static void get_color(l_textbox *tb, uint8_t index, SDL_Color *c)
{
	c->r = tb->s->format->palette->colors[index].r;
	c->g = tb->s->format->palette->colors[index].g;
	c->b = tb->s->format->palette->colors[index].b;
}

static void _textbox_scroll(l_textbox *tb)
{
	SDL_Rect	r;

	r.x = 0;	
	r.y = font_height(tb->font) / 2;
	r.w = tb->s->w;
	r.h = tb->s->h - font_height(tb->font) / 2;
	if (SDL_BlitSurface(tb->s, &r, tb->s, NULL))
		sdl_error(tb->errorState);

	r.y = r.h;
	r.h = font_height(tb->font) / 2;
	if (SDL_FillRect(tb->s, &r, BG_INDEX))
		sdl_error(tb->errorState);

	_update_screen(tb);
}

static void _textbox_newline(l_textbox *tb)
{
	SDL_Rect r;

	/* Update the cursor */
	tb->x = 0;
	tb->y += font_height(tb->font);

	/* Clear the whitespace and number of chars counters */
	tb->ws = 0;
	tb->max = tb->maxChar;
	tb->nchars = 0;

	/* Scroll the text */
	if (tb->y >= tb->rect->h) {
		_textbox_scroll(tb);
		_textbox_scroll(tb);

		tb->y -= font_height(tb->font);
	}
}

static void _textbox_highlight(l_textbox *tb, uint8_t srci, uint8_t desti, \
				uint32_t lineno)
{
	uint32_t x, y;
	uint8_t *pixels;

	if (SDL_MUSTLOCK(tb->s))
		SDL_LockSurface(tb->s);
	
	pixels = (uint8_t *)tb->s->pixels + \
		 ((lineno * font_height(tb->font)) * tb->s->pitch);

	for (y = 0; y < font_height(tb->font); y++) {
		for (x = 0; x < tb->s->pitch; x++) {
			if (*pixels == srci)
				*pixels = desti;
			pixels++;
		}
	}	

	if (SDL_MUSTLOCK(tb->s))
		SDL_UnlockSurface(tb->s);

	_update_screen(tb);
}

/*
 * _update_screen()
 */
static void _update_screen(l_textbox *tb)
{
	if (SDL_BlitSurface_r(tb->s, NULL, tb->screen, tb->rect))
		sdl_error(tb->errorState);
	if (SDL_UpdateTexture(
			tb->texture, NULL,
			tb->screen->pixels,  tb->screen->pitch
			))
		sdl_error(tb->errorState);

	if (SDL_RenderCopy(tb->renderer, tb->texture, NULL, NULL))
		sdl_error(tb->errorState);

	SDL_RenderPresent(tb->renderer);
}

/*
 * _textbox_wrap()
 * Wrap text in the text window
 *
 * This matches the algorithm used by the Bard's Tale DOS version.
 * Basically there are a maximum of 21 characters on the line. The 
 * maximum is adjusted downward by the number of characters that are
 * not a space, not lower case or 'm'. For every three of those types
 * of characters, the maximum number of characters allowed is reduced
 * by one.
 *
 * 20 looks like it wraps like the original
 */
/*#define MAX_CHAR	21*/
/*#define MAX_CHAR	20*/
static uint32_t _textbox_wrap(l_textbox *tb, btstring_t *str, uint32_t base)
{
	uint32_t nchars = 0;
	uint32_t ncaps = 0;
	uint8_t *textp;

	textp = &str->buf[base];

	while ((*textp != '\0') && (*textp != '\n')) {
		if ((tb->nchars + nchars) > tb->maxChar) 
			break;

		if ((*textp == 'm') || ((*textp != ' ') && (!islower(*textp)))){
			ncaps++;

			tb->max = tb->max - (((ncaps * 2) + tb->ws + 4) / 6);
		}
		nchars++;
		textp++;
	}

	if ((tb->nchars + nchars) < tb->max)
		return nchars;

	while ((*--textp != ' ') && (nchars))
		nchars--;

	return nchars;
}

/********************************/
/*				*/
/* Class functions		*/
/*				*/
/********************************/

/*
 * l_textbox_new()
 */
static int l_textbox_new(lua_State *L)
{
	l_textbox	*tb;

	tb = lua_newuserdata(L, sizeof(l_textbox));
	luaL_getmetatable(L, "l_sdl_textbox");
	lua_setmetatable(L, -2);

	tb->errorState	= L;
	tb->renderer	= l_checkRenderer(L, 1);
	tb->texture	= l_checkTexture(L, 2);
	tb->screen	= l_checkSurface(L, 3);
	tb->rect	= l_checkRect(L, 4);
	tb->maxChar	= (uint32_t)luaL_checkinteger(L, 5);
	tb->max		= tb->maxChar;
	tb->font = NULL;

	tb->x = 0;
	tb->y = 0;
	tb->ws = 0;
	tb->nchars = 0;

	if (tb->rect) {
		tb->s = SDL_CreateRGBSurface(0, tb->rect->w, tb->rect->h,
					     8, 0, 0, 0, 0);
		if (tb->s == NULL)
			sdl_error(L);
	} else {
		luaL_error(L, "Rectangle not specified for textbox");
	}

	return 1;
}


static int l_textbox_getbg(lua_State *L)
{
	l_textbox	*tb;
	SDL_Color	c;

	tb = l_checkTextbox(L, 1);
	get_color(tb, BG_INDEX, &c);

	lua_newtable(L);

	lua_pushnumber(L, 1);
	lua_pushnumber(L, c.r);
	lua_rawset(L, -3);
	lua_pushnumber(L, 2);
	lua_pushnumber(L, c.g);
	lua_rawset(L, -3);
	lua_pushnumber(L, 3);
	lua_pushnumber(L, c.b);
	lua_rawset(L, -3);

	return 1;
}

static int l_textbox_setbg(lua_State *L)
{
	l_textbox	*tb;
	SDL_Color	c;

	tb = l_checkTextbox(L, 1);
	l_checkColor(L, 2, &c);

	set_color(tb, BG_INDEX, &c);

	return 0;
}

static int l_textbox_getfg(lua_State *L)
{
	l_textbox	*tb;
	SDL_Color	c;

	tb = l_checkTextbox(L, 1);
	get_color(tb, FG_INDEX, &c);

	lua_newtable(L);

	lua_pushnumber(L, 1);
	lua_pushnumber(L, c.r);
	lua_rawset(L, -3);
	lua_pushnumber(L, 2);
	lua_pushnumber(L, c.g);
	lua_rawset(L, -3);
	lua_pushnumber(L, 3);
	lua_pushnumber(L, c.b);
	lua_rawset(L, -3);

	return 1;
}

static int l_textbox_setfg(lua_State *L)
{
	l_textbox	*tb;
	SDL_Color	c;

	tb = l_checkTextbox(L, 1);
	l_checkColor(L, 2, &c);

	set_color(tb, FG_INDEX, &c);

	return 0;
}

static int l_textbox_gethg(lua_State *L)
{
	l_textbox	*tb;
	SDL_Color	c;

	tb = l_checkTextbox(L, 1);
	get_color(tb, HG_INDEX, &c);

	lua_newtable(L);

	lua_pushnumber(L, 1);
	lua_pushnumber(L, c.r);
	lua_rawset(L, -3);
	lua_pushnumber(L, 2);
	lua_pushnumber(L, c.g);
	lua_rawset(L, -3);
	lua_pushnumber(L, 3);
	lua_pushnumber(L, c.b);
	lua_rawset(L, -3);

	return 1;
}

static int l_textbox_sethg(lua_State *L)
{
	l_textbox	*tb;
	SDL_Color	c;

	tb = l_checkTextbox(L, 1);
	l_checkColor(L, 2, &c);

	set_color(tb, HG_INDEX, &c);

	return 0;
}

static int l_textbox_draw(lua_State *L)
{
	l_textbox *tb = l_checkTextbox(L, 1);

	_update_screen(tb);

	return 0;
}

static int l_textbox_clear(lua_State *L)
{
	l_textbox *tb = l_checkTextbox(L, 1);

	if (SDL_FillRect(tb->s, NULL, BG_INDEX) < 0)
		sdl_error(L);

	_update_screen(tb);

	/* Reset the cursor */
	tb->x = 0;
	tb->y = 0;
	tb->ws = 0;
	tb->nchars = 0;
	tb->max = tb->maxChar;
}

static int l_textbox_free(lua_State *L)
{
	l_textbox *tb = l_checkTextbox(L, 1);

	SDL_FreeSurface(tb->s);

	return 0;
}

static int l_textbox_setcursor(lua_State *L)
{
	l_textbox *tb;

	tb = l_checkTextbox(L, 1);
	tb->x = luaL_checkinteger(L, 2);
	tb->y = luaL_checkinteger(L, 3) * font_height(tb->font);

	tb->ws = 0;
	tb->nchars = 0;

	return 0;
}

static int l_textbox_getcursor(lua_State *L)
{
	l_textbox *tb;

	tb = l_checkTextbox(L, 1);

	lua_pushnumber(L, tb->x);
	lua_pushnumber(L, tb->y / font_height(tb->font));

	return 2;
}

static int l_textbox_setfont(lua_State *L)
{
	l_textbox *tb;
	font_t *font;

	tb = l_checkTextbox(L, 1);
	font = l_checkFont(L, 2);

	tb->font = font;

	return 0;
}

static int l_textbox_print(lua_State *L)
{
	l_textbox	*tb;
	btstring_t	*text;
	const char	*instring;
	SDL_Surface	*s;
	SDL_Color	fg;
	SDL_Rect	srect, prect;
	uint32_t	nchars;
	uint32_t	index = 0;

	tb = l_checkTextbox(L, 1);
	text = bts_strcpy(luaL_checkstring(L, 2));

	get_color(tb, FG_INDEX, &fg);

	debug("text: '%s'\n", text->buf);
	while (index < text->size - 1) {
		btstring_t *tmp;
#if 0
		/*
		 * Bard's Tale doesn't use the width of the font
		 * character's to do text wrapping.
		 */
		nchars = font_wrap(tb->font, text->buf, index, \
					(tb->rect->w - tb->x));
#endif
		nchars = _textbox_wrap(tb, text, index);

		while ((tb->x == 0) && (text->buf[index] == ' '))
			index++;

		if (nchars) {
			tmp = bts_strncpy(&text->buf[index], nchars);
			debug("tmp = '%s'\n", tmp->buf);

			s = font_render(tb->font, tmp->buf, &fg);
			if (!s)
				sdl_error(L);

			srect.x = tb->x;
			srect.y = tb->y;
			srect.w = s->w;
			srect.h = s->h;

			if (SDL_BlitSurface_r(s, NULL, tb->s, &srect))
				sdl_error(L);
			_update_screen(tb);

			tb->x += s->w;

			index += nchars;
			tb->nchars += nchars;

			bts_free(tmp);
			SDL_FreeSurface(s);
		}

		/* Newline in the text. Reset x and increment y */
		if (text->buf[index] == '\n') {
			index++;

			_textbox_newline(tb);
		} else if (index < text->size - 1) {
			/* Line wrapped. Reset x and increment y */
			_textbox_newline(tb);
		} 

	}

	bts_free(text);

	return 0;
}

static int l_textbox_putc(lua_State *L)
{
	l_textbox *tb;
	uint32_t c;
	SDL_Color fg;
	uint32_t width;
	SDL_Rect srect, prect;

	tb = l_checkTextbox(L, 1);
	c = luaL_checkint(L, 2);

	get_color(tb, FG_INDEX, &fg);

	width = font_putc(tb->font, c, tb->s, tb->x, tb->y, &fg);

	_update_screen(tb);

	tb->x += width;

	lua_pushnumber(L, width);

	return 1;
}

/*
 * l_textbox_erasechar()
 * Erase a character in the text window.
 *
 * Args:
 *   1 - Text box userdata
 *   2 - Numerical value of character to erase
 *   3 - Starting x coordinate
 *   4 - Flag to erase the current character or the previous character
 *       (default is current character)
 *
 * If the 4th argument is non-zero, then the width of the given character
 * is subtracted from the the x coordinate. This allows us to easily erase
 * the cursor glyph and the previous character.
 */
static int l_textbox_erasechar(lua_State *L)
{
	l_textbox *tb;
	uint32_t c;
	SDL_Color bg;
	int w, h;
	int x, goback;
	SDL_Rect srect, prect;

	tb = l_checkTextbox(L, 1);
	c = luaL_checkint(L, 2);
	x = luaL_checkint(L, 3);
	goback = luaL_optint(L, 4, 0);

	font_glyph_dim(tb->font, c, &w, &h);
	srect.x = x - ((goback) ? w : 0);
	srect.y = tb->y;
	srect.w = w;
	srect.h = h;
	if (SDL_FillRect(tb->s, &srect, BG_INDEX))
		sdl_error(L);

	_update_screen(tb);

	lua_pushnumber(L, w);

	return 1;
}

/*
 * l_textbox_clearline()
 * Clear the line that the cursor is on
 */
static int l_textbox_clearline(lua_State *L)
{
	l_textbox *tb;
	SDL_Rect srect;

	tb = l_checkTextbox(L, 1);

	srect.x = 0;
	srect.y = tb->y;
	srect.w = tb->rect->w;
	srect.h = font_height(tb->font);
	if (SDL_FillRect(tb->s, &srect, BG_INDEX))
		sdl_error(L);

	_update_screen(tb);

	return 0;
}

static int l_textbox_highlight(lua_State *L)
{
	int x, y;
	int lineno;
	uint8_t *pixels;
	l_textbox *tb;

	tb = l_checkTextbox(L, 1);
	lineno = luaL_checkint(L, 2);

	/* Make sure the line number is in the window */
	if (lineno * font_height(tb->font) >= tb->s->h)
		return 0;

	_textbox_highlight(tb, BG_INDEX, HG_INDEX, lineno);

	return 0;
}

static int l_textbox_unhighlight(lua_State *L)
{
	int lineno;
	l_textbox *tb;

	tb = l_checkTextbox(L, 1);
	lineno = luaL_checkint(L, 2);

	/* Make sure the line number is in the window */
	if (lineno * font_height(tb->font) >= tb->s->h)
		return 0;

	_textbox_highlight(tb, HG_INDEX, BG_INDEX, lineno);
	
	return 0;
}

/********************************/
/*				*/
/* Lua functions		*/
/*				*/
/********************************/


void l_sdl_textbox_open(lua_State *L)
{
	class_begin(L, "l_sdl_textbox");
	mod_function(L,	"__gc",		l_textbox_free);
	mod_variable(L, "bg_color",	l_textbox_getbg, l_textbox_setbg);
	mod_variable(L, "fg_color",	l_textbox_getfg, l_textbox_setfg);
	mod_variable(L, "hg_color",	l_textbox_gethg, l_textbox_sethg);
	mod_function(L, "Draw",		l_textbox_draw);
	mod_function(L, "Clear",	l_textbox_clear);
	mod_function(L, "SetCursor",	l_textbox_setcursor);
	mod_function(L, "GetCursor",	l_textbox_getcursor);
	mod_function(L, "SetFont",	l_textbox_setfont);
	mod_function(L, "Print",	l_textbox_print);
	mod_function(L, "Highlight",	l_textbox_highlight);
	mod_function(L, "UnHighlight",	l_textbox_unhighlight);
	mod_function(L, "putc",		l_textbox_putc);
	mod_function(L, "eraseChar",	l_textbox_erasechar);
	mod_function(L, "clearLine",	l_textbox_clearline);
	class_end(L);

	mod_function(L,	"NewTextbox",	l_textbox_new);
}
