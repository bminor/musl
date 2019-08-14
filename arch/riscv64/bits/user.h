struct user_fpregs_struct {
	double f[32];
	unsigned int fcsr;
};

#define ELF_NGREG 32
typedef unsigned long elf_greg_t, elf_gregset_t[ELF_NGREG];
typedef struct user_fpregs_struct elf_fpregset_t;
