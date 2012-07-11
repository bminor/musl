struct k_sigaction {
	void (*handler)(int);
	unsigned long flags;
	void (*restorer)(void);
	long mask[2];
};
