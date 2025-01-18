#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdlib.h>
#include <stdio.h>

#include "lua.h"

lua_State *L = NULL;

// Register C functions
luaL_Reg nish_functions[] = {
	{"getenv", lua_getenv },
	{"setenv", lua_setenv },
	{NULL, NULL },
};



void
initialize_lua()
{
	L = luaL_newstate();
	luaL_openlibs(L);  // Load lua standard libraries
	
	setup_nish_obj(L);
}


int
execute_lua_inline(const char *script)
{
    if (luaL_dostring(L, script) != LUA_OK) {
        fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        return 1;
    }
    return 0;
}


int
lua_getenv(lua_State *L)
{
	const char *var = luaL_checkstring(L, 1);
	const char *value = getenv(var);
	
	if (value)
		lua_pushstring(L, value);
	else
		lua_pushnil(L);

	return 1;
}


int
lua_setenv(lua_State *L)
{
	const char *var = luaL_checkstring(L, 1);
	const char *val = luaL_checkstring(L, 2);
	int overwrite = luaL_optinteger(L, 3, 1);
	
	if (setenv(var, val, overwrite) == 0)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);

	return 1;
}


void
setup_nish_obj(lua_State *L)
{
	lua_newtable(L);
	luaL_setfuncs(L, nish_functions, 0);
	lua_setglobal(L, "nish");
}


void
close_lua()
{
	lua_close(L);
}
