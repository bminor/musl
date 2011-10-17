struct user_fpregs_struct
{
	struct fp_reg {
		unsigned sign1:1;
		unsigned unused:15;
		unsigned sign2:1;
		unsigned exponent:14;
		unsigned j:1;
		unsigned mantissa1:31;
		unsigned mantissa0:32;
	} fpregs[8];
	unsigned fpsr:32;
	unsigned fpcr:32;
	unsigned char ftype[8];
	unsigned int init_flag;
};

struct user_regs_struct
{
	unsigned long uregs[18];
};

struct user
{
	struct user_regs_struct		regs;
	int				u_fpvalid;
	unsigned long			u_tsize;
	unsigned long			u_dsize;
	unsigned long			u_ssize;
	unsigned long			start_code;
	unsigned long			start_stack;
	long				signal;
	int				reserved;
	struct user_regs_struct		*u_ar0;
	unsigned long int		magic;
	char				u_comm[32];
	int				u_debugreg[8];
	struct user_fpregs_struct	u_fp;
	struct user_fpregs_struct	*u_fp0;
};
