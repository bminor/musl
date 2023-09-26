#define ELF_NGREG    45
#define ELF_NFPREG   33

typedef unsigned long elf_greg_t, elf_gregset_t[ELF_NGREG];
typedef double elf_fpreg_t, elf_fpregset_t[ELF_NFPREG];
