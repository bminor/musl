#include <crypt.h>
#include "libc.h"

struct crypt_data;

char *__crypt_des(const char *, const char *, char *);
char *__crypt_md5(const char *, const char *, char *);
char *__crypt_blowfish(const char *, const char *, char *);
char *__crypt_sha256(const char *, const char *, char *);
char *__crypt_sha512(const char *, const char *, char *);

char *__crypt_r(const char *key, const char *salt, struct crypt_data *data)
{
	char *output = (char *)data;
	if (salt[0] == '$' && salt[1] && salt[2]) {
#if 0
		if (salt[1] == '1' && salt[2] == '$')
			return __crypt_md5(key, salt, output);
#endif
		if (salt[1] == '2' && salt[3] == '$')
			return __crypt_blowfish(key, salt, output);
		if (salt[1] == '5' && salt[2] == '$')
			return __crypt_sha256(key, salt, output);
		if (salt[1] == '6' && salt[2] == '$')
			return __crypt_sha512(key, salt, output);
	}
	return __crypt_des(key, salt, output);
}

weak_alias(__crypt_r, crypt_r);
