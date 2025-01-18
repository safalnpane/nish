/**
 * builtin.c - Handle shell builtin command
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "nish.h"
#include "lua.h"
#include "config.h"


int
change_dir(struct cmd_t *c)
{
	char *path = c->args[1];

	if ((path == NULL) || (strcmp(path, "~") == 0)) {
		// Take me home
		path = getenv("HOME");
	} else if (strcmp(path, "-") == 0) {
		// Take me to previous PWD
		path = getenv("OLDPWD");
		if (!path) {
			fprintf(stderr, "cd: OLDPWD not set\n");
			return 1;
		}
	} else if ((path[0] == '~') && (path[1] == '/')) {
		char expanded_path[1024];
		snprintf(expanded_path, sizeof(expanded_path), "%s%s", getenv("HOME"), path + 1);
		path = expanded_path;
	}

	if (chdir(path) == 0) {
		setenv("OLDPWD", getenv("PWD"), 1);
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			setenv("PWD", cwd, 1);
			return 0;
		} else {
			perror("getcwd failed");
			return 1;
		}
	} else {
		perror("cd");
		return 1;
	}
}


int
read_env_var(struct cmd_t *c)
{
	if (c->args[1] == NULL) {
		fprintf(stderr, "Usage: getenv VARIBALE\n");
		return 1;
	}
	const char *value = getenv(c->args[1]);
	if (value)
		fprintf(stdout, "%s=%s\n", c->args[1], value);
	else
		fprintf(stderr, "%s: Env not set. Use `setenv %s=<VALUE>`.\n", c->args[1], c->args[1]);

	return 0;
}


int
set_env_var(struct cmd_t *c)
{
	char *arg = c->args[1];
	if (arg == NULL) {
		fprintf(stderr, "Usage: setenv <var>=<val>\n");
		return 1;
	}

	char *equal_sign = strchr(arg, '=');
	if (!equal_sign) {
		fprintf(stderr, "Invalid format. Use: setenv <var>=<val>\n");
		return 1;
	}
	
	size_t var_len = equal_sign - arg;
	if (var_len == 0) {
		// no variable only `=<value>`
		fprintf(stderr, "Invalid format. Use: setenv <var>=<val>\n");
		return 1;
	}

	char var[256];
	strncpy(var, arg, var_len);
	var[var_len] = '\0';

	char *value = equal_sign + 1;

	if (setenv(var, value, 1) == 0) {
		printf("%s = %s\n", var, value); 
		return 0;
	} else {
		perror("setenv failed");
		return 1;
	}
}


int
unset_env_var(struct cmd_t *c)
{
	if (c->args[1] == NULL) {
		fprintf(stderr, "Usage: unsetenv VARIBALE\n");
		return 1;
	}
	if (unsetenv(c->args[1]) == 0)
		fprintf(stdout, "'%s' is unset.\n", c->args[1]);
	else
		fprintf(stderr, "'%s': Env not set. Use `setenv %s=<VALUE>`.\n", c->args[1], c->args[1]);

	return 0;
}


int
reload_nish_config(struct cmd_t *c)
{
	(void) c;
	load_config_file();
	fprintf(stdout, "nish: config file reloaded\n");
	fflush(stdout);
	return 0;
}


static int file_exists(const char *path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}


int
execute_nlua(struct cmd_t *c)
{
	if (c->args[1] == NULL) {
		fprintf(stderr, "Usage: nlua '<inline lua>' or\n");
		fprintf(stderr, "       nlua '<lua file path>'\n");
		return 1;
	}

	// combine all args into one string
	char args[1024] = {0};
	for (int i = 1; c->args[i] != NULL; i++) {
		strcat(args, c->args[i]);
		if (c->args[i + 1] != NULL) {
			strcat(args, " ");
		}
	}

	// check if the string ends with `.lua`
	size_t args_len = strlen(args);
	if (args_len > 4 && strcmp(args + args_len - 4, ".lua") == 0) {
		if (file_exists(args)) {
			return execute_lua_file(args);
		} else {
			fprintf(stderr, "nlua: '%s' file is not found.\n", args);
			return 1;
		}
	} else {
		return execute_lua_inline(args);
	}
}
