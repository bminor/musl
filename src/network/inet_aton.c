#include <sys/socket.h>
#include <arpa/inet.h>

int inet_aton(const char *cp, struct in_addr *inp)
{
	return inet_pton(AF_INET, cp, (void *)inp) > 0;
}
