/**
 * command.h - Handles command path resolution and execution
 */

#ifndef COMMAND_H
#define COMMAND_H

#define MAX_CMD_ARGS 50
#define CMD_BUILTIN 0
#define CMD_BINARY 0

struct cmd {
	char *path;
	char *args[MAX_CMD_ARGS];
	char **environ;
	int type;
};

void resolve_cmd(struct cmd *c);
char *resolve_cmd_path(const char *c);
int execute_cmd(struct cmd *c);


#endif
