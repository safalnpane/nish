/*
 * Hello world!
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_LINE_SIZE 128


void
displayPrompt(void)
{
	printf("~\n> ");
}


void mainLoop(void)
{
	displayPrompt();
	char lineBuff[MAX_LINE_SIZE];
	while(fgets(lineBuff, MAX_LINE_SIZE, stdin) != NULL) {
		lineBuff[strlen(lineBuff) - 1] = '\0';

		if (strcmp(lineBuff, "exit") == 0)
			exit(0);
		
		printf("%s\n", lineBuff);
		displayPrompt();
	}
	printf("\n");
}


int
main(void)
{
	mainLoop();
	return 0;
}
