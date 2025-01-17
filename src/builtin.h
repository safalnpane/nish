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


struct builtin_cmd_t builtins[] = {
	{"cd", change_dir},
	{NULL, NULL},
};


#endif
