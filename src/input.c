/**
* input.c - Handling terminal raw input
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "input.h"

struct termios original_termios;

static void
disable_raw_mode() {
	tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}


static void
enable_raw_mode()
{
	tcgetattr(STDIN_FILENO, &original_termios);
	atexit(disable_raw_mode);

	struct termios raw = original_termios;
	raw.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}


char *
read_line()
{
	char *input_buff = calloc(MAX_LINE_SIZE, sizeof(char));
	char c;
	int index = 0;
	enable_raw_mode();
	while (1) {
		read(STDIN_FILENO, &c, 1);
		if (c == CTRL_L) {
			printf("%s%s", CRSR_HOME, CRSR_ERASE_ALL);
			fflush(stdout);
			break;
		} else if ((c == ' ' || c == TAB) && (strlen(input_buff) == 0)) {
			continue;
		} else if (c == EOF) {
			break;
		} else if (c == NEW_LINE) {
			input_buff[index] = '\0';
			putchar('\n');
			fflush(stdout);
			break;
		} else if (c == DELETE || c == BSPACE) {
			if (index > 0) {
				index--;
				printf("%c %c", BACKSPACE, BACKSPACE);
				fflush(stdout);
			}
		} else {
			if (index < MAX_LINE_SIZE - 1) {
				putchar(c);
				fflush(stdout);
				input_buff[index] = c;
				index += 1;
			} else {
				input_buff[index] = '\0';
				break;
			}
		}
	}
	disable_raw_mode();
	return input_buff;
}


void
parse_input(const char *input, char **args)
{
	char *token;
	char *input_copy = strdup(input);
	int i = 0;

	token = strtok(input_copy, " ");
	while (token) {
		if (i == 0) {
			// First token is always a `command`
			args[i] = strdup(token);
			token = strtok(NULL, " ");
			i += 1;
			continue;
		}

		// Build rest of the arguments
		if (token[0] == '"') {
			char new_arg[MAX_ARG_LEN] = {0};
			int end_quote_found = 0;
			strcat(new_arg, token + 1);
			while (!end_quote_found) {
				token = strtok(NULL, " ");
				size_t token_len = strlen(token);
				if (token[token_len - 1] == '"') {
					token[token_len - 1] = '\0';
					strcat(new_arg, " ");
					strcat(new_arg, token);
					end_quote_found = 1;
					break;
				}
			}
			
			if (!end_quote_found) {
				// Likely a syntax error
				free(input_copy);
				fprintf(stderr, "Syntax error. Did you forgot '\"'?\n");
				exit(-1);
				break;
			}

			args[i] = strdup(new_arg);
			token = strtok(NULL, " ");
			i += 1;
			continue;
		}

		args[i] = strdup(token);
		token = strtok(NULL, " ");
		i += 1;
	}

	args[i] = NULL;
	free(input_copy);
} 
