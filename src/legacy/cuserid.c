#define _GNU_SOURCE
#include <pwd.h>
#include <stdio.h>
#include <unistd.h>

char *cuserid(char *buf)
{
	static char usridbuf[L_cuserid];
	struct passwd pw, *ppw;
	long pwb[256];
	if (getpwuid_r(geteuid(), &pw, (void *)pwb, sizeof pwb, &ppw))
		return 0;
	if (!buf) buf = usridbuf;
	snprintf(buf, L_cuserid, "%s", pw.pw_name);
	return buf;
}
