#ifndef	_SYS_SOCKET_H
#define	_SYS_SOCKET_H
#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_socklen_t
#define __NEED_sa_family_t
#define __NEED_size_t
#define __NEED_ssize_t
#define __NEED_uid_t
#define __NEED_pid_t
#define __NEED_gid_t

#include <bits/alltypes.h>

#include <bits/socket.h>

struct sockaddr
{
	sa_family_t sa_family;
	char sa_data[14];
};

struct sockaddr_storage
{
	sa_family_t ss_family;
	long long __ss_align;
	char __ss_padding[128 - sizeof(sa_family_t) - sizeof(long long)];
};

int socket (int, int, int);
int socketpair (int, int, int, int [2]);

int shutdown (int, int);

int bind (int, const struct sockaddr *, socklen_t);
int connect (int, const struct sockaddr *, socklen_t);
int listen (int, int);
int accept (int, struct sockaddr *, socklen_t *);

int getsockname (int, struct sockaddr *, socklen_t *);
int getpeername (int, struct sockaddr *, socklen_t *);

ssize_t send (int, const void *, size_t, int);
ssize_t recv (int, void *, size_t, int);
ssize_t sendto (int, const void *, size_t, int, const struct sockaddr *, socklen_t);
ssize_t recvfrom (int, void *, size_t, int, struct sockaddr *, socklen_t *);
ssize_t sendmsg (int, const struct msghdr *, int);
ssize_t recvmsg (int, struct msghdr *, int);

int getsockopt (int, int, int, void *, socklen_t *);
int setsockopt (int, int, int, const void *, socklen_t);

int sockatmark (int);

#define SHUT_RD 0
#define SHUT_WR 1
#define SHUT_RDWR 2

#ifdef __cplusplus
}
#endif
#endif
