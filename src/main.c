/*
 * main.c - Entrypoint for NiSH
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include "nish.h"
#include "input.h"
#include "command.h"


extern char **environ;


void
displayPrompt(void)
{
	static char path[1024];
	const char *home = getenv("HOME");
	char *pwd = getenv("PWD");

	if (home && strncmp(pwd, home, strlen(home)) == 0) {
		snprintf(path, sizeof(path), "~%s", pwd + strlen(home));
	}

	// char *last_slash = strrchr(pwd, '/');
	// if (last_slash) {
	// 	pwd = last_slash + 1;
	// }
	printf("%s\n> ", path);
    fflush(stdout);
}


void
mainLoop(void)
{
	while(1) {
		displayPrompt();
		int index, status = 0;

		char *lineBuff = read_line();  // Please free this.
		if (strlen(lineBuff) == 0) {
			printf("\n");
			continue;
		}

		struct cmd_t c = {0};
		c.environ = environ;
		parse_input(lineBuff, c.args);
		free(lineBuff);  // freed!
		if (strcmp(c.args[0], "exit") == 0)
			exit(0);

		resolve_cmd(&c);
		if (c.handler == NULL) {
			free(c.path);
			fprintf(stderr, "command '%s' not found\n\n", c.args[0]);
			continue;
		}

		status = c.handler(&c);
		free(c.path);
		if (status == -1)
			exit(1);

		putchar('\n');
	}
}


int
main(void)
{
	mainLoop();
	return 0;
}
