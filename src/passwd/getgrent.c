#include "pwf.h"

static FILE *f;

void setgrent()
{
	if (f) fclose(f);
	f = 0;
}

weak_alias(setgrent, endgrent);

struct group *getgrent()
{
	static char *line, **mem;
	static struct group gr;
	size_t size=0, nmem=0;
	if (!f) f = fopen("/etc/group", "rb");
	if (!f) return 0;
	return __getgrent_a(f, &gr, &line, &size, &mem, &nmem);
}

struct group *getgrgid(gid_t gid)
{
	struct group *gr;
	setgrent();
	while ((gr=getgrent()) && gr->gr_gid != gid);
	endgrent();
	return gr;
}

struct group *getgrnam(const char *name)
{
	struct group *gr;
	setgrent();
	while ((gr=getgrent()) && strcmp(gr->gr_name, name));
	endgrent();
	return gr;
}
