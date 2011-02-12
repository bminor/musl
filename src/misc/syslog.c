#include <stdarg.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include "libc.h"

static int lock;
static const char *log_ident;
static int log_opt;
static int log_facility = LOG_USER;
static int log_mask = 0xff;
static FILE *log_f;

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
	LOCK(&lock);
	if (log_f) fclose(log_f);
	log_f = NULL;
	UNLOCK(&lock);
}

static void __openlog(const char *ident, int opt, int facility)
{
	int fd;

	log_ident = ident;
	log_opt = opt;
	log_facility = facility;

	if (!(opt & LOG_NDELAY) || log_f) return;

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	fcntl(fd, F_SETFD, FD_CLOEXEC);
	if (connect(fd, (void *)&log_addr, sizeof(short) + sizeof "/dev/log") < 0)
		close(fd);
	else log_f = fdopen(fd, "wb");
}

void openlog(const char *ident, int opt, int facility)
{
	LOCK(&lock);
	__openlog(ident, opt, facility);
	UNLOCK(&lock);
}

void syslog(int priority, const char *message, ...)
{
	struct sigaction sa;
	va_list ap;
	char timebuf[16];
	time_t now;
	struct tm tm;
	//const char *fmt, *ident, *sep;
	//int i;

	if (!(log_mask & LOG_MASK(priority&7)) || (priority&~0x3ff)) return;

	LOCK(&lock);

	if (!log_f) __openlog(log_ident, log_opt | LOG_NDELAY, log_facility);
	if (!log_f) {
		UNLOCK(&lock);
		return;
	}

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGPIPE, &sa, &sa) < 0) {
		// we must abandon logging or we might cause SIGPIPE
		UNLOCK(&lock);
		return;
	}

	now = time(NULL);
	gmtime_r(&now, &tm);
	strftime(timebuf, sizeof timebuf, "%b %e %T", &tm);

	fprintf(log_f, "<%d>%s ", priority, timebuf);
	if (log_ident) fprintf(log_f, "%s", log_ident);
	if (log_opt & LOG_PID) fprintf(log_f, "[%d]", getpid());
	if (log_ident) fprintf(log_f, ": ");

	va_start(ap, message);
	vfprintf(log_f, message, ap);
	va_end(ap);
	fputc(0, log_f);
	fflush(log_f);

	// Note: LOG_CONS is not supported because it is annoying!!
	// syslogd will send messages to console if it deems them appropriate!

	sigaction(SIGPIPE, &sa, NULL);

	UNLOCK(&lock);
}
