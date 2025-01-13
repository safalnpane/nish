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


#define PRINT_ARRAY(array) 								\
		int arrayIndex = 0; 							\
		while(array[arrayIndex] != NULL) { 							\
			printf("DEBUG: %s\n", array[arrayIndex]); 	\
			arrayIndex += 1;							\
		}


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
		char lineBuff[MAX_LINE_SIZE] = {0};
		int index, status = 0;

		read_loop(lineBuff);
		if (strlen(lineBuff) == 0) {
			printf("\n");
			continue;
		}

		struct cmd c = {0};
		parse_input(lineBuff, c.args);
		if (strcmp(c.args[0], "exit") == 0)
			exit(0);

		c.cmd_path = resolve_cmd_path(c.args[0]);
		if (c.cmd_path == NULL) {
			free(c.cmd_path);
			fprintf(stderr, "command '%s' not found\n\n", c.args[0]);
			continue;
		}

		c.environ = environ;
		status = execute_cmd(&c);
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
