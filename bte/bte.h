#ifndef _BTE_H
#define _BTE_H

#include <btlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <SDL2/SDL.h>
#include <l_module.h>

#define BT_ANIM_EVENT	SDL_USEREVENT
#define BT_SLEEP_EVENT	(SDL_USEREVENT+1)
#define BT_CB_EVENT	(SDL_USEREVENT+2)

#define FONT_BTF	1
#define FONT_TTF	2

extern SDL_Color egapal[];


/********************************/
/*				*/
/* Fwd structure declarations	*/
/*				*/
/********************************/

lua_State	*bt_lua_state(void);
void		bt_lua_doFile(lua_State *L, uint8_t *script);

btstring_t	*_l_getFilePath(lua_State *L, uint8_t index);
FILE		*_l_openFileFP(lua_State *L, uint8_t index, char *flags);


typedef struct font_t font_t;

void sdl_error(lua_State *L);

/* l_anim.c */
void l_sdl_anim_open(lua_State *L);

/* l_color.c */
void l_sdl_color_open(lua_State *L);
SDL_Color *sdl_color_arg(lua_State *L, int *index);

/* l_img.c */
int l_img_load_png(lua_State *L);
int l_img_load_bta(lua_State *L);

/* l_rect.c */
SDL_Rect *sdl_rect_arg(lua_State *L, int *index);
void l_sdl_rect_open(lua_State *L);

/* l_surface.c */
int l_surface_new(lua_State *L);
SDL_Surface *l_checkSurface(lua_State *L, int index);
SDL_Surface *l_testSurface(lua_State *L, int index);
SDL_Surface **l_checkSurfaceP(lua_State *L, int index);

/* l_ttf.c */
void l_sdl_ttf_open(lua_State *L);

/* l_textbox.c */
void l_sdl_textbox_open(lua_State *L);

/* l_font.c */
void l_sdl_font_open(lua_State *L);
font_t *l_checkFont(lua_State *L, int index);
SDL_Surface *sdl_font_render(font_t *font, const char *text, SDL_Color *c);

#endif
