#include <crypt.h>
#include "libc.h"

struct crypt_data;

char *__crypt_des(const char *, const char *, char *);
char *__crypt_md5(const char *, const char *, char *);

char *__crypt_r(const char *key, const char *salt, struct crypt_data *data)
{
	char *output = (char *)data;
#if 0
	/* MD5 or SHA? */
	if (salt[0] == '$' && salt[1] && salt[2] == '$') {
		if (salt[1] == '1')
			return __crypt_md5((char *)data, key, salt);
		else
			return "x";
	}
#endif
	return __crypt_des(key, salt, output);
}

weak_alias(__crypt_r, crypt_r);
