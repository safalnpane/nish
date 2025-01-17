#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdlib.h>
#include <stdio.h>

#include "lua.h"

lua_State *L = NULL;


void
initialize_lua()
{
	L = luaL_newstate();
	luaL_openlibs(L);  // Load lua standard libraries
}


int execute_lua_inline(const char *script) {
    if (luaL_dostring(L, script) != LUA_OK) {
        fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        return 1;
    }
    return 0;
}


void
close_lua()
{
	lua_close(L);
}
