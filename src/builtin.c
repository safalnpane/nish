/**
 * builtin.c - Handle shell builtin command
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "nish.h"


int
change_dir(struct cmd_t *c)
{
	char *path = c->args[1];

	if ((path == NULL) || (strcmp(path, "~") == 0)) {
		// Take me home
		path = getenv("HOME");
	} else if (strcmp(path, "-") == 0) {
		// Take me to previous PWD
		path = getenv("OLDPWD");
		if (!path) {
			fprintf(stderr, "cd: OLDPWD not set\n");
			return 1;
		}
	} else if ((path[0] == '~') && (path[1] == '/')) {
		char expanded_path[1024];
		snprintf(expanded_path, sizeof(expanded_path), "%s%s", getenv("HOME"), path + 1);
		path = expanded_path;
	}

	if (chdir(path) == 0) {
		setenv("OLDPWD", getenv("PWD"), 1);
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			setenv("PWD", cwd, 1);
		} else {
			perror("getcwd failed");
			return 1;
		}
	} else {
		perror("cd");
		return 1;
	}
}
