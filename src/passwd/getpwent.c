#include "pwf.h"

static FILE *f;

void setpwent()
{
	if (f) fclose(f);
	f = 0;
}

weak_alias(setpwent, endpwent);

struct passwd *getpwent()
{
	static char *line;
	static struct passwd pw;
	size_t size=0;
	if (!f) f = fopen("/etc/passwd", "rb");
	if (!f) return 0;
	return __getpwent_a(f, &pw, &line, &size);
}

struct passwd *getpwuid(uid_t uid)
{
	struct passwd *pw;
	setpwent();
	while ((pw=getpwent()) && pw->pw_uid != uid);
	endpwent();
	return pw;
}

struct passwd *getpwnam(const char *name)
{
	struct passwd *pw;
	setpwent();
	while ((pw=getpwent()) && strcmp(pw->pw_name, name));
	endpwent();
	return pw;
}
