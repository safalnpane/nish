/*
 * main.c - Entrypoint for NiSH
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include "input.h"
#include "command.h"


extern char **environ;


void
displayPrompt(void)
{
	printf("~\n> ");
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
