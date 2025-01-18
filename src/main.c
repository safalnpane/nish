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
#include "lua.h"
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
		snprintf(path, sizeof(path), "\e[1;36m~%s\e[m", pwd + strlen(home));
	}

	printf("%s\n\e[1;32m❯\e[m ", path);
    fflush(stdout);
}


void
shell_init(void)
{
	const char *term = getenv("TERM");
	if (!term) {
		setenv("TERM", "xterm-256color", 1);
	}

	initialize_lua();
}


void
mainLoop(void)
{
	shell_init();

	while(1) {
		displayPrompt();
		int status = 0;

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
	close_lua();
	return 0;
}
