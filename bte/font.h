#ifndef _FONT_H
#define _FONT_H

#include <btlib.h>
#include <bt_btf.h>
#include <SDL/SDL.h>

#define	FONT_BTF	1
#define FONT_TTF	2

/* Forward structure declarations */
typedef struct font_t font_t;

void		font_close(font_t *font);
font_t *	font_open(btstring_t *fname, uint8_t type, int flags);
int		font_size_text(font_t *font, const char *text, int *w, int *h);
uint8_t		font_height(font_t *font);
uint32_t	font_wrap(font_t *font, const char *text, uint32_t base, 
			uint32_t max);
SDL_Surface * 	font_render(font_t *font, const char *text, SDL_Color *c);

uint32_t 	font_putc(font_t *font, uint32_t c, SDL_Surface *s,
			int x, int y, SDL_Color *fg);
void		font_glyph_dim(font_t *font, uint32_t c, int *w, int *h);


SDL_Surface 	*BTF_RenderText(btf_t *btf, const char *text, SDL_Color color);
uint32_t 	BTF_WrapText(btf_t *btf, const char *text, uint32_t base, 
			uint32_t max);
uint32_t 	BTF_putc(btf_t *btf, uint32_t c, SDL_Surface *s, int x, int y, 
			SDL_Color *fg);
void		BTF_glyph_dim(btf_t *btf, uint32_t c, int *w, int *h);

#endif
