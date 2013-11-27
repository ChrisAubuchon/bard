#include <bte.h>
#include <SDL2/SDL_image.h>
#include <l_sdl.h>

/*#define DEBUG 1*/
#include <debug.h>

/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/


int l_img_load_png(lua_State *L)
{
	/* args */
	btstring_t	*fname;

	SDL_RWops	*ops;
	SDL_Surface	**surface;
	SDL_Surface	*img_surface;

	fname = _l_getFilePath(L, 1);

	ops = SDL_RWFromFile(fname->buf, "r");
	bts_free(fname);

	lua_pop(L, 1);

	l_surface_new(L);
	surface = l_checkSurfaceP(L, -1);
	img_surface = IMG_LoadTyped_RW(ops, 1, "png");
	if (img_surface == NULL)  {
		luaL_error(L, IMG_GetError());
		exit(1);
	}

	*surface = img_surface;

#if CONVERT_TO_RGBA
	/*
	 * PNG files are always stored in RGBA format. SDL_Image
	 * creates the surface based on the machines endian-ness. This
	 * can lead to incorrect displaying of the images. Here we 
	 * create a RGBA surface and convert.
	 */
	*surface = SDL_CreateRGBSurface(0, img_surface->w, img_surface->h,
		(img_surface->pitch / img_surface->w) * 8,
		0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	if (*surface == NULL) 
		sdl_error(L);
	
	if (SDL_BlitSurface(img_surface, NULL, *surface, NULL))
		sdl_error(L);

	SDL_FreeSurface(img_surface);
#endif

	return 1;
}
