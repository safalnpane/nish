/**
 * input.h - Handles terminal raw input
 */
#ifndef INPUT_H
#define

#include <termios.h>

#define MAX_LINE_SIZE 512
#define MAX_INPUT_ARGS 20

struct termios original_termios;

void disable_raw_mode();
void enable_raw_mode();
void parse_input(const char *input, char **args);

#endif
