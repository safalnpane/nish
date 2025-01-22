/**
 * builtin.h - Handle shell builtin command
 */

#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdlib.h>

#include "nish.h"


struct builtin_cmd_t {
	char *name;
	int (*handler)(struct cmd_t *c);
};


int change_dir(struct cmd_t *c);

// Environment Vars
int read_env_var(struct cmd_t *c);
int set_env_var(struct cmd_t *c);
int unset_env_var(struct cmd_t *c);

// Config
int reload_nish_config(struct cmd_t *c);

// Lua
int execute_nlua(struct cmd_t *c);

// Alias
int create_alias(struct cmd_t *c);


struct builtin_cmd_t builtins[] = {
	{"cd", change_dir},
	{"getenv", read_env_var},
	{"setenv", set_env_var},
	{"unsetenv", unset_env_var},
	{"reload", reload_nish_config},
	{"nlua", execute_nlua},
	{"alias", create_alias},
	{NULL, NULL},
};


#endif
