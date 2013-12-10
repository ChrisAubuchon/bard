#include <bte.h>
#include <bt_btf.h>
#include <font.h>

/*#define DEBUG 1*/
#include <debug.h>

/********************************/
/*				*/
/* Text line structure		*/
/*				*/
/********************************/

SDL_Surface *BTF_RenderText(btf_t *btf, const char *text, SDL_Color color)
{
	const char *textp = text;
	btstring_t *c;
	int width;
	int w, h;
	SDL_Surface *rval;
	SDL_Palette *palette;
	int base_x = 0;
	int x,y;
	uint8_t *srcp;
	uint8_t *pixp;

	BTF_SizeText(btf, text, &w, &h);

	/* Create the surface */
	rval = SDL_CreateRGBSurface(0, w, h, 8, 0, 0, 0, 0);
	if (rval == NULL) {
		return NULL;
	}
	debug("rval->format = %s\n", SDL_GetPixelFormatName(rval->format->format));
	palette = rval->format->palette;
	palette->colors[0].r = 255 - color.r;
<<<<<<< HEAD
	palette->colors[0].g = 255 - color.g;
	palette->colors[0].b = 255 - color.b;
	palette->colors[0].a = 0;
	palette->colors[1].r = color.r;
	palette->colors[1].g = color.g;
	palette->colors[1].b = color.b;
	palette->colors[1].a = 255;
   	SDL_SetColorKey(rval, SDL_TRUE, 0);

	debug("rval->format->palette[0] = { %3d, %3d, %3d }\n",
		rval->format->palette->colors[0].r,
		rval->format->palette->colors[0].g,
		rval->format->palette->colors[0].b
	);
	debug("rval->format->palette[1] = { %3d, %3d, %3d }\n",
		rval->format->palette->colors[1].r,
		rval->format->palette->colors[1].g,
		rval->format->palette->colors[1].b
	);
=======
   	palette->colors[0].g = 255 - color.g;
   	palette->colors[0].b = 255 - color.b;
   	palette->colors[1].r = color.r;
   	palette->colors[1].g = color.g;
   	palette->colors[1].b = color.b;
#if 0
   	SDL_SetColorKey(rval, SDL_SRCCOLORKEY, 0 );
#endif

>>>>>>> 18c3a63c7e03ebf53e3f3d4d212b248963194f17

	if (SDL_MUSTLOCK(rval))
		SDL_LockSurface(rval);

	while ((*textp != '\0') && (*textp != '\n')) {

		/* Maybe just skip unknown characters... */
		btf_getChar(btf, *textp, &width, &c);

		if (width == 0) {
			textp++;
			continue;
		}

		if (btf->type == BTF_MONOSPACE)
			width = btf->monoWidth;

		pixp = (uint8_t *)rval->pixels + base_x;
		srcp = c->buf;

		for (y = 0; y < btf->height; y++) {
			for (x = 0; x < width; x++) 
				*pixp++ |= *srcp++;

			pixp += (rval->pitch - width);
			srcp += (btf->monoWidth - width);
		}

		base_x += width;
		textp++;
	}

	if (SDL_MUSTLOCK(rval))
		SDL_UnlockSurface(rval);

	return rval;
}

uint32_t BTF_WrapText(btf_t *btf, const char *text, uint32_t base, \
			uint32_t max)
{
	uint32_t size = 0;
	uint32_t nchars = 0;
	uint32_t width;
	btstring_t *bs;
	const char *textp;

	textp = &text[base];

	while ((*textp != '\0') && (*textp != '\n')) {

		btf_getChar(btf, *textp, &width, &bs);

		/* Skip unprintable characters */
		if (width == 0) {
			textp++;
			continue;
		}

		if (btf->type == BTF_VARWIDTH)
			size += width;
		else
			size += btf->monoWidth;

		nchars++;
		textp++;

		/* Break out after we've exceeded the max width */
		if (size > max)
			break;

	}

	if (size <= max)
		return nchars;

	while ((*--textp != ' ') && (nchars))
		nchars--;

	return nchars;
}

uint32_t BTF_putc(btf_t *btf, uint32_t c, SDL_Surface *s, int in_x, int in_y,
			SDL_Color *fg)
{
	btstring_t *str;
	int x, y;
	int width;
	uint8_t *pixp;
	uint8_t *srcp;

	/* Skip invalid characters */
	if (c > btf_getNumChars(btf))
		return 0;

	btf_getChar(btf, c, &width, &str);

	/* Skip characters not in the font */
	if (width == 0)
		return 0;

	if (btf->type == BTF_MONOSPACE) {
		width = btf->monoWidth;
	}

	if (SDL_MUSTLOCK(s))
		SDL_LockSurface(s);

	pixp = s->pixels + (in_y * s->pitch) + in_x;
	srcp = str->buf;

	for (y = 0; y < btf->height; y++) {
		for (x = 0; x < width; x++) {
			*pixp++ = *srcp++;
		}

		pixp += (s->pitch - width);
		srcp += (btf->monoWidth - width);
	}

	if (SDL_MUSTLOCK(s))
		SDL_UnlockSurface(s);

	return width;
}

void BTF_glyph_dim(btf_t *btf, uint32_t c, int *w, int *h)
{
	btstring_t *bs;


	if (c > btf_getNumChars(btf)) {
		*h = 0;
		*w = 0;
		return;
	}

	btf_getChar(btf, c, w, &bs);
	if (*w == 0) {
		*h = 0;
		return;
	}

	*h = btf->height;
	if (btf->type == BTF_MONOSPACE) {
		*w = btf->monoWidth;
	}
}
