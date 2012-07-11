struct k_sigaction {
	unsigned flags;
	void (*handler)(int);
	unsigned long mask[4];
	void (*restorer)();
};
