/**
 * command.h - Handles command path resolution and execution
 */

#ifndef COMMAND_H
#define COMMAND_H


void resolve_cmd(struct cmd_t *c);
int execute_cmd(struct cmd_t *c);


#endif
