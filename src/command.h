/**
 * command.h - Handles command path resolution and execution
 */

#ifndef COMMAND_H
#define COMMAND_H

#include "nish.h"


void resolve_cmd(struct cmd_t *c);
static char *resolve_cmd_path(const char *c);
int execute_cmd(struct cmd_t *c);


#endif
