#include <bte.h>

char *program_name = "BTE";

int main(int argc, char **argv)
{
	lua_State *L = bt_lua_state();

	if (argc != 2) {
		fprintf(stderr, "Usage: bte lua_file\n\n");
		return 1;
	}

	bt_lua_doFile(L, argv[1]);

	lua_close(L);
}
