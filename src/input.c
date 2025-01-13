/**
* input.c - Handling terminal raw input
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "input.h"


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

	token = strtok(input_copy, " \t");
	while (token) {
		args[i] = strdup(token);
		token = strtok(NULL, " \t");
		i += 1;
	}
	args[i] = NULL;
	free(input_copy);
} 
