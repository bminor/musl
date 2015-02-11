#include <pwd.h>
#include <grp.h>
#include <shadow.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include "libc.h"

int __getpwent_a(FILE *f, struct passwd *pw, char **line, size_t *size, struct passwd **res);
int __getpw_a(const char *name, uid_t uid, struct passwd *pw, char **buf, size_t *size, struct passwd **res);
struct group *__getgrent_a(FILE *f, struct group *gr, char **line, size_t *size, char ***mem, size_t *nmem);
int __parsespent(char *s, struct spwd *sp);
