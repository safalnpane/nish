/**
 * command.h - Handles command path resolution and execution
 */

#ifndef COMMAND_H
#define COMMAND_H

#define MAX_CMD_ARGS 50

struct cmd {
	char *cmd_path;
	char *args[MAX_CMD_ARGS];
	char **environ;
};

char *resolve_cmd_path(const char *c);
int execute_cmd(struct cmd *c);


#endif
