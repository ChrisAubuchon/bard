#include <bte.h>
#include <l_sdl.h>

/*
 * SDL_BlitSurface_r()
 *
 * Call SDL_BlitSurface without the side effect of changing
 * the destination rectangle
 */
int SDL_BlitSurface_r(SDL_Surface *src, SDL_Rect *srcrect,
		      SDL_Surface *dst, SDL_Rect *dstrect)
{
	SDL_Rect	tmprect;

	if (dstrect != NULL) {
		tmprect.x = dstrect->x;
		tmprect.y = dstrect->y;
		tmprect.w = dstrect->w;
		tmprect.h = dstrect->h;

		return SDL_BlitSurface(src, srcrect, dst, &tmprect);
	} else {
		return SDL_BlitSurface(src, srcrect, dst, dstrect);
	}
}
