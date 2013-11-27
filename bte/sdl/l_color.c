#include <bte.h>
#include <l_sdl.h>

/********************************/
/*				*/
/* Internal function prototypes	*/
/*				*/
/********************************/

static void l_read_RGBAtable(lua_State *L, SDL_Color *rwColor);
static void l_read_RGBAarray(lua_State *L, SDL_Color *rwColor);

/********************************/
/*				*/
/* Internal functions		*/
/*				*/
/********************************/

/*
 * l_read_RGBAtable()
 *
 * This function assumes that the top of the stack is
 * the color table.
 */
static void l_read_RGBAtable(lua_State *L, SDL_Color *rwColor)
{
	lua_pushstring(L, "r");
	lua_rawget(L, -2);
	rwColor->r = (uint8_t)luaL_checkint(L, -1);
	lua_pop(L, 1);

	lua_pushstring(L, "g");
	lua_rawget(L, -2);
	rwColor->g = (uint8_t)luaL_checkint(L, -1);
	lua_pop(L, 1);

	lua_pushstring(L, "b");
	lua_rawget(L, -2);
	rwColor->b = (uint8_t)luaL_checkint(L, -1);
	lua_pop(L, 1);

	lua_pushstring(L, "a");
	lua_rawget(L, -2);
	rwColor->a = (uint8_t)luaL_optint(L, -1, 255);
	lua_pop(L, 1);
}

/*
 * l_read_RGBAarray()
 *
 * This function reads a table with "r" in slot 1, "g" in slot 2,
 * "b" in slot 3 and "a" in slot 4
 */
static void l_read_RGBAarray(lua_State *L, SDL_Color *rwColor)
{
	lua_pushnumber(L, 1);
	lua_rawget(L, -2);
	rwColor->r = (uint8_t)luaL_checkint(L, -1);
	lua_pop(L, 1);

	lua_pushnumber(L, 2);
	lua_rawget(L, -2);
	rwColor->g = (uint8_t)luaL_checkint(L, -1);
	lua_pop(L, 1);

	lua_pushnumber(L, 3);
	lua_rawget(L, -2);
	rwColor->b = (uint8_t)luaL_checkint(L, -1);
	lua_pop(L, 1);

	lua_pushnumber(L, 4);
	lua_rawget(L, -2);
	rwColor->a = (uint8_t)luaL_optint(L, -1, 255);
	lua_pop(L, 1);

}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

/*
 * l_checkColor()
 */
void l_checkColor(lua_State *L, int index, SDL_Color *rwColor)
{
	if (rwColor == NULL)
		luaL_error(L, "NULL pointer passed to l_checkColor");
	
	/*
	 * Check for an { R, G, B, A } table
	 */
	if (lua_type(L, index) == LUA_TTABLE) {
		lua_pushvalue(L, index);	/* Push a copy of the table */

		/* Check for an RGBA array */
		lua_pushnumber(L, 1);
		lua_rawget(L, -2);
		if (!lua_isnil(L, -1)) {
			lua_pop(L, 1);		/* Remove the result */
			l_read_RGBAarray(L, rwColor);
			lua_pop(L, 1);		/* Pop the table copy */
			return;
		}
		lua_pop(L, 1);

		/* Check for an RGBA dictionary */
		lua_pushstring(L, "r");
		lua_rawget(L, -2);
		if (!lua_isnil(L, -1)) {
			lua_pop(L, 1);		/* Remove the result */
			l_read_RGBAtable(L, rwColor);
			lua_pop(L, 1);		/* Pop the table copy */
			return;
		}
		lua_pop(L, 1);

		luaL_error(L, "Expected an RGBA table");
	} else if (lua_type(L, index) == LUA_TNUMBER) {
		/* 
		 * Read the RGBA values. Leave the Red value on
		 * the stack and remove the others. This hides
		 * the fact that the color is actually 3 or 4 stack
		 * slots and lets the caller assume that a color
		 * is only one slot.
		 *
		 * color = l_checkColor(L, 1);
		 * otherValue = l_checkValue(L, 2);
		 **/
		rwColor->r = (uint8_t)luaL_checkint(L, index);
		index++;
		rwColor->g = (uint8_t)luaL_checkint(L, index);
		lua_remove(L, index);
		rwColor->b = (uint8_t)luaL_checkint(L, index);
		lua_remove(L, index);
		if (lua_isnumber(L, index)) {
			rwColor->a = (uint8_t)luaL_checkint(L, index);
			lua_remove(L, index);
		} else {
			rwColor->a = 255;
		}
	} else {
		luaL_error(L, "Expecting a color");
	}
}

