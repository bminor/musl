#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <elf.h>
#include <sys/mman.h>
#include <limits.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>
#include <elf.h>
#include <setjmp.h>
#include <pthread.h>
#include <ctype.h>
#include <dlfcn.h>

static int errflag;
static char errbuf[128];

#ifdef SHARED

#if ULONG_MAX == 0xffffffff
typedef Elf32_Ehdr Ehdr;
typedef Elf32_Phdr Phdr;
typedef Elf32_Sym Sym;
#define R_TYPE(x) ((x)&255)
#define R_SYM(x) ((x)>>8)
#else
typedef Elf64_Ehdr Ehdr;
typedef Elf64_Phdr Phdr;
typedef Elf64_Sym Sym;
#define R_TYPE(x) ((x)&0xffffffff)
#define R_SYM(x) ((x)>>32)
#endif

struct debug {
	int ver;
	void *head;
	void (*bp)(void);
	int state;
	void *base;
};

struct dso {
	unsigned char *base;
	char *name;
	size_t *dynv;
	struct dso *next, *prev;

	int refcnt;
	Sym *syms;
	uint32_t *hashtab;
	uint32_t *ghashtab;
	char *strings;
	unsigned char *map;
	size_t map_len;
	dev_t dev;
	ino_t ino;
	signed char global;
	char relocated;
	char constructed;
	struct dso **deps;
	char *shortname;
	char buf[];
};

#include "reloc.h"

void __init_ssp(size_t *);

static struct dso *head, *tail, *libc;
static char *env_path, *sys_path, *r_path;
static int rtld_used;
static int ssp_used;
static int runtime;
static int ldd_mode;
static int ldso_fail;
static jmp_buf rtld_fail;
static pthread_rwlock_t lock;
static struct debug debug;
static size_t *auxv;

struct debug *_dl_debug_addr = &debug;

#define AUX_CNT 24
#define DYN_CNT 34

static void decode_vec(size_t *v, size_t *a, size_t cnt)
{
	memset(a, 0, cnt*sizeof(size_t));
	for (; v[0]; v+=2) if (v[0]<cnt) {
		a[0] |= 1ULL<<v[0];
		a[v[0]] = v[1];
	}
}

static int search_vec(size_t *v, size_t *r, size_t key)
{
	for (; v[0]!=key; v+=2)
		if (!v[0]) return 0;
	*r = v[1];
	return 1;
}

static uint32_t sysv_hash(const char *s0)
{
	const unsigned char *s = (void *)s0;
	uint_fast32_t h = 0;
	while (*s) {
		h = 16*h + *s++;
		h ^= h>>24 & 0xf0;
	}
	return h & 0xfffffff;
}

static uint32_t gnu_hash(const char *s0)
{
	const unsigned char *s = (void *)s0;
	uint_fast32_t h = 5381;
	for (; *s; s++)
		h = h*33 + *s;
	return h;
}

static Sym *sysv_lookup(const char *s, uint32_t h, struct dso *dso)
{
	size_t i;
	Sym *syms = dso->syms;
	uint32_t *hashtab = dso->hashtab;
	char *strings = dso->strings;
	for (i=hashtab[2+h%hashtab[0]]; i; i=hashtab[2+hashtab[0]+i]) {
		if (!strcmp(s, strings+syms[i].st_name))
			return syms+i;
	}
	return 0;
}

static Sym *gnu_lookup(const char *s, uint32_t h1, struct dso *dso)
{
	Sym *sym;
	char *strings;
	uint32_t *hashtab = dso->ghashtab;
	uint32_t nbuckets = hashtab[0];
	uint32_t *buckets = hashtab + 4 + hashtab[2]*(sizeof(size_t)/4);
	uint32_t h2;
	uint32_t *hashval;
	uint32_t n = buckets[h1 % nbuckets];

	if (!n) return 0;

	strings = dso->strings;
	sym = dso->syms + n;
	hashval = buckets + nbuckets + (n - hashtab[1]);

	for (h1 |= 1; ; sym++) {
		h2 = *hashval++;
		if ((h1 == (h2|1)) && !strcmp(s, strings + sym->st_name))
			return sym;
		if (h2 & 1) break;
	}

	return 0;
}

#define OK_TYPES (1<<STT_NOTYPE | 1<<STT_OBJECT | 1<<STT_FUNC | 1<<STT_COMMON)
#define OK_BINDS (1<<STB_GLOBAL | 1<<STB_WEAK)

static void *find_sym(struct dso *dso, const char *s, int need_def)
{
	uint32_t h = 0, gh = 0;
	void *def = 0;
	if (dso->ghashtab) {
		gh = gnu_hash(s);
		if (gh == 0xf9040207 && !strcmp(s, "dlopen")) rtld_used = 1;
		if (gh == 0xf4dc4ae && !strcmp(s, "dlsym")) rtld_used = 1;
		if (gh == 0x1f4039c9 && !strcmp(s, "__stack_chk_fail")) ssp_used = 1;
	} else {
		h = sysv_hash(s);
		if (h == 0x6b366be && !strcmp(s, "dlopen")) rtld_used = 1;
		if (h == 0x6b3afd && !strcmp(s, "dlsym")) rtld_used = 1;
		if (h == 0x595a4cc && !strcmp(s, "__stack_chk_fail")) ssp_used = 1;
	}
	for (; dso; dso=dso->next) {
		Sym *sym;
		if (!dso->global) continue;
		if (dso->ghashtab) {
			if (!gh) gh = gnu_hash(s);
			sym = gnu_lookup(s, gh, dso);
		} else {
			if (!h) h = sysv_hash(s);
			sym = sysv_lookup(s, h, dso);
		}
		if (sym && (!need_def || sym->st_shndx) && sym->st_value
		 && (1<<(sym->st_info&0xf) & OK_TYPES)
		 && (1<<(sym->st_info>>4) & OK_BINDS)) {
			if (def && sym->st_info>>4 == STB_WEAK) continue;
			def = dso->base + sym->st_value;
			if (sym->st_info>>4 == STB_GLOBAL) break;
		}
	}
	return def;
}

static void do_relocs(struct dso *dso, size_t *rel, size_t rel_size, size_t stride)
{
	unsigned char *base = dso->base;
	Sym *syms = dso->syms;
	char *strings = dso->strings;
	Sym *sym;
	const char *name;
	size_t sym_val, sym_size;
	size_t *reloc_addr;
	void *ctx;
	int type;
	int sym_index;

	for (; rel_size; rel+=stride, rel_size-=stride*sizeof(size_t)) {
		reloc_addr = (void *)(base + rel[0]);
		type = R_TYPE(rel[1]);
		sym_index = R_SYM(rel[1]);
		if (sym_index) {
			sym = syms + sym_index;
			name = strings + sym->st_name;
			ctx = IS_COPY(type) ? head->next : head;
			sym_val = (size_t)find_sym(ctx, name, IS_PLT(type));
			if (!sym_val && sym->st_info>>4 != STB_WEAK) {
				snprintf(errbuf, sizeof errbuf,
					"Error relocating %s: %s: symbol not found",
					dso->name, name);
				if (runtime) longjmp(rtld_fail, 1);
				dprintf(2, "%s\n", errbuf);
				ldso_fail = 1;
				continue;
			}
			sym_size = sym->st_size;
		} else {
			sym_val = sym_size = 0;
		}
		do_single_reloc(reloc_addr, type, sym_val, sym_size, base, rel[2]);
	}
}

/* A huge hack: to make up for the wastefulness of shared libraries
 * needing at least a page of dirty memory even if they have no global
 * data, we reclaim the gaps at the beginning and end of writable maps
 * and "donate" them to the heap by setting up minimal malloc
 * structures and then freeing them. */

static void reclaim(unsigned char *base, size_t start, size_t end)
{
	size_t *a, *z;
	start = start + 6*sizeof(size_t)-1 & -4*sizeof(size_t);
	end = (end & -4*sizeof(size_t)) - 2*sizeof(size_t);
	if (start>end || end-start < 4*sizeof(size_t)) return;
	a = (size_t *)(base + start);
	z = (size_t *)(base + end);
	a[-2] = 1;
	a[-1] = z[0] = end-start + 2*sizeof(size_t) | 1;
	z[1] = 1;
	free(a);
}

static void reclaim_gaps(unsigned char *base, Phdr *ph, size_t phent, size_t phcnt)
{
	for (; phcnt--; ph=(void *)((char *)ph+phent)) {
		if (ph->p_type!=PT_LOAD) continue;
		if ((ph->p_flags&(PF_R|PF_W))!=(PF_R|PF_W)) continue;
		reclaim(base, ph->p_vaddr & -PAGE_SIZE, ph->p_vaddr);
		reclaim(base, ph->p_vaddr+ph->p_memsz,
			ph->p_vaddr+ph->p_memsz+PAGE_SIZE-1 & -PAGE_SIZE);
	}
}

static void *map_library(int fd, size_t *lenp, unsigned char **basep, size_t *dynp)
{
	Ehdr buf[(896+sizeof(Ehdr))/sizeof(Ehdr)];
	size_t phsize;
	size_t addr_min=SIZE_MAX, addr_max=0, map_len;
	size_t this_min, this_max;
	off_t off_start;
	Ehdr *eh;
	Phdr *ph;
	unsigned prot;
	unsigned char *map, *base;
	size_t dyn;
	size_t i;

	ssize_t l = read(fd, buf, sizeof buf);
	if (l<sizeof *eh) return 0;
	eh = buf;
	phsize = eh->e_phentsize * eh->e_phnum;
	if (phsize + sizeof *eh > l) return 0;
	if (eh->e_phoff + phsize > l) {
		l = pread(fd, buf+1, phsize, eh->e_phoff);
		if (l != phsize) return 0;
		eh->e_phoff = sizeof *eh;
	}
	ph = (void *)((char *)buf + eh->e_phoff);
	for (i=eh->e_phnum; i; i--, ph=(void *)((char *)ph+eh->e_phentsize)) {
		if (ph->p_type == PT_DYNAMIC)
			dyn = ph->p_vaddr;
		if (ph->p_type != PT_LOAD) continue;
		if (ph->p_vaddr < addr_min) {
			addr_min = ph->p_vaddr;
			off_start = ph->p_offset;
			prot = (((ph->p_flags&PF_R) ? PROT_READ : 0) |
				((ph->p_flags&PF_W) ? PROT_WRITE: 0) |
				((ph->p_flags&PF_X) ? PROT_EXEC : 0));
		}
		if (ph->p_vaddr+ph->p_memsz > addr_max) {
			addr_max = ph->p_vaddr+ph->p_memsz;
		}
	}
	if (!dyn) return 0;
	addr_max += PAGE_SIZE-1;
	addr_max &= -PAGE_SIZE;
	addr_min &= -PAGE_SIZE;
	off_start &= -PAGE_SIZE;
	map_len = addr_max - addr_min + off_start;
	/* The first time, we map too much, possibly even more than
	 * the length of the file. This is okay because we will not
	 * use the invalid part; we just need to reserve the right
	 * amount of virtual address space to map over later. */
	map = mmap((void *)addr_min, map_len, prot, MAP_PRIVATE, fd, off_start);
	if (map==MAP_FAILED) return 0;
	base = map - addr_min;
	ph = (void *)((char *)buf + eh->e_phoff);
	for (i=eh->e_phnum; i; i--, ph=(void *)((char *)ph+eh->e_phentsize)) {
		if (ph->p_type != PT_LOAD) continue;
		/* Reuse the existing mapping for the lowest-address LOAD */
		if ((ph->p_vaddr & -PAGE_SIZE) == addr_min) continue;
		this_min = ph->p_vaddr & -PAGE_SIZE;
		this_max = ph->p_vaddr+ph->p_memsz+PAGE_SIZE-1 & -PAGE_SIZE;
		off_start = ph->p_offset & -PAGE_SIZE;
		prot = (((ph->p_flags&PF_R) ? PROT_READ : 0) |
			((ph->p_flags&PF_W) ? PROT_WRITE: 0) |
			((ph->p_flags&PF_X) ? PROT_EXEC : 0));
		if (mmap(base+this_min, this_max-this_min, prot, MAP_PRIVATE|MAP_FIXED, fd, off_start) == MAP_FAILED)
			goto error;
		if (ph->p_memsz > ph->p_filesz) {
			size_t brk = (size_t)base+ph->p_vaddr+ph->p_filesz;
			size_t pgbrk = brk+PAGE_SIZE-1 & -PAGE_SIZE;
			memset((void *)brk, 0, pgbrk-brk & PAGE_SIZE-1);
			if (pgbrk-(size_t)base < this_max && mmap((void *)pgbrk, (size_t)base+this_max-pgbrk, prot, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) == MAP_FAILED)
				goto error;
		}
	}
	for (i=0; ((size_t *)(base+dyn))[i]; i+=2)
		if (((size_t *)(base+dyn))[i]==DT_TEXTREL) {
			if (mprotect(map, map_len, PROT_READ|PROT_WRITE|PROT_EXEC) < 0)
				goto error;
			break;
		}
	if (!runtime) reclaim_gaps(base, (void *)((char *)buf + eh->e_phoff),
		eh->e_phentsize, eh->e_phnum);
	*lenp = map_len;
	*basep = base;
	*dynp = dyn;
	return map;
error:
	munmap(map, map_len);
	return 0;
}

static int path_open(const char *name, const char *search, char *buf, size_t buf_size)
{
	const char *s=search, *z;
	int l, fd;
	for (;;) {
		while (*s==':') s++;
		if (!*s) return -1;
		z = strchr(s, ':');
		l = z ? z-s : strlen(s);
		snprintf(buf, buf_size, "%.*s/%s", l, s, name);
		if ((fd = open(buf, O_RDONLY))>=0) return fd;
		s += l;
	}
}

static void decode_dyn(struct dso *p)
{
	size_t dyn[DYN_CNT] = {0};
	decode_vec(p->dynv, dyn, DYN_CNT);
	p->syms = (void *)(p->base + dyn[DT_SYMTAB]);
	p->strings = (void *)(p->base + dyn[DT_STRTAB]);
	if (dyn[0]&(1<<DT_HASH))
		p->hashtab = (void *)(p->base + dyn[DT_HASH]);
	if (search_vec(p->dynv, dyn, DT_GNU_HASH))
		p->ghashtab = (void *)(p->base + *dyn);
}

static struct dso *load_library(const char *name)
{
	char buf[2*NAME_MAX+2];
	const char *pathname;
	unsigned char *base, *map;
	size_t dyno, map_len;
	struct dso *p;
	int fd;
	struct stat st;

	/* Catch and block attempts to reload the implementation itself */
	if (name[0]=='l' && name[1]=='i' && name[2]=='b') {
		static const char *rp, reserved[] =
			"c\0pthread\0rt\0m\0dl\0util\0xnet\0";
		char *z = strchr(name, '.');
		if (z) {
			size_t l = z-name;
			for (rp=reserved; *rp && memcmp(name+3, rp, l-3); rp+=strlen(rp)+1);
			if (*rp) {
				if (!libc->prev) {
					tail->next = libc;
					libc->prev = tail;
					tail = libc->next ? libc->next : libc;
				}
				return libc;
			}
		}
	}
	if (strchr(name, '/')) {
		pathname = name;
		fd = open(name, O_RDONLY);
	} else {
		/* Search for the name to see if it's already loaded */
		for (p=head->next; p; p=p->next) {
			if (p->shortname && !strcmp(p->shortname, name)) {
				p->refcnt++;
				return p;
			}
		}
		if (strlen(name) > NAME_MAX) return 0;
		fd = -1;
		if (r_path) fd = path_open(name, r_path, buf, sizeof buf);
		if (fd < 0 && env_path) fd = path_open(name, env_path, buf, sizeof buf);
		if (fd < 0) {
			if (!sys_path) {
				FILE *f = fopen(ETC_LDSO_PATH, "r");
				if (f) {
					if (getline(&sys_path, (size_t[1]){0}, f) > 0)
						sys_path[strlen(sys_path)-1]=0;
					fclose(f);
				}
			}
			if (sys_path) fd = path_open(name, sys_path, buf, sizeof buf);
			else fd = path_open(name, "/lib:/usr/local/lib:/usr/lib", buf, sizeof buf);
		}
		pathname = buf;
	}
	if (fd < 0) return 0;
	if (fstat(fd, &st) < 0) {
		close(fd);
		return 0;
	}
	for (p=head->next; p; p=p->next) {
		if (p->dev == st.st_dev && p->ino == st.st_ino) {
			/* If this library was previously loaded with a
			 * pathname but a search found the same inode,
			 * setup its shortname so it can be found by name. */
			if (!p->shortname) p->shortname = strrchr(p->name, '/')+1;
			close(fd);
			p->refcnt++;
			return p;
		}
	}
	map = map_library(fd, &map_len, &base, &dyno);
	close(fd);
	if (!map) return 0;
	p = calloc(1, sizeof *p + strlen(pathname) + 1);
	if (!p) {
		munmap(map, map_len);
		return 0;
	}

	p->map = map;
	p->map_len = map_len;
	p->base = base;
	p->dynv = (void *)(base + dyno);
	decode_dyn(p);

	p->dev = st.st_dev;
	p->ino = st.st_ino;
	p->refcnt = 1;
	p->name = p->buf;
	strcpy(p->name, pathname);
	/* Add a shortname only if name arg was not an explicit pathname. */
	if (pathname != name) p->shortname = strrchr(p->name, '/')+1;

	tail->next = p;
	p->prev = tail;
	tail = p;

	if (ldd_mode) dprintf(1, "\t%s => %s (%p)\n", name, pathname, base);

	return p;
}

static void load_deps(struct dso *p)
{
	size_t i, ndeps=0;
	struct dso ***deps = &p->deps, **tmp, *dep;
	for (; p; p=p->next) {
		for (i=0; p->dynv[i]; i+=2) {
			if (p->dynv[i] != DT_RPATH) continue;
			r_path = (void *)(p->strings + p->dynv[i+1]);
		}
		for (i=0; p->dynv[i]; i+=2) {
			if (p->dynv[i] != DT_NEEDED) continue;
			dep = load_library(p->strings + p->dynv[i+1]);
			if (!dep) {
				snprintf(errbuf, sizeof errbuf,
					"Error loading shared library %s: %m (needed by %s)",
					p->strings + p->dynv[i+1], p->name);
				if (runtime) longjmp(rtld_fail, 1);
				dprintf(2, "%s\n", errbuf);
				ldso_fail = 1;
				continue;
			}
			if (runtime) {
				tmp = realloc(*deps, sizeof(*tmp)*(ndeps+2));
				if (!tmp) longjmp(rtld_fail, 1);
				tmp[ndeps++] = dep;
				tmp[ndeps] = 0;
				*deps = tmp;
			}
		}
		r_path = 0;
	}
}

static void load_preload(char *s)
{
	int tmp;
	char *z;
	for (z=s; *z; s=z) {
		for (   ; *s && isspace(*s); s++);
		for (z=s; *z && !isspace(*z); z++);
		tmp = *z;
		*z = 0;
		load_library(s);
		*z = tmp;
	}
}

static void make_global(struct dso *p)
{
	for (; p; p=p->next) p->global = 1;
}

static void reloc_all(struct dso *p)
{
	size_t dyn[DYN_CNT] = {0};
	for (; p; p=p->next) {
		if (p->relocated) continue;
		decode_vec(p->dynv, dyn, DYN_CNT);
#ifdef NEED_ARCH_RELOCS
		do_arch_relocs(p, head);
#endif
		do_relocs(p, (void *)(p->base+dyn[DT_JMPREL]), dyn[DT_PLTRELSZ],
			2+(dyn[DT_PLTREL]==DT_RELA));
		do_relocs(p, (void *)(p->base+dyn[DT_REL]), dyn[DT_RELSZ], 2);
		do_relocs(p, (void *)(p->base+dyn[DT_RELA]), dyn[DT_RELASZ], 3);
		p->relocated = 1;
	}
}

static void free_all(struct dso *p)
{
	struct dso *n;
	while (p) {
		n = p->next;
		if (p->map && p!=libc && p!=head) free(p);
		p = n;
	}
}

static size_t find_dyn(Phdr *ph, size_t cnt, size_t stride)
{
	for (; cnt--; ph = (void *)((char *)ph + stride))
		if (ph->p_type == PT_DYNAMIC)
			return ph->p_vaddr;
	return 0;
}

static void find_map_range(Phdr *ph, size_t cnt, size_t stride, struct dso *p)
{
	size_t min_addr = -1, max_addr = 0;
	for (; cnt--; ph = (void *)((char *)ph + stride)) {
		if (ph->p_type != PT_LOAD) continue;
		if (ph->p_vaddr < min_addr)
			min_addr = ph->p_vaddr;
		if (ph->p_vaddr+ph->p_memsz > max_addr)
			max_addr = ph->p_vaddr+ph->p_memsz;
	}
	min_addr &= -PAGE_SIZE;
	max_addr = (max_addr + PAGE_SIZE-1) & -PAGE_SIZE;
	p->map = p->base + min_addr;
	p->map_len = max_addr - min_addr;
}

static void do_init_fini(struct dso *p)
{
	size_t dyn[DYN_CNT] = {0};
	for (; p; p=p->prev) {
		if (p->constructed) return;
		decode_vec(p->dynv, dyn, DYN_CNT);
		if (dyn[0] & (1<<DT_FINI))
			atexit((void (*)(void))(p->base + dyn[DT_FINI]));
		if (dyn[0] & (1<<DT_INIT))
			((void (*)(void))(p->base + dyn[DT_INIT]))();
		p->constructed = 1;
	}
}

void _dl_debug_state(void)
{
}

void *__dynlink(int argc, char **argv)
{
	size_t aux[AUX_CNT] = {0};
	size_t i;
	Phdr *phdr;
	Ehdr *ehdr;
	static struct dso builtin_dsos[3];
	struct dso *const app = builtin_dsos+0;
	struct dso *const lib = builtin_dsos+1;
	struct dso *const vdso = builtin_dsos+2;
	char *env_preload=0;
	size_t vdso_base;

	/* Find aux vector just past environ[] */
	for (i=argc+1; argv[i]; i++)
		if (!memcmp(argv[i], "LD_LIBRARY_PATH=", 16))
			env_path = argv[i]+16;
		else if (!memcmp(argv[i], "LD_PRELOAD=", 11))
			env_preload = argv[i]+11;
	auxv = (void *)(argv+i+1);

	decode_vec(auxv, aux, AUX_CNT);

	/* Only trust user/env if kernel says we're not suid/sgid */
	if ((aux[0]&0x7800)!=0x7800 || aux[AT_UID]!=aux[AT_EUID]
	  || aux[AT_GID]!=aux[AT_EGID] || aux[AT_SECURE]) {
		env_path = 0;
		env_preload = 0;
	}

	/* If the dynamic linker was invoked as a program itself, AT_BASE
	 * will not be set. In that case, we assume the base address is
	 * the start of the page containing the PHDRs; I don't know any
	 * better approach... */
	if (!aux[AT_BASE]) {
		aux[AT_BASE] = aux[AT_PHDR] & -PAGE_SIZE;
		aux[AT_PHDR] = aux[AT_PHENT] = aux[AT_PHNUM] = 0;
	}

	/* The dynamic linker load address is passed by the kernel
	 * in the AUX vector, so this is easy. */
	lib->base = (void *)aux[AT_BASE];
	lib->name = lib->shortname = "libc.so";
	lib->global = 1;
	ehdr = (void *)lib->base;
	find_map_range((void *)(aux[AT_BASE]+ehdr->e_phoff),
		ehdr->e_phnum, ehdr->e_phentsize, lib);
	lib->dynv = (void *)(lib->base + find_dyn(
		(void *)(aux[AT_BASE]+ehdr->e_phoff),
		ehdr->e_phnum, ehdr->e_phentsize));
	decode_dyn(lib);

	if (aux[AT_PHDR]) {
		size_t interp_off = 0;
		/* Find load address of the main program, via AT_PHDR vs PT_PHDR. */
		phdr = (void *)aux[AT_PHDR];
		for (i=aux[AT_PHNUM]; i; i--, phdr=(void *)((char *)phdr + aux[AT_PHENT])) {
			if (phdr->p_type == PT_PHDR)
				app->base = (void *)(aux[AT_PHDR] - phdr->p_vaddr);
			else if (phdr->p_type == PT_INTERP)
				interp_off = (size_t)phdr->p_vaddr;
		}
		if (interp_off) lib->name = (char *)app->base + interp_off;
		app->name = argv[0];
		app->dynv = (void *)(app->base + find_dyn(
			(void *)aux[AT_PHDR], aux[AT_PHNUM], aux[AT_PHENT]));
		find_map_range((void *)aux[AT_PHDR],
			aux[AT_PHNUM], aux[AT_PHENT], app);
	} else {
		int fd;
		char *ldname = argv[0];
		size_t dyno, l = strlen(ldname);
		if (l >= 3 && !strcmp(ldname+l-3, "ldd")) ldd_mode = 1;
		*argv++ = (void *)-1;
		if (argv[0] && !strcmp(argv[0], "--")) *argv++ = (void *)-1;
		if (!argv[0]) {
			dprintf(2, "musl libc/dynamic program loader\n");
			dprintf(2, "usage: %s pathname%s\n", ldname,
				ldd_mode ? "" : " [args]");
			_exit(1);
		}
		fd = open(argv[0], O_RDONLY);
		if (fd < 0) {
			dprintf(2, "%s: cannot load %s: %s\n", ldname, argv[0], strerror(errno));
			_exit(1);
		}
		runtime = 1;
		ehdr = (void *)map_library(fd, &app->map_len, &app->base, &dyno);
		if (!ehdr) {
			dprintf(2, "%s: %s: Not a valid dynamic program\n", ldname, argv[0]);
			_exit(1);
		}
		runtime = 0;
		close(fd);
		lib->name = ldname;
		app->name = argv[0];
		app->dynv = (void *)(app->base + dyno);
		aux[AT_ENTRY] = ehdr->e_entry;
	}
	app->global = 1;
	app->constructed = 1;
	decode_dyn(app);

	/* Attach to vdso, if provided by the kernel */
	if (search_vec(auxv, &vdso_base, AT_SYSINFO_EHDR)) {
		ehdr = (void *)vdso_base;
		phdr = (void *)(vdso_base + ehdr->e_phoff);
		for (i=ehdr->e_phnum; i; i--, phdr=(void *)((char *)phdr + ehdr->e_phentsize)) {
			if (phdr->p_type == PT_DYNAMIC)
				vdso->dynv = (void *)(vdso_base + phdr->p_offset);
			if (phdr->p_type == PT_LOAD)
				vdso->base = (void *)(vdso_base - phdr->p_vaddr + phdr->p_offset);
		}
		vdso->name = vdso->shortname = "linux-gate.so.1";
		vdso->global = 1;
		decode_dyn(vdso);
		vdso->prev = lib;
		lib->next = vdso;
	}

	/* Initial dso chain consists only of the app. We temporarily
	 * append the dynamic linker/libc so we can relocate it, then
	 * restore the initial chain in preparation for loading third
	 * party libraries (preload/needed). */
	head = tail = app;
	libc = lib;
	app->next = lib;
	reloc_all(lib);
	app->next = 0;

	/* PAST THIS POINT, ALL LIBC INTERFACES ARE FULLY USABLE. */

	/* Donate unused parts of app and library mapping to malloc */
	reclaim_gaps(app->base, (void *)aux[AT_PHDR], aux[AT_PHENT], aux[AT_PHNUM]);
	ehdr = (void *)lib->base;
	reclaim_gaps(lib->base, (void *)(lib->base+ehdr->e_phoff),
		ehdr->e_phentsize, ehdr->e_phnum);

	/* Load preload/needed libraries, add their symbols to the global
	 * namespace, and perform all remaining relocations. The main
	 * program must be relocated LAST since it may contain copy
	 * relocations which depend on libraries' relocations. */
	if (env_preload) load_preload(env_preload);
	load_deps(app);
	make_global(app);
	reloc_all(app->next);
	reloc_all(app);

	if (ldso_fail) _exit(127);
	if (ldd_mode) _exit(0);

	/* Switch to runtime mode: any further failures in the dynamic
	 * linker are a reportable failure rather than a fatal startup
	 * error. If the dynamic loader (dlopen) will not be used, free
	 * all memory used by the dynamic linker. */
	runtime = 1;

#ifndef DYNAMIC_IS_RO
	for (i=0; app->dynv[i]; i+=2)
		if (app->dynv[i]==DT_DEBUG)
			app->dynv[i+1] = (size_t)&debug;
#endif
	debug.ver = 1;
	debug.bp = _dl_debug_state;
	debug.head = head;
	debug.base = lib->base;
	debug.state = 0;
	_dl_debug_state();

	if (ssp_used) __init_ssp(auxv);

	do_init_fini(tail);

	if (!rtld_used) {
		free_all(head);
		free(sys_path);
		reclaim((void *)builtin_dsos, 0, sizeof builtin_dsos);
	}

	errno = 0;
	return (void *)aux[AT_ENTRY];
}

void *dlopen(const char *file, int mode)
{
	struct dso *volatile p, *orig_tail = tail, *next;
	size_t i;
	int cs;

	if (!file) return head;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
	pthread_rwlock_wrlock(&lock);

	if (setjmp(rtld_fail)) {
		/* Clean up anything new that was (partially) loaded */
		if (p->deps) for (i=0; p->deps[i]; i++)
			if (p->deps[i]->global < 0)
				p->deps[i]->global = 0;
		for (p=orig_tail->next; p; p=next) {
			next = p->next;
			munmap(p->map, p->map_len);
			free(p->deps);
			free(p);
		}
		tail = orig_tail;
		tail->next = 0;
		p = 0;
		errflag = 1;
		goto end;
	} else p = load_library(file);

	if (!p) {
		snprintf(errbuf, sizeof errbuf,
			"Error loading shared library %s: %m", file);
		errflag = 1;
		goto end;
	}

	/* First load handling */
	if (!p->deps) {
		load_deps(p);
		if (p->deps) for (i=0; p->deps[i]; i++)
			if (!p->deps[i]->global)
				p->deps[i]->global = -1;
		if (!p->global) p->global = -1;
		reloc_all(p);
		if (p->deps) for (i=0; p->deps[i]; i++)
			if (p->deps[i]->global < 0)
				p->deps[i]->global = 0;
		if (p->global < 0) p->global = 0;
	}

	if (mode & RTLD_GLOBAL) {
		if (p->deps) for (i=0; p->deps[i]; i++)
			p->deps[i]->global = 1;
		p->global = 1;
	}

	if (ssp_used) __init_ssp(auxv);

	_dl_debug_state();

	do_init_fini(tail);
end:
	pthread_rwlock_unlock(&lock);
	pthread_setcancelstate(cs, 0);
	return p;
}

static void *do_dlsym(struct dso *p, const char *s, void *ra)
{
	size_t i;
	uint32_t h = 0, gh = 0;
	Sym *sym;
	if (p == RTLD_NEXT) {
		for (p=head; p && (unsigned char *)ra-p->map>p->map_len; p=p->next);
		if (!p) p=head;
		void *res = find_sym(p->next, s, 0);
		if (!res) goto failed;
		return res;
	}
	if (p == head || p == RTLD_DEFAULT) {
		void *res = find_sym(head, s, 0);
		if (!res) goto failed;
		return res;
	}
	if (p->ghashtab) {
		gh = gnu_hash(s);
		sym = gnu_lookup(s, gh, p);
	} else {
		h = sysv_hash(s);
		sym = sysv_lookup(s, h, p);
	}
	if (sym && sym->st_value && (1<<(sym->st_info&0xf) & OK_TYPES))
		return p->base + sym->st_value;
	if (p->deps) for (i=0; p->deps[i]; i++) {
		if (p->deps[i]->ghashtab) {
			if (!gh) gh = gnu_hash(s);
			sym = gnu_lookup(s, gh, p->deps[i]);
		} else {
			if (!h) h = sysv_hash(s);
			sym = sysv_lookup(s, h, p->deps[i]);
		}
		if (sym && sym->st_value && (1<<(sym->st_info&0xf) & OK_TYPES))
			return p->deps[i]->base + sym->st_value;
	}
failed:
	errflag = 1;
	snprintf(errbuf, sizeof errbuf, "Symbol not found: %s", s);
	return 0;
}

int __dladdr(void *addr, Dl_info *info)
{
	struct dso *p;
	Sym *sym;
	uint32_t nsym;
	char *strings;
	size_t i;
	void *best = 0;
	char *bestname;

	pthread_rwlock_rdlock(&lock);
	for (p=head; p && (unsigned char *)addr-p->map>p->map_len; p=p->next);
	pthread_rwlock_unlock(&lock);

	if (!p) return 0;

	sym = p->syms;
	strings = p->strings;
	if (p->hashtab) {
		nsym = p->hashtab[1];
	} else {
		uint32_t *buckets;
		uint32_t *hashval;
		buckets = p->ghashtab + 4 + (p->ghashtab[2]*sizeof(size_t)/4);
		sym += p->ghashtab[1];
		for (i = 0; i < p->ghashtab[0]; i++) {
			if (buckets[i] > nsym)
				nsym = buckets[i];
		}
		if (nsym) {
			nsym -= p->ghashtab[1];
			hashval = buckets + p->ghashtab[0] + nsym;
			do nsym++;
			while (!(*hashval++ & 1));
		}
	}

	for (; nsym; nsym--, sym++) {
		if (sym->st_shndx && sym->st_value
		 && (1<<(sym->st_info&0xf) & OK_TYPES)
		 && (1<<(sym->st_info>>4) & OK_BINDS)) {
			void *symaddr = p->base + sym->st_value;
			if (symaddr > addr || symaddr < best)
				continue;
			best = symaddr;
			bestname = strings + sym->st_name;
			if (addr == symaddr)
				break;
		}
	}

	if (!best) return 0;

	info->dli_fname = p->name;
	info->dli_fbase = p->base;
	info->dli_sname = bestname;
	info->dli_saddr = best;

	return 1;
}

void *__dlsym(void *restrict p, const char *restrict s, void *restrict ra)
{
	void *res;
	pthread_rwlock_rdlock(&lock);
	res = do_dlsym(p, s, ra);
	pthread_rwlock_unlock(&lock);
	return res;
}
#else
void *dlopen(const char *file, int mode)
{
	return 0;
}
void *__dlsym(void *restrict p, const char *restrict s, void *restrict ra)
{
	return 0;
}
int __dladdr (void *addr, Dl_info *info)
{
	return 0;
}
#endif

char *dlerror()
{
	if (!errflag) return 0;
	errflag = 0;
	return errbuf;
}

int dlclose(void *p)
{
	return 0;
}
