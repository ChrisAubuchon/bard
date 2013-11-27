#include <bte.h>
#include <l_sdl.h>
#include <bt_bta.h>
#include <gl_list.h>
#include <gl_array_list.h>

/*#define DEBUG 1*/
#include <debug.h>


/********************************/
/*				*/
/* Static variables		*/
/*				*/
/********************************/

/********************************/
/*				*/
/* Animation struture		*/
/*				*/
/********************************/

typedef struct {
	SDL_Renderer	*renderer;
	SDL_Texture	*texture;
	SDL_Surface	*screen;

	SDL_Rect	*rect;
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
/* Private function prototypes	*/
/*				*/
/********************************/

static uint32_t simpleCallback(uint32_t interval, void *param);
static void simpleAnim(void *data);
static void l_loop_free(const void *data);

static int l_anim_isAnim(lua_State *L);
static int l_anim_get_w(lua_State *L);
static int l_anim_get_h(lua_State *L);
static int l_anim_free(lua_State *L);
static int l_anim_tostring(lua_State *L);
static int l_anim_start(lua_State *L);
static int l_anim_stop(lua_State *L);

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
	l_anim		*anim;

	anim = l_checkAnim(L, 1);

	lua_pushnumber(L, anim->s->w);

	return 1;
}

static int l_anim_get_h(lua_State *L)
{
	l_anim		*anim;

	anim = l_checkAnim(L, 1);

	lua_pushnumber(L, anim->s->h);

	return 1;
}

static uint32_t simpleCallback(uint32_t interval, void *param)
{
	SDL_Event	e;
	SDL_UserEvent	u;
	l_loop_t	*l = (l_loop_t *)param;
	bta_cell_t	*c;
	uint32_t	cell;

	cell = l->c_cell + 1;
	if (cell == bta_getNumCells(l->anim->bta, l->loopNumber))
		cell = 0;

	u.type = BT_ANIM_EVENT;
	u.data1 = simpleAnim;
	u.data2 = param;

	e.type = BT_ANIM_EVENT;
	e.user = u;

	SDL_PushEvent(&e);

	c = bta_cell_get(l->anim->bta, l->loopNumber, cell);

	return c->delay;
}

static void simpleAnim(void *data)
{
	l_loop_t	*loop;
	l_anim		*anim;
	bta_cell_t	*cell;
	SDL_Rect	r;
	SDL_Surface	*s;

	loop = (l_loop_t *)data;
	anim = loop->anim;

	/* 
	 * The current cell number is updated by the timer.
	 * Draw the current cell into the window
	 */
	cell = bta_cell_get(loop->anim->bta, loop->loopNumber, loop->c_cell);

	r.x = cell->x;
	r.y = cell->y;
	r.w = cell->width;
	r.h = cell->height;

	s = SDL_CreateRGBSurfaceFrom(cell->gfx->buf, cell->width, cell->height, 
		32, 4*cell->width, 
		0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	SDL_BlitSurface(s, NULL, anim->s, &r);
	SDL_FreeSurface(s);
	SDL_BlitSurface(anim->s, NULL, anim->screen, anim->rect);
	SDL_UpdateTexture(anim->texture, NULL,
			anim->screen->pixels, anim->screen->pitch);
	SDL_RenderCopy(anim->renderer, anim->texture, NULL, NULL);	
	SDL_RenderPresent(anim->renderer);

	loop->c_cell++;
	if (loop->c_cell == bta_getNumCells(loop->anim->bta, loop->loopNumber))
		loop->c_cell = 0;
}

static int l_anim_free(lua_State *L)
{
	l_anim		*a;

	a = l_checkAnim(L, 1);

	if (a != NULL) {
		/* 
		 * Call l_anim_stop() to eliminate a race condition
		 * between the free'ing of the bta and the events.
		 */
		l_anim_stop(L);
		SDL_FreeSurface(a->s);
		bta_free(a->bta);
	}

	return 0;
}

static int l_anim_tostring(lua_State *L)
{
	l_anim		*a;

	a = l_checkAnim(L, 1);

	lua_pushfstring(L, "Type = %p", a);

	return 1;
}

static int l_anim_start(lua_State *L)
{
	l_anim		*anim;

	bta_cell_t	*base;
	uint32_t	index;


	anim = l_checkAnim(L, 1);
	anim->renderer = l_checkRenderer(L, 2);
	anim->texture = l_checkTexture(L, 3);
	anim->screen = l_checkSurface(L, 4);
	anim->rect = l_checkRect(L, 5);
	
	base = bta_get_base(anim->bta);

	SDL_LockSurface(anim->s);
	memcpy(anim->s->pixels, base->gfx->buf, base->gfx->size);
	SDL_UnlockSurface(anim->s);

	SDL_BlitSurface(anim->s, NULL, anim->screen, anim->rect);
	SDL_UpdateTexture(anim->texture, NULL, 
			anim->screen->pixels, anim->screen->pitch);
	SDL_RenderCopy(anim->renderer, anim->texture, NULL, NULL);	
	SDL_RenderPresent(anim->renderer);

	/* Create a timer for each loop */
	for (index = 0; index < bta_getNumLoops(anim->bta); index++) {
		l_loop_t *loop;
		bta_cell_t *c;

		loop = (l_loop_t *)gl_list_get_at(anim->loopData, index);
		c = bta_cell_get(anim->bta, index, 0);

		loop->timer = SDL_AddTimer(c->delay, simpleCallback, loop);
	}

	return 0;
}

static int l_anim_stop(lua_State *L)
{
	l_anim		*a;
	int		i, j;
	uint32_t	nevents;
	uint32_t	push;
	SDL_Event	event[20];

	a = l_checkAnim(L, 1);

	for (i = 0; i < bta_getNumLoops(a->bta); i++) {
		l_loop_t *l = (l_loop_t *)gl_list_get_at(a->loopData, i);

		if (SDL_RemoveTimer(l->timer) == SDL_FALSE) {
			luaL_error(L, "Error removing animation timer");
		}
		l->timer = 0;
	}

	/*
	 * There could be outstanding animation events in the event
	 * queue. Loop through the outstanding events and compare with
	 * each loop pointer of the current animation. If they don't match
	 * push the event back into the event queue. 
	 */
	nevents = SDL_PeepEvents(event, 20, SDL_GETEVENT,
				BT_ANIM_EVENT,
				BT_ANIM_EVENT
				);
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
				BT_ANIM_EVENT,
				BT_ANIM_EVENT
				);
		

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
		l->timer = 0;

		if (gl_list_nx_add_last(a->loopData, l) == NULL) {
			luaL_error(L, "Out of memory");
		}
	}

	a->s = SDL_CreateRGBSurface(SDL_SWSURFACE, c->width, c->height,
			32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	debug("a->s->format = %s\n", SDL_GetPixelFormatName(a->s->format->format));

	return 1;
}
