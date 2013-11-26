#include <bte.h>


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

	mod_constant_s(L, "dataPath",	DATADIR);

	lua_setglobal(L, "sys");
}
