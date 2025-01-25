/* alias.h - Handles alias creation and resolution */
#ifndef ALIAS_H
#define ALIAS_H

#define MAX_ALIASES 50


struct alias_t {
	char *name;
	char *target;
};


int create_alias(const char *name, const char *target);
char *get_alias(const char *name);


#endif  // ALIAS_H
