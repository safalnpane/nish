/**
 * nish.h - Global structs and functions
 */

#ifndef NISH_H
#define NISH_H

#define MAX_CMD_ARGS 50


struct cmd_t {
	char *path;
	char *args[MAX_CMD_ARGS];
	char **environ;
	int (*handler)(struct cmd_t *c);
};


#endif // NISH_H
