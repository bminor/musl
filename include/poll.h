#ifndef	_POLL_H
#define	_POLL_H

#ifdef __cplusplus
extern "C" {
#endif

#define POLLIN     0x001
#define POLLPRI    0x002
#define POLLOUT    0x004
#define POLLERR    0x008
#define POLLHUP    0x010
#define POLLNVAL   0x020
#define POLLRDNORM 0x040
#define POLLRDBAND 0x080
#define POLLWRNORM 0x100
#define POLLWRBAND 0x200
#define POLLMSG    0x400

typedef unsigned int nfds_t;

struct pollfd
{
	int fd;
	short events;
	short revents;
};

int poll (struct pollfd *, nfds_t, int);

#ifdef __cplusplus
}
#endif

#endif
