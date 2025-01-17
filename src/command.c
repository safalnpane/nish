/**
* command.c - Handles command path resolution and execution
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

#include "nish.h"
#include "builtin.h"
#include "command.h"


static char *
resolve_cmd_path(const char *c)
{
	if (strchr(c, '/')) {
		if (access(c, X_OK) == 0) {
			return strdup(c);
		}
		return NULL;
	}

	char *path = getenv("PATH");
	if (!path) {
		perror("ERROR: PATH environment variable is not set\n");
		return NULL;
	}

	char *path_cpy = strdup(path);
	char *dir = strtok(path_cpy, ":");
	char fullPath[1024];
	while (dir) {
		snprintf(fullPath, (strlen(dir) * sizeof(char)) + (strlen(c) * sizeof(c)), "%s/%s", dir, c);
		if (access(fullPath, X_OK) == 0) {
			free(path_cpy);
			return strdup(fullPath);
		}
		dir = strtok(NULL, ":");
	}

	free(path_cpy);
	return NULL;
}


void
resolve_cmd(struct cmd_t *c)
{
	for (int i = 0; builtins[i].name != NULL; i++) {
		if (strcmp(builtins[i].name, c->args[0]) == 0) {
			c->handler = builtins[i].handler;
			return;
		}
	}

	char *valid_path = resolve_cmd_path(c->args[0]);
	if (valid_path) {
		c->path = strdup(valid_path);
		c->handler = execute_cmd;
	} else {
		c->handler = NULL;
	}
}


int
execute_cmd(struct cmd_t *c)
{
	pid_t pid = fork();
	if (pid < 0) {
		perror("Process Fork failed\n");
		return 1;
	} else if (pid == 0) {
		if (execve(c->path, c->args, c->environ) == -1) {
			perror("Exec Failed\n");
			return -1;
		}
		return 0;
	} else {
		int status;
		wait(&status);
		if (WIFEXITED(status)) {
			return 0;
		} else if (WIFSIGNALED(status)) {
			printf("Child terminated by signal: %d\n", WTERMSIG(status));
			return 1;
		}
		return 0;
	}
}
