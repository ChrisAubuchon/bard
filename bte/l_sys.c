#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <l_int.h>


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
/* Public functions		*/
/*				*/
/********************************/


void l_sys_open(lua_State *L)
{
	lua_newtable(L);

	L_REGSTRING(L, "dataPath",	DATADIR);

	lua_setglobal(L, "sys");
}
