#include <stdio.h>
#include <string.h>
#include <mntent.h>

FILE *setmntent(const char *name, const char *mode)
{
	return fopen(name, mode);
}

int endmntent(FILE *f)
{
	fclose(f);
	return 1;
}

struct mntent *getmntent(FILE *f)
{
	static char linebuf[256];
	static struct mntent mnt;
	int cnt, n[8];

	mnt.mnt_freq = 0;
	mnt.mnt_passno = 0;

	do {
		fgets(linebuf, sizeof linebuf, f);
		if (feof(f)) return NULL;
		cnt = sscanf(linebuf, " %n%*s%n %n%*s%n %n%*s%n %n%*s%n %d %d",
			n, n+1, n+2, n+3, n+4, n+5, n+6, n+7,
			&mnt.mnt_freq, &mnt.mnt_passno);
	} while (cnt >= 8 && linebuf[n[0]] != '#');

	linebuf[n[1]] = 0;
	linebuf[n[3]] = 0;
	linebuf[n[5]] = 0;
	linebuf[n[7]] = 0;

	mnt.mnt_fsname = linebuf+n[0];
	mnt.mnt_dir = linebuf+n[2];
	mnt.mnt_type = linebuf+n[4];
	mnt.mnt_opts = linebuf+n[6];

	return &mnt;
}

int addmntent(FILE *f, const struct mntent *mnt)
{
	fseek(f, 0, SEEK_END);
	return fprintf(f, "%s\t%s\t%s\t%s\t%d\t%d\n",
		mnt->mnt_fsname, mnt->mnt_dir, mnt->mnt_type, mnt->mnt_opts,
		mnt->mnt_freq, mnt->mnt_passno) < 0;
}

char *hasmntopt(const struct mntent *mnt, const char *opt)
{
	return strstr(mnt->mnt_opts, opt);
}
