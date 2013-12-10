#include <sdl.h>

int SDL_FillRect_RC(SDL_Surface *dst, Sint32 x, Sint32 y, Sint32 w, Sint32 h, Uint32 color)
{
	SDL_Rect r;

	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;

/*	return SDL_FillRect(dst, &r, color);*/
}

/********************************/
/*				*/
/* _RP functions		*/
/*				*/
/********************************/

void SDL_UpdateRect_RP(SDL_Surface *screen, SDL_Rect *r)
{
#if 0
	if (r != NULL)
		SDL_UpdateRect(screen, r->x, r->y, r->w, r->h);
	else
		SDL_UpdateRect(screen, 0, 0, 0, 0);
#endif
}

/*
 * This version of SDL_BlitSurface does NOT modify destrect
 */
int SDL_BlitSurface_RP(SDL_Surface *src, SDL_Rect *srcrect, \
		       SDL_Surface *dest, SDL_Rect *destrect)
{
	SDL_Rect r;

	if (destrect != NULL) {
		r.x = destrect->x;
		r.y = destrect->y;
		r.w = destrect->w;
		r.h = destrect->h;

		return SDL_BlitSurface(src, srcrect, dest, &r);
	} else {
		return SDL_BlitSurface(src, srcrect, dest, destrect);
	}
}
