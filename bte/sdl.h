#ifndef _BT_SDL_H
#define _BT_SDL_H

#include <btlib.h>
#include <SDL/SDL.h>

/*
 * This header file is to make the SDL library a bit 
 * more consistent in its functions.
 *
 * Functions ending with _RC take rectangle coordinates as 
 * the SDL_Rect argument.
 *
 * Functions ending with _RP take an SDL_Rect * as the SDL_Rect argument.
 */

/*
 * _RC functions
 */
#define SDL_UpdateRect_RC	SDL_UpdateRect
int SDL_FillRect_RC(SDL_Surface *dst, Sint32 x, Sint32 y, Sint32 w, Sint32 h, Uint32 color);

/*
 * _RP functions
 */
void SDL_UpdateRect_RP(SDL_Surface *screen, SDL_Rect *r);
#define SDL_FillRect_RP		SDL_FillRect
int SDL_BlitSurface_RP(SDL_Surface *src, SDL_Rect *srcrect, \
		       SDL_Surface *dest, SDL_Rect *destrect);


#endif
