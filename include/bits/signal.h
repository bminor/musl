struct __siginfo
{
	int si_signo;
	int si_errno;
	int si_code;
	union
	{
		char __pad[128 - 3*sizeof(int)];
		struct {
			pid_t si_pid;
			uid_t si_uid;
			union sigval si_sigval;
		} __rt;
		struct {
			unsigned int si_timer1;
			unsigned int si_timer2;
		} __timer;
		struct {
			pid_t si_pid;
			uid_t si_uid;
			int si_status;
			clock_t si_utime;
			clock_t si_stime;
		} __sigchld;
		struct {
			void *si_addr;
		} __sigfault;
		struct {
			long int si_band;
			int si_fd;
		} __sigpoll;
	} __si_fields;
};

#define si_pid     __si_fields.__sigchld.si_pid
#define si_uid     __si_fields.__sigchld.si_uid
#define si_status  __si_fields.__sigchld.si_status
#define si_utime   __si_fields.__sigchld.si_utime
#define si_stime   __si_fields.__sigchld.si_stime
#define si_value   __si_fields.__rt.si_sigval
#define si_addr    __si_fields.__sigfault.si_addr
#define si_band    __si_fields.__sigpoll.si_band

#define SA_NOCLDSTOP  1
#define SA_NOCLDWAIT  2
#define SA_SIGINFO    4
#define SA_ONSTACK    0x08000000
#define SA_RESTART    0x10000000
#define SA_NODEFER    0x40000000
#define SA_RESETHAND  0x80000000

#define SS_ONSTACK    1
#define SS_DISABLE    2

#define SIG_BLOCK     0
#define SIG_UNBLOCK   1
#define SIG_SETMASK   2

#define SIG_ERR  ((void (*)(int))-1)
#define SIG_DFL  ((void (*)(int)) 0)
#define SIG_IGN  ((void (*)(int)) 1)
#define SIG_HOLD ((void (*)(int)) 2)

#define NSIG      64

#define SIGHUP    1
#define SIGINT    2
#define SIGQUIT   3
#define SIGILL    4
#define SIGTRAP   5
#define SIGABRT   6
#define SIGBUS    7
#define SIGFPE    8
#define SIGKILL   9
#define SIGUSR1   10
#define SIGSEGV   11
#define SIGUSR2   12
#define SIGPIPE   13
#define SIGALRM   14
#define SIGTERM   15
#define SIGSTKFLT 16
#define SIGCHLD   17
#define SIGCONT   18
#define SIGSTOP   19
#define SIGTSTP   20
#define SIGTTIN   21
#define SIGTTOU   22
#define SIGURG    23
#define SIGXCPU   24
#define SIGXFSZ   25
#define SIGVTALRM 26
#define SIGPROF   27
#define SIGWINCH  28
#define SIGIO     29
#define SIGPOLL   29
#define SIGPWR    30
#define SIGSYS    31
#define SIGUNUSED SIGSYS
