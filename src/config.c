/**
* config.c - Handles loading and parsing lua configuration.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lua.h"
#include "config.h"


void
load_config_file()
{
	char *home = getenv("HOME");
	char config_path[1024];
	snprintf(config_path, sizeof(config_path), "%s/%s", home, CONFIG_FILE_NAME);

	if (access(config_path, F_OK) == -1) {
		fprintf(stderr, "nish: '%s' config file not found\n", config_path);
		fflush(stderr);
	}

	execute_lua_file(config_path);
}
