#if __BYTE_ORDER == __BIG_ENDIAN
#define ENDIAN_SUFFIX "eb"
#else
#define ENDIAN_SUFFIX ""
#endif

#define LDSO_ARCH "sh" ENDIAN_SUFFIX

#define TPOFF_K 8

static int remap_rel(int type)
{
	switch(type) {
	case R_SH_DIR32:
		return REL_SYMBOLIC;
	case R_SH_REL32:
		return REL_OFFSET;
	case R_SH_GLOB_DAT:
		return REL_GOT;
	case R_SH_JMP_SLOT:
		return REL_PLT;
	case R_SH_RELATIVE:
		return REL_RELATIVE;
	case R_SH_COPY:
		return REL_COPY;
	case R_SH_TLS_DTPMOD32:
		return REL_DTPMOD;
	case R_SH_TLS_DTPOFF32:
		return REL_DTPOFF;
	case R_SH_TLS_TPOFF32:
		return REL_TPOFF;
	}
	return 0;
}
