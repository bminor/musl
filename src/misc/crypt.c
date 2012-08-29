#include <unistd.h>
#include <crypt.h>

char *__crypt_r(const char *, const char *, struct crypt_data *);

char *crypt(const char *key, const char *salt)
{
	/* Note: update this size when we add more hash types */
	static char buf[128];
	return __crypt_r(key, salt, (struct crypt_data *)buf);
}
