struct user {
	unsigned long regs[45+64];
	unsigned long u_tsize, u_dsize, u_ssize;
	unsigned long start_code, start_data, start_stack;
	long signal;
	void *u_ar0;
	unsigned long magic;
	char u_comm[32];
};
