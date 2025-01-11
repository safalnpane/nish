/*
 * Hello world!
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_LINE_SIZE 512
#define MAX_INPUT_ARGS 20

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
}


char *
resolveCommandPath(const char *command)
{
	if (strchr(command, '/')) {
		if (access(command, X_OK) == 0) {
			return strdup(command);
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
		snprintf(fullPath, (strlen(dir) * sizeof(char)) + (strlen(command) * sizeof(command)), "%s/%s", dir, command);
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
parse_input(const char *input, char **args)
{
	char *token;
	char *input_copy = strdup(input);
	int i = 0;

	token = strtok(input_copy, " \t");
	while (token) {
		args[i] = strdup(token);
		token = strtok(NULL, " \t");
		i += 1;
	}
	args[i] = NULL;
	free(input_copy);
}


void
mainLoop(void)
{
	displayPrompt();
	char lineBuff[MAX_LINE_SIZE];

	while(fgets(lineBuff, MAX_LINE_SIZE, stdin) != NULL) {
		lineBuff[strlen(lineBuff) - 1] = '\0';
		if (strlen(lineBuff) == 0) {
			printf("\n");
			displayPrompt();
			continue;
		}

		char *inputArgs[MAX_INPUT_ARGS];

		parse_input(lineBuff, inputArgs);
		// PRINT_ARRAY(inputArgs);

		if (strcmp(inputArgs[0], "exit") == 0)
			exit(0);

		char *resolvedCommandPath = resolveCommandPath(inputArgs[0]);
		if (resolvedCommandPath == NULL) {
			free(resolvedCommandPath);
			fprintf(stderr, "Command '%s' not found\n", inputArgs[0]);
			displayPrompt();
			continue;
		}

		pid_t pid = fork();
		if (pid < 0) {
			free(resolvedCommandPath);
			perror("Process Fork failed\n");
			exit(1);
		} else if (pid == 0) {
			char *envs[] = {
				"PATH=/bin:/usr/bin",
				NULL,
			};

			if (execve(resolvedCommandPath, inputArgs, envs) == -1) {
				free(resolvedCommandPath);
				perror("Exec Failed\n");
				exit(1);
			}
			free(resolvedCommandPath);
		} else {
			if (resolvedCommandPath)
				free(resolvedCommandPath);
			int status;
			wait(&status);
			if (WIFEXITED(status)) {
				// printf("Child exited with status: %d\n", WEXITSTATUS(status));
			} else if (WIFSIGNALED(status)) {
				printf("Child terminated by signal: %d\n", WTERMSIG(status));
			}
		}
		
		printf("\n");
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
