#include <string.h>
#include <elf.h>
#include <endian.h>

#if __BYTE_ORDER == __BIG_ENDIAN
#define ENDIAN_SUFFIX "_be"
#else
#define ENDIAN_SUFFIX ""
#endif

#define LDSO_ARCH "aarch64" ENDIAN_SUFFIX

#define NO_LEGACY_INITFINI

#define TPOFF_K 16

static int remap_rel(int type)
{
	switch(type) {
	case R_AARCH64_ABS64:
		return REL_SYMBOLIC;
	case R_AARCH64_GLOB_DAT:
		return REL_GOT;
	case R_AARCH64_JUMP_SLOT:
		return REL_PLT;
	case R_AARCH64_RELATIVE:
		return REL_RELATIVE;
	case R_AARCH64_COPY:
		return REL_COPY;
	case R_AARCH64_TLS_DTPMOD64:
		return REL_DTPMOD;
	case R_AARCH64_TLS_DTPREL64:
		return REL_DTPOFF;
	case R_AARCH64_TLS_TPREL64:
		return REL_TPOFF;
	case R_AARCH64_TLSDESC:
		return REL_TLSDESC;
	}
	return 0;
}
