#if __BYTE_ORDER == __BIG_ENDIAN
#define ENDIAN_SUFFIX "eb"
#else
#define ENDIAN_SUFFIX ""
#endif

#define LDSO_ARCH "sh" ENDIAN_SUFFIX

#define TPOFF_K 8

#define REL_SYMBOLIC    R_SH_DIR32
#define REL_OFFSET      R_SH_REL32
#define REL_GOT         R_SH_GLOB_DAT
#define REL_PLT         R_SH_JMP_SLOT
#define REL_RELATIVE    R_SH_RELATIVE
#define REL_COPY        R_SH_COPY
#define REL_DTPMOD      R_SH_TLS_DTPMOD32
#define REL_DTPOFF      R_SH_TLS_DTPOFF32
#define REL_TPOFF       R_SH_TLS_TPOFF32

#define CRTJMP(pc,sp) __asm__ __volatile__( \
	"jmp @%0 ; mov %1,r15" : : "r"(pc), "r"(sp) : "memory" )
