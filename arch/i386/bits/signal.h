#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE)

struct __fpstate {
	unsigned long __x[7];
	unsigned char __y[80];
	unsigned long __z;
};

typedef struct {
	unsigned long __gregs[19];
	void *__fpregs;
	unsigned long __oldmask, __cr2;
} mcontext_t;

typedef struct __ucontext {
	unsigned long uc_flags;
	struct __ucontext *uc_link;
	stack_t uc_stack;
	mcontext_t uc_mcontext;
	sigset_t uc_sigmask;
	struct __fpstate __fpregs_mem;
} ucontext_t;

#ifdef _GNU_SOURCE
struct sigcontext {
	unsigned short gs, __gsh, fs, __fsh, es, __esh, ds, __dsh;
	unsigned long edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned long trapno, err, eip;
	unsigned short cs, __csh;
	unsigned long eflags, esp_at_signal;
	unsigned short ss, __ssh;
	struct __fpstate *fpstate;
	unsigned long oldmask, cr2;
};
#endif

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

#define SI_ASYNCNL (-60)
#define SI_TKILL (-6)
#define SI_SIGIO (-5)
#define SI_ASYNCIO (-4)
#define SI_MESGQ (-3)
#define SI_TIMER (-2)
#define SI_QUEUE (-1)
#define SI_USER 0
#define SI_KERNEL 128

#define FPE_INTDIV 1
#define FPE_INTOVF 2
#define FPE_FLTDIV 3
#define FPE_FLTOVF 4
#define FPE_FLTUNT 5
#define FPE_FLTRES 6
#define FPE_FLTINV 7
#define FPE_FLTSUB 8

#define ILL_ILLOPC 1
#define ILL_ILLOPN 2
#define ILL_ILLADR 3
#define ILL_ILLTRP 4
#define ILL_PRVOPC 5
#define ILL_PRVREG 6
#define ILL_COPROC 7
#define ILL_BADSTK 8

#define SEGV_MAPERR 1
#define SEGV_ACCERR 2

#define BUS_ADRALN 1
#define BUS_ADRERR 2
#define BUS_OBJERR 3

#define CLD_EXITED 1
#define CLD_KILLED 2
#define CLD_DUMPED 3
#define CLD_TRAPPED 4
#define CLD_STOPPED 5
#define CLD_CONTINUED 6

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE)
#define TRAP_BRKPT 1
#define TRAP_TRACE 2
#define POLL_IN 1
#define POLL_OUT 2
#define POLL_MSG 3
#define POLL_ERR 4
#define POLL_PRI 5
#define POLL_HUP 6
#define SS_ONSTACK    1
#define SS_DISABLE    2
#define MINSIGSTKSZ 2048
#define SIGSTKSZ 8192
#endif

#define SA_NOCLDSTOP  1
#define SA_NOCLDWAIT  2
#define SA_SIGINFO    4
#define SA_ONSTACK    0x08000000
#define SA_RESTART    0x10000000
#define SA_NODEFER    0x40000000
#define SA_RESETHAND  0x80000000
#define SA_RESTORER   0x04000000

#define SIG_BLOCK     0
#define SIG_UNBLOCK   1
#define SIG_SETMASK   2

#endif

#ifdef _GNU_SOURCE
#define NSIG      64
#endif

#define SIG_ERR  ((void (*)(int))-1)
#define SIG_DFL  ((void (*)(int)) 0)
#define SIG_IGN  ((void (*)(int)) 1)
#define SIG_HOLD ((void (*)(int)) 2)

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
