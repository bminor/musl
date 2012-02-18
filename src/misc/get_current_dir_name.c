#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

char *get_current_dir_name(void) {
	char buf[PATH_MAX];
	char* res = getenv("PWD");
	if(res && *res) return strdup(res);
	if(!getcwd(buf, sizeof(buf))) return NULL;
	return strdup(buf);
}
