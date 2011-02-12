#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <stddef.h>
#include <libc.h>

int scandir(const char *path, struct dirent ***res,
	int (*sel)(const struct dirent *),
	int (*cmp)(const struct dirent **, const struct dirent **))
{
	DIR *d = opendir(path);
	struct dirent *de, **names=0, **tmp;
	size_t cnt=0, len=0, size;
	int old_errno = errno;

	if (!d) return -1;

	while ((errno=0), (de = readdir(d))) {
		if (sel && !sel(de)) continue;
		if (cnt >= len) {
			len = 2*len+1;
			if (len > SIZE_MAX/sizeof *names) break;
			tmp = realloc(names, len * sizeof *names);
			if (!tmp) break;
			names = tmp;
		}
		size = offsetof(struct dirent,d_name) + strlen(de->d_name) + 1;
		names[cnt] = malloc(size);
		if (!names[cnt]) break;
		memcpy(names[cnt++], de, size);
	}

	closedir(d);

	if (errno) {
		old_errno = errno;
		if (names) while (cnt-->0) free(names[cnt]);
		free(names);
		errno = old_errno;
		return -1;
	}

	if (cmp) qsort(names, cnt, sizeof *names, (int (*)(const void *, const void *))cmp);
	*res = names;
	return cnt;
}

LFS64(scandir);
