#ifndef _LUA_L_SDL_H
#define _LUA_L_SDL_H



/********************************/
/*				*/
/* Functions			*/
/*				*/
/********************************/

/* l_color.c */
void		l_checkColor(lua_State *L, int index, SDL_Color *rwColor);

/* l_font.c */
void		l_sdl_font_open(lua_State *L);

/* l_rect.c */
void		l_sdl_rect_open(lua_State *L);
int		l_rect_new(lua_State *L);
SDL_Rect	*l_testRect(lua_State *L, int index);
SDL_Rect	*l_checkRect(lua_State *L, int index);
 
/* l_renderer.c */
void		l_sdl_renderer_open(lua_State *L);
int		l_renderer_new(lua_State *L);
SDL_Renderer	*l_testRenderer(lua_State *L, int index);
SDL_Renderer	*l_checkRenderer(lua_State *L, int index);
SDL_Renderer	**l_checkRendererP(lua_State *L, int index);

/* l_surface.c */
void		l_sdl_surface_open(lua_State *L);
int		l_surface_new(lua_State *L);
SDL_Surface	*l_checkSurface(lua_State *L, int index);
SDL_Surface	*l_testSurface(lua_State *L, int index);
SDL_Surface	**l_checkSurfaceP(lua_State *L, int index);

/* l_textbox.c */
void		l_sdl_textbox_open(lua_State *L);

/* l_texture.c */
void		l_sdl_texture_open(lua_State *L);
int		l_texture_new(lua_State *L);
SDL_Texture	*l_testTexture(lua_State *L, int index);
SDL_Texture	*l_checkTexture(lua_State *L, int index);
SDL_Texture	**l_checkTextureP(lua_State *L, int index);

/* l_window.c */
void		l_sdl_window_open(lua_State *L);
int		l_window_new(lua_State *L);
SDL_Window	*l_testWindow(lua_State *L, int index);
SDL_Window	*l_checkWindow(lua_State *L, int index);
SDL_Window	**l_checkWindowP(lua_State *L, int index);

/* sdl_util.c */
int		SDL_BlitSurface_r(SDL_Surface *src, SDL_Rect *srcrect, \
				  SDL_Surface *dst, SDL_Rect *dstrect);

#if 0
#include <btlib.h>
#include <bt_bta.h>
#include <bt_btf.h>
<<<<<<< HEAD
#include <SDL/SDL.h>
=======
#include <SDL2/SDL.h>
>>>>>>> 18c3a63c7e03ebf53e3f3d4d212b248963194f17
#include <sdl.h>

#define IMG_SURFACE	0
#define IMG_PNG		1
#define IMG_BTA_SIMPLE	2
#define IMG_BTA_XOR	3

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



/* l_ttf.c */
void l_sdl_ttf_open(lua_State *L);

/* l_textbox.c */
void l_sdl_textbox_open(lua_State *L);

/* l_font.c */
void l_sdl_font_open(lua_State *L);
font_t *l_checkFont(lua_State *L, int index);
SDL_Surface *sdl_font_render(font_t *font, const char *text, SDL_Color *c);

/* l_rect.c */
void		l_sdl_rect_open(lua_State *L);
SDL_Rect	*l_testRect(lua_State *L, int index);
SDL_Rect	*l_checkRect(lua_State *L, int index);

/* l_renderer.c */
void		l_sdl_renderer_open(lua_State *L);
int		l_renderer_new(lua_State *L);
SDL_Renderer	*l_testRenderer(lua_State *L, int index);
SDL_Renderer	*l_checkRenderer(lua_State *L, int index);
SDL_Renderer	**l_checkRendererP(lua_State *L, int index);

/* l_window.c */
void		l_sdl_window_open(lua_State *L);
int		l_window_new(lua_State *L);
SDL_Window	*l_testWindow(lua_State *L, int index);
SDL_Window	*l_checkWindow(lua_State *L, int index);
SDL_Window	**l_checkWindowP(lua_State *L, int index);

#endif

#endif
