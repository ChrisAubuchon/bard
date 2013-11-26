#include <bte.h>
#include <math.h>
#include <time.h>

#include "random.c"


/********************************/
/*				*/
/* Private function prototype	*/
/*				*/
/********************************/

/* Module functions */
static int l_random_new(lua_State *L);
static MT *l_checkRandom(lua_State *L, int index);

/* Class functions */
static int l_random_seed(lua_State *L);

/********************************/
/*				*/
/* Private function		*/
/*				*/
/********************************/

static int l_random_new(lua_State *L)
{
	lua_Number seed;
	MT *m;

	seed = luaL_optnumber(L, 1, time(NULL));

	m = lua_newuserdata(L, sizeof(MT));
	L_SETMETATABLE(L, "l_random");

	init_genrand(m, seed);

	return 1;
}

static MT *l_checkRandom(lua_State *L, int index)
{
	return luaL_checkudata(L, index, "l_random");
}

static int l_random_seed(lua_State *L)
{
	MT *m = l_checkRandom(L, 1);

	init_genrand(m, luaL_optnumber(L, 2, time(NULL)));

	lua_settop(L, 1);

	return 1;
}

static int l_random_rnd(lua_State *L)
{
	uint32_t a, b, r;
	MT *m = l_checkRandom(L, 1);

	r = genrand_int32(m);

	switch (lua_gettop(L)) {
	case 1:
		lua_pushnumber(L, r);
		return 1;
	case 2:
		a = 1;
		b = luaL_checkint(L, 2);
		break;
	case 3:
		a = luaL_checkint(L, 2);
		b = luaL_checkint(L, 3);
		break;
	}

	if (a > b) { 
		int t = a;
		a = b;
		b = t;
	}

#if 0
	a = ceil(a);
	b = floor(b);
#endif

	if (a > b)
		return 0;

	r = a + (r % (b - a + 1));

	lua_pushnumber(L, r);

	return 1;
}

static int l_random_tostring(lua_State *L)
{
	MT *m = l_checkRandom(L, 1);

	lua_pushfstring(L, "l_random %p", (void *)m);

	return 1;
}

void l_random_open(lua_State *L)
{
	class_begin(L, "l_random");
	mod_function(L,	"__tostring",	l_random_tostring);
	mod_function(L, "__call",	l_random_rnd);
	mod_function(L, "seed",		l_random_seed);
	mod_function(L, "value",	l_random_rnd);
	class_end(L);

	lua_newtable(L);
	mod_function(L, "new",		l_random_new);
	lua_setglobal(L, "mt_random");
}
