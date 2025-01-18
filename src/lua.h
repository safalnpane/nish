/**
 * lua.h - Handles lua->Nish connection
 */
#ifndef LUA_H
#define LUA_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>


void initialize_lua();
void close_lua();
int execute_lua_inline(const char *script);

int lua_getenv(lua_State *L);
int lua_setenv(lua_State *L);

void setup_nish_obj(lua_State *L);

#endif
