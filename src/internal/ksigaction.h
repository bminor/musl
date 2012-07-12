struct k_sigaction {
	void (*handler)(int);
	unsigned long flags;
	void (*restorer)(void);
	unsigned mask[2];
};
