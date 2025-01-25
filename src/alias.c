/* alias.c - Handles alias creation and resolution */

#include <string.h>

#include "alias.h"


struct alias_t aliases[MAX_ALIASES] = {0};
int alias_index = 0;


int
create_alias(const char *name, const char *target)
{
	if (alias_index < MAX_ALIASES) {
		aliases[alias_index] = (struct alias_t){strdup(name), strdup(target)};
		return 0;
	}

	return -1;
}


char *
get_alias(const char *name)
{
	for (int i = 0; i < alias_index; i++) {
		if (aliases[i].name)
			break;
		if (strcmp(aliases[i].name, name) == 0) {
			return strdup(aliases[i].target);
		}
	}

	return NULL;
}
