/**
 * input.h - Handles terminal raw input
 */
#ifndef INPUT_H
#define INPUT_H

#include <termios.h>

#define MAX_LINE_SIZE 1024
#define MAX_INPUT_ARGS 20

// Special Keys
#define CTRL_L 0x0C
#define DELETE 0x7F
#define BSPACE 0x08

// Escape Code
#define NEW_LINE '\n'
#define TAB '\t'
#define BACKSPACE '\b'
#define ESC 0x1B
#define CRSR_HOME "\033[H"
#define CRSR_ERASE_ALL "\033[J"
#define CRSR_MV_UP "\033[#A"
#define CRSR_MV_DOWN "\033[#B"
#define CRSR_MV_RIGHT "\033[#C"
#define CRSR_MV_LEFT "\033[#D"

static struct termios original_termios;

char *read_line();
void parse_input(const char *input, char **args);

#endif
