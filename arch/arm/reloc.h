#include <string.h>
#include <elf.h>
#include <endian.h>

#if __BYTE_ORDER == __BIG_ENDIAN
#define ENDIAN_SUFFIX "eb"
#else
#define ENDIAN_SUFFIX ""
#endif

#if __SOFTFP__
#define FP_SUFFIX ""
#else
#define FP_SUFFIX "hf"
#endif

#define LDSO_ARCH "arm" ENDIAN_SUFFIX FP_SUFFIX

#define NO_LEGACY_INITFINI

#define TPOFF_K 8

static int remap_rel(int type)
{
	switch(type) {
	case R_ARM_ABS32:
		return REL_SYMBOLIC;
	case R_ARM_GLOB_DAT:
		return REL_GOT;
	case R_ARM_JUMP_SLOT:
		return REL_PLT;
	case R_ARM_RELATIVE:
		return REL_RELATIVE;
	case R_ARM_COPY:
		return REL_COPY;
	case R_ARM_TLS_DTPMOD32:
		return REL_DTPMOD;
	case R_ARM_TLS_DTPOFF32:
		return REL_DTPOFF;
	case R_ARM_TLS_TPOFF32:
		return REL_TPOFF;
	}
	return 0;
}
