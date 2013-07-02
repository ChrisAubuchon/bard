#include <btlib.h>
#include <font.h>
#include <SDL/SDL_ttf.h>


/********************************/
/*				*/
/* Internal structures		*/
/*				*/
/********************************/

typedef struct font_t font_t;
struct font_t {
	uint8_t type;

	btf_t *btf;
	TTF_Font *ttf;
};

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#define FONT_BTF	1
#define FONT_TTF	2

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

void font_close(font_t *font)
{
	switch (font->type) {
	case FONT_TTF:
		TTF_CloseFont(font->ttf);
		break;
	case FONT_BTF:
		BTF_CloseFont(font->btf);
		break;
	}
}

font_t *font_open(btstring_t *fname,  uint8_t type, int flags)
{
	font_t *font;

	font = (font_t *)xzalloc(sizeof(font_t));

	font->type = type;
	switch (type) {
	case FONT_TTF:
		if (!TTF_WasInit() && TTF_Init() == -1) {
			SDL_SetError(TTF_GetError());
			return NULL;
		}

		font->ttf = TTF_OpenFont(fname->buf, flags);
		if (!font->ttf) {
			SDL_SetError(TTF_GetError());
			return NULL;
		}

		break;
	case FONT_BTF:
		font->btf = BTF_OpenFont(fname, flags);
		break;
	default:
		SDL_SetError("Invalid font type: %d", type);
		return NULL;
	}

	return font;
}

int font_size_text(font_t *font, const char *text, int *w, int *h)
{
	switch (font->type)
	case FONT_TTF:
		if (TTF_SizeText(font->ttf, text, w, h) < 0) {
			SDL_SetError(TTF_GetError());
			return 1;
		break;
	case FONT_BTF:
		BTF_SizeText(font->btf, text, w, h);
		break;
	default:
		SDL_SetError("Invalid font type: %d", font->type);
		return 1;
	}

	return 0;
}

uint8_t font_height(font_t *font)
{
	switch (font->type) {
	case FONT_TTF:
		return TTF_FontHeight(font->ttf);
	case FONT_BTF:
		return BTF_FontHeight(font->btf);
	default:
		SDL_SetError("Invalid font type: %d", font->type);
		return 0;
	}
}

SDL_Surface *font_render(font_t *font, const char *text, SDL_Color *c)
{
	if (!font) {
		SDL_SetError("No font defined!");
		return NULL;
	}

	switch (font->type) {
	case FONT_TTF:
		return TTF_RenderText_Solid(font->ttf, text, *c);
	case FONT_BTF:
		return BTF_RenderText(font->btf, text, *c);
	default:
		SDL_SetError("Invalid font type: %d", font->type);
		return NULL;
	}
}

uint32_t font_wrap(font_t *font, const char *text, uint32_t base, uint32_t max)
{
	if (!font) {
		SDL_SetError("No font defined!");
		return 0;
	}

	switch (font->type) {
	case FONT_TTF:
		SDL_SetError("font_wrap for TTF not implemented");
		return 0;
	case FONT_BTF:
		return BTF_WrapText(font->btf, text, base, max);
	default:
		SDL_SetError("Invalid font type: %d", font->type);
		return 0;
	}
}

uint32_t font_putc(font_t *font, uint32_t c, SDL_Surface *s,
		int x, int y, SDL_Color *fg)
{
	if (!font) {
		SDL_SetError("No font defined!");
		return 0;
	}

	switch (font->type) {
	case FONT_TTF:
		SDL_SetError("font_putc for TTF not implemented");
		return 0;
	case FONT_BTF:
		return BTF_putc(font->btf, c, s, x, y, fg);
	default:
		SDL_SetError("Invalid font type: %d", font->type);
		return 0;
	}
}

void font_glyph_dim(font_t *font, uint32_t c, int *w, int *h)
{
	if (!font) {
		SDL_SetError("No font defined!");
		return;
	}

	switch (font->type) {
	case FONT_TTF:
		SDL_SetError("font_glyph_dim for TTF not implemented");
		return;
	case FONT_BTF:
		return BTF_glyph_dim(font->btf, c, w, h);
	default:
		SDL_SetError("Invalid font type: %d", font->type);
		return;
	}
}
