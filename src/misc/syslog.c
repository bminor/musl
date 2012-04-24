#include <stdarg.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include "libc.h"

static int lock[2];
static const char *log_ident;
static int log_opt;
static int log_facility = LOG_USER;
static int log_mask = 0xff;
static int log_fd = -1;

int setlogmask(int maskpri)
{
	int old = log_mask;
	if (maskpri) log_mask = maskpri;
	return old;
}

static const struct {
	short sun_family;
	char sun_path[9];
} log_addr = {
	AF_UNIX,
	"/dev/log"
};

void closelog(void)
{
	int cs;
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
	LOCK(lock);
	close(log_fd);
	log_fd = -1;
	UNLOCK(lock);
	pthread_setcancelstate(cs, 0);
}

static void __openlog(const char *ident, int opt, int facility)
{
	log_ident = ident;
	log_opt = opt;
	log_facility = facility;

	if (!(opt & LOG_NDELAY) || log_fd>=0) return;

	log_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	fcntl(log_fd, F_SETFD, FD_CLOEXEC);
}

void openlog(const char *ident, int opt, int facility)
{
	int cs;
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
	LOCK(lock);
	__openlog(ident, opt, facility);
	UNLOCK(lock);
	pthread_setcancelstate(cs, 0);
}

static void _vsyslog(int priority, const char *message, va_list ap)
{
	char timebuf[16];
	time_t now;
	struct tm tm;
	char buf[256];
	int pid;
	int l, l2;

	if (log_fd < 0) {
		__openlog(log_ident, log_opt | LOG_NDELAY, log_facility);
		if (log_fd < 0) {
			UNLOCK(lock);
			return;
		}
	}

	now = time(NULL);
	gmtime_r(&now, &tm);
	strftime(timebuf, sizeof timebuf, "%b %e %T", &tm);

	pid = (log_opt & LOG_PID) ? getpid() : 0;
	l = snprintf(buf, sizeof buf, "<%d>%s %s%s%.0d%s: ",
		priority, timebuf,
		log_ident ? log_ident : "",
		"["+!pid, pid, "]"+!pid);
	l2 = vsnprintf(buf+l, sizeof buf - l, message, ap);
	if (l2 >= 0) {
		l += l2;
		if (buf[l-1] != '\n') buf[l++] = '\n';
		sendto(log_fd, buf, l, 0, (void *)&log_addr, 11);
	}

	UNLOCK(lock);
}

void __vsyslog(int priority, const char *message, va_list ap)
{
	int cs;
	if (!(log_mask & LOG_MASK(priority&7)) || (priority&~0x3ff)) return;
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
	LOCK(lock);
	_vsyslog(priority, message, ap);
	UNLOCK(lock);
	pthread_setcancelstate(cs, 0);
}

void syslog(int priority, const char *message, ...)
{
	va_list ap;
	va_start(ap, message);
	__vsyslog(priority, message, ap);
	va_end(ap);
}

weak_alias(__vsyslog, vsyslog);
