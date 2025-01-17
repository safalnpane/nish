/**
 * lua.h - Handles lua->Nish connection
 */
#ifndef LUA_H
#define LUA_H

#include <lualib.h>


void initialize_lua();
void close_lua();
int execute_lua_inline(const char *script);


#endif
