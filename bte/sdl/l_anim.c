#include <btlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <l_int.h>
#include <l_sdl.h>
#include <gl_list.h>
#include <gl_array_list.h>

#define DEBUG 1
#include <debug.h>


/********************************/
/*				*/
/* Animation struture		*/
/*				*/
/********************************/

typedef struct {
	SDL_Surface	*dest;
	SDL_Rect	*dest_rect;
	SDL_Surface	*s;
	bta_t 		*bta;
	gl_list_t	loopData;
} l_anim;

typedef struct {
	l_anim		*anim;
	uint32_t	c_cell;
	uint32_t	loopNumber;
	SDL_TimerID	timer;
} l_loop_t;
	

/********************************/
/*				*/
/* Helper macros		*/
/*				*/
/********************************/

#define TO_SDL_ANIM(state, index) (l_anim *)luaL_checkudata(state, \
						index, "l_sdl_anim")

/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

#if 0
static uint32_t xorCallback(uint32_t interval, void *param);
static void xorAnim(void *data);
#endif
static uint32_t simpleCallback(uint32_t interval, void *param);
static void simpleAnim(void *data);
static void l_loop_free(const void *data);

static int l_anim_isAnim(lua_State *L);
static int l_anim_get_w(lua_State *L);
static int l_anim_get_h(lua_State *L);
static int l_anim_free(lua_State *L);
static int l_anim_tostring(lua_State *L);
static int l_anim_start(lua_State *L);
static int l_anim_top(lua_State *L);

static int l_anim_new(lua_State *L);
static l_anim *l_checkAnim(lua_State *L, int index);
static l_anim *l_testAnim(lua_State *L, int index);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

static void l_loop_free(const void *data)
{
	l_loop_t *loop = (l_loop_t *)data;
	free(loop);
}

static int l_anim_isAnim(lua_State *L)
{
/*	lua_pushnumber(L, 1);*/
	lua_pushboolean(L, 1);
	return 1;
}

static int l_anim_get_w(lua_State *L)
{
	l_anim *anim = TO_SDL_ANIM(L, 1);

	lua_pushnumber(L, anim->s->w);

	return 1;
}

static int l_anim_get_h(lua_State *L)
{
	l_anim *anim = TO_SDL_ANIM(L, 1);

	lua_pushnumber(L, anim->s->h);

	return 1;
}

static uint32_t simpleCallback(uint32_t interval, void *param)
{
	SDL_Event e;
	SDL_UserEvent u;
	l_loop_t *l = (l_loop_t *)param;
	bta_cell_t *c;
	uint32_t	cell;

#if 0
	l->c_cell++;
	if (l->c_cell == bta_getNumCells(l->anim->bta, l->loopNumber))
		l->c_cell = 0;
#endif
	cell = l->c_cell + 1;
	if (cell == bta_getNumCells(l->anim->bta, l->loopNumber))
		cell = 0;

	u.type = BT_ANIM_EVENT;
	u.data1 = simpleAnim;
	u.data2 = param;

	e.type = BT_ANIM_EVENT;
	e.user = u;

	SDL_PushEvent(&e);

#if 0
	c = bta_cell_get(l->anim->bta, l->loopNumber, l->c_cell);
#endif
	c = bta_cell_get(l->anim->bta, l->loopNumber, cell);

	return c->delay;
}

#if 0
static uint32_t xorCallback(uint32_t interval, void *param)
{
	SDL_Event e;
	SDL_UserEvent u;
	l_loop_t *l = (l_loop_t *)param;
	bta_cell_t *c;
	uint32_t cell;

	cell = l->c_cell + 1;
	if (cell == bta_getNumCells(l->anim->bta, l->loopNumber))
		cell = 0;

	u.type = BT_ANIM_EVENT;
	u.data1 = xorAnim;
	u.data2 = param;

	e.type = BT_ANIM_EVENT;
	e.user = u;

	SDL_PushEvent(&e);

#if 0
	c = bta_cell_get(l->anim->bta, l->loopNumber, l->c_cell);
#endif
	c = bta_cell_get(l->anim->bta, l->loopNumber, cell);

	return c->delay;
}
#endif

static void simpleAnim(void *data)
{
	bta_cell_t	*c;
	l_loop_t 	*l = (l_loop_t *)data;
	l_anim		*a = (l_anim *)l->anim;
	SDL_Rect	r;
	SDL_Surface	*s;

	/* 
	 * The current cell number is updated by the timer.
	 * Draw the current cell into the window
	 */
	c = bta_cell_get(l->anim->bta, l->loopNumber, l->c_cell);

	r.x = c->x;
	r.y = c->y;
	r.w = c->width;
	r.h = c->height;

	s = SDL_CreateRGBSurfaceFrom(c->gfx->buf, c->width, c->height, 
		32, 4*c->width, 
		0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	SDL_BlitSurface(s, NULL, a->s, &r);
	SDL_FreeSurface(s);
#if 0
	/*
	 * This assumes that the entire graphic is replaced
	 * In order to make it more generic it should only update
	 * the area defined by c->x, c->y, c->w, c->h. But I'm lazy.
	 */
	SDL_LockSurface(a->s);
	memcpy(a->s->pixels, c->gfx->buf, c->gfx->size);
	SDL_UnlockSurface(a->s);

#endif
	SDL_BlitSurface(a->s, NULL, a->dest, a->dest_rect);
	SDL_UpdateRect(a->dest, a->dest_rect->x, a->dest_rect->y, \
				a->dest_rect->w, a->dest_rect->h);

	l->c_cell++;
	if (l->c_cell == bta_getNumCells(l->anim->bta, l->loopNumber))
		l->c_cell = 0;
}

#if 0
static void xorAnim(void *data)
{
	l_loop_t *l = (l_loop_t *)data;
	bta_cell_t *c;
	l_anim *a = l->anim;
	uint32_t i, j;
	uint8_t *src, *dest;

	/* 
	 * The current cell number is updated by the timer.
	 * Draw the current cell into the window
	 */
	c = bta_cell_get(l->anim->bta, l->loopNumber, l->c_cell);

#if 0
	SDL_LockSurface(a->s);

	dest = (uint8_t *)a->s->pixels + ((c->y * a->s->pitch) + c->x * 4);
	src = c->gfx->buf;

	for (i = 0; i < c->height; i++) {
		for (j = 0; j < c->width * 4; j++)
			*dest++ ^= *src++;

		dest += (a->s->pitch - c->width) * 4;
	}
	SDL_UnlockSurface(a->s);
#endif

	SDL_Surface *new;
	SDL_Rect r;

	r.x = c->x;
	r.y = c->y;
	r.w = c->width;
	r.h = c->height;
	new = SDL_CreateRGBSurfaceFrom(c->gfx->buf, c->width, c->height, 32, 4*c->width, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	SDL_BlitSurface(new, NULL, a->s, &r);
	SDL_FreeSurface(new);

	SDL_BlitSurface(a->s, NULL, a->dest, a->dest_rect);
	SDL_UpdateRect(a->dest, a->dest_rect->x, a->dest_rect->y, \
				a->dest_rect->w, a->dest_rect->h);

	/* Update the current cell after updating the surface */
	l->c_cell++;
	if (l->c_cell == bta_getNumCells(l->anim->bta, l->loopNumber))
		l->c_cell = 0;
}
#endif

static int l_anim_free(lua_State *L)
{
	l_anim *a = TO_SDL_ANIM(L, 1);

	if (a != NULL) {
		SDL_FreeSurface(a->s);
		bta_free(a->bta);
		free(a->dest_rect);
	}

	return 0;
}

static int l_anim_tostring(lua_State *L)
{
	l_anim *a = TO_SDL_ANIM(L, 1);

	lua_pushfstring(L, "Type = %p", a);

	return 1;
}

static int l_anim_start(lua_State *L)
{
	SDL_Surface *win;
	l_anim *a;
	SDL_Rect *r;
	int index;
	bta_cell_t *btac;

	a = l_checkAnim(L, 1);
	win = l_checkSurface(L, 2);
	index = 3;
	r = sdl_rect_arg(L, &index);

	a->dest = win;
	a->dest_rect = r;

#if 0
	/* Initialize the surface with the base image */
	if (a->bta->type == BTA_TYPE_SIMPLELOOP) {
		btac = bta_cell_get(a->bta, 0, 0);
	} else {
		btac = a->bta->base;
	}
#endif

	btac = bta_get_base(a->bta);

	SDL_LockSurface(a->s);
	memcpy(a->s->pixels, btac->gfx->buf, btac->gfx->size);
	SDL_UnlockSurface(a->s);

	SDL_BlitSurface(a->s, NULL, a->dest, a->dest_rect);
	SDL_UpdateRect(a->dest, a->dest_rect->x, a->dest_rect->y, \
				a->dest_rect->w, a->dest_rect->h);

	/* Create a timer for each loop */
	for (index = 0; index < bta_getNumLoops(a->bta); index++) {
		l_loop_t *loop;
		bta_cell_t *c;

		loop = (l_loop_t *)gl_list_get_at(a->loopData, index);
		c = bta_cell_get(a->bta, index, 0);

		loop->timer = SDL_AddTimer(c->delay, simpleCallback, loop);
#if 0
		if (a->bta->type == BTA_TYPE_SIMPLELOOP)
			loop->timer = SDL_AddTimer(c->delay, simpleCallback, loop);
		else
			loop->timer = SDL_AddTimer(c->delay, xorCallback, loop);
	
#endif
	}

	return 0;
}

static int l_anim_stop(lua_State *L)
{
	l_anim *a;
	int i, j;
	uint32_t nevents;
	SDL_Event event[20];
	uint32_t push;

	a = l_checkAnim(L, 1);

	for (i = 0; i < bta_getNumLoops(a->bta); i++) {
		l_loop_t *l = (l_loop_t *)gl_list_get_at(a->loopData, i);

		if (SDL_RemoveTimer(l->timer) == SDL_FALSE) {
			luaL_error(L, "Error removing animation timer");
		}
	}

	/*
	 * There could be outstanding animation events in the event
	 * queue. Loop through the outstanding events and compare with
	 * each loop pointer of the current animation. If they don't match
	 * push the event back into the event queue. 
	 */
	nevents = SDL_PeepEvents(event, 20, SDL_GETEVENT,
				SDL_EVENTMASK(BT_ANIM_EVENT));
	for (i = 0; i < nevents; i++) {
		push = 1;
		for (j = 0; j < bta_getNumLoops(a->bta); j++) {
			if (event[i].user.data2 == gl_list_get_at(a->loopData,j)) {
				push = 0;
				break;
			}
		}

		if (push) {
			SDL_PushEvent(&event[i]);
		}
	}
	nevents = SDL_PeepEvents(event, 20, SDL_GETEVENT,
				SDL_EVENTMASK(BT_ANIM_EVENT));
		

	return 0;
}
/********************************/
/*				*/
/* Lua interface		*/
/*				*/
/********************************/

static int l_anim_new(lua_State *L)
{
	l_anim *s;

	s = lua_newuserdata(L, sizeof(l_anim));
	luaL_getmetatable(L, "l_sdl_anim");
	lua_setmetatable(L, -2);

	return 1;
}

static l_anim *l_testAnim(lua_State *L, int index)
{
	return (l_anim *)luaL_testudata(L, index, "l_sdl_anim");
}

static l_anim *l_checkAnim(lua_State *L, int index)
{
	return (l_anim *)luaL_checkudata(L, index, "l_sdl_anim");
}

void l_sdl_anim_open(lua_State *L)
{
	class_begin(L, "l_sdl_anim");
	mod_variable(L, "isAnim", l_anim_isAnim, l_var_readonly);
	mod_variable(L, "w", l_anim_get_w, l_var_readonly);
	mod_variable(L, "h", l_anim_get_h, l_var_readonly);

	mod_function(L, "__gc", l_anim_free);
	mod_function(L, "__tostring", l_anim_tostring);
	mod_function(L, "start", l_anim_start);
	mod_function(L, "stop", l_anim_stop);
	class_end(L);
}

int l_img_load_bta(lua_State *L)
{
	btstring_t	*fname;
	l_anim		*a;
	bta_cell_t	*c;
	uint32_t	i;

	fname = _l_getFilePath(L, 1);
	l_anim_new(L);
	a = l_checkAnim(L, -1);

	a->bta = bta_read_bta(fname);
	c = bta_get_base(a->bta);
#if 0
	if (a->bta->type == BTA_TYPE_SIMPLELOOP) {
		c = bta_cell_get(a->bta, 0, 0);
	} else {
		c = a->bta->base;
	}
#endif

	a->loopData = gl_list_nx_create_empty(GL_ARRAY_LIST,
				NULL, NULL, l_loop_free, 1);
	if (a->loopData == NULL) {
		luaL_error(L, "Error creating loop data");
	}

	for (i = 0; i < bta_getNumLoops(a->bta); i++) {
		l_loop_t 	*l;

		l = xzalloc(sizeof(l_loop_t));
		l->anim = a;
		l->c_cell = 0;
		l->loopNumber = i;

		if (gl_list_nx_add_last(a->loopData, l) == NULL) {
			luaL_error(L, "Out of memory");
		}
	}

	a->s = SDL_CreateRGBSurface(SDL_SWSURFACE, c->width, c->height,
			32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	return 1;
}
