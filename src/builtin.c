/**
 * builtin.c - Handle shell builtin command
 */

#include <stdio.h>

#include "nish.h"


int
change_dir(struct cmd_t *c)
{
	printf("Hello, World!\n");
	return 0;
}
