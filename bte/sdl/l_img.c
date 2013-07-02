#include <btlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <l_int.h>
#include <l_sdl.h>

#define DEBUG 1
#include <debug.h>

/********************************/
/*				*/
/* Helper macros		*/
/*				*/
/********************************/

SDL_Color egapal[] = {
	{ 0,	0,	0,	0	},
	{ 0, 	0,	170,	0	},
	{ 0, 	170,	0,	0	},
	{ 0, 	170,	170,	0	},
	{ 170, 	0,	0,	0	},
	{ 170, 	0,	170,	0	},
	{ 170, 	85,	0,	0	},
	{ 170, 	170,	170,	0	},
	{ 85, 	85,	85,	0	},
	{ 85, 	85,	255,	0	},
	{ 85, 	255,	85,	0	},
	{ 85, 	255,	255,	0	},
	{ 255, 	85,	85,	0	},
	{ 255, 	85,	255,	0	},
	{ 255, 	255,	85,	0	},
	{ 255, 	255,	255,	0	}
};

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


int l_img_load_png(lua_State *L)
{
	btstring_t *fname;
	SDL_RWops *ops;
	SDL_Surface **s;

	fname = _l_getFilePath(L, 1);

	ops = SDL_RWFromFile(fname->buf, "r");
	bts_free(fname);

	lua_pop(L, 1);

	l_surface_new(L);
	s = l_checkSurfaceP(L, -1);
	*s = IMG_LoadTyped_RW(ops, 1, "png");
	if (*s == NULL)  {
		luaL_error(L, IMG_GetError());
		exit(1);
	}
	
	return 1;
}
	
#if 0
int l_img_load_bta(lua_State *L)
{
	btstring_t *fname;
	l_anim *a;
	bta_cell_t *c;

	fname = _l_getFilePath(L, 1);

	l_anim_new(L);
	a = l_checkAnim(L, -1);
	a->bta = bta_read_bta(fname);

	if (a->bta->type == BTA_TYPE_SIMPLELOOP) {
		c = bta_cell_get(a->bta, 0, 0);
	} else {
		c = a->bta->base;
	}

	a->s = SDL_CreateRGBSurface(SDL_SWSURFACE, c->width, c->height,\
					 8, 0, 0, 0, 0);
	SDL_SetColors(a->s, egapal, 0, 16); 

	return 1;
}
#endif
