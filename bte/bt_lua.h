#ifndef _BTE_BT_LUA_H
#define _BTE_BT_LUA_H

#include <lua.h>
#include <stdint.h>

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

lua_State *bt_lua_state(void);
void bt_lua_doFile(lua_State *L, uint8_t *script);

#endif
