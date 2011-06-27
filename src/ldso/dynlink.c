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
#include <dlfcn.h>

#include "reloc.h"

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

struct dso
{
	struct dso *next, *prev;
	int refcnt;
	size_t *dynv;
	Sym *syms;
	uint32_t *hashtab;
	char *strings;
	unsigned char *base;
	unsigned char *map;
	size_t map_len;
	dev_t dev;
	ino_t ino;
	int global;
	int relocated;
	struct dso **deps;
	char *name;
	char buf[];
};

static struct dso *head, *tail, *libc;
static char *env_path, *sys_path;
static int rtld_used;
static int runtime;
static jmp_buf rtld_fail;
static pthread_rwlock_t lock;

#define AUX_CNT 15
#define DYN_CNT 34

static void decode_vec(size_t *v, size_t *a, size_t cnt)
{
	memset(a, 0, cnt*sizeof(size_t));
	for (; v[0]; v+=2) if (v[0]<cnt) {
		a[0] |= 1ULL<<v[0];
		a[v[0]] = v[1];
	}
}

static uint32_t hash(const char *s)
{
	uint_fast32_t h = 0;
	while (*s) {
		h = 16*h + *s++;
		h ^= h>>24 & 0xf0;
	}
	return h & 0xfffffff;
}

static Sym *lookup(const char *s, uint32_t h, Sym *syms, uint32_t *hashtab, char *strings)
{
	size_t i;
	for (i=hashtab[2+h%hashtab[0]]; i; i=hashtab[2+hashtab[0]+i]) {
		if (!strcmp(s, strings+syms[i].st_name))
			return syms+i;
	}
	return 0;
}

#define OK_TYPES (1<<STT_NOTYPE | 1<<STT_OBJECT | 1<<STT_FUNC | 1<<STT_COMMON)

static void *find_sym(struct dso *dso, const char *s, int need_def)
{
	uint32_t h = hash(s);
	if (h==0x6b366be && !strcmp(s, "dlopen")) rtld_used = 1;
	if (h==0x6b3afd && !strcmp(s, "dlsym")) rtld_used = 1;
	for (; dso; dso=dso->next) {
		Sym *sym;
		if (!dso->global) continue;
		sym = lookup(s, h, dso->syms, dso->hashtab, dso->strings);
		if (sym && (!need_def || sym->st_shndx) && sym->st_value
		 && (1<<(sym->st_info&0xf) & OK_TYPES))
			return dso->base + sym->st_value;
	}
	return 0;
}

static void do_relocs(unsigned char *base, size_t *rel, size_t rel_size, size_t stride, Sym *syms, char *strings, struct dso *dso)
{
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
			ctx = IS_COPY(type) ? dso->next : dso;
			sym_val = (size_t)find_sym(ctx, name, IS_PLT(type));
			if (!sym_val && sym->st_info>>4 != STB_WEAK) {
				if (runtime) longjmp(rtld_fail, 1);
				dprintf(2, "%s: symbol not found\n", name);
				_exit(127);
			}
			sym_size = sym->st_size;
		}
		do_single_reloc(reloc_addr, type, sym_val, sym_size, base, rel[2]);
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
	map = mmap(0, map_len, prot, MAP_PRIVATE, fd, off_start);
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
		if (mmap(base+this_min, this_max-this_min, prot, MAP_PRIVATE|MAP_FIXED, fd, off_start) == MAP_FAILED) {
			munmap(map, map_len);
			return 0;
		}
		if (ph->p_memsz > ph->p_filesz) {
			size_t brk = (size_t)base+ph->p_vaddr+ph->p_filesz;
			size_t pgbrk = brk+PAGE_SIZE-1 & -PAGE_SIZE;
			memset((void *)brk, 0, pgbrk-brk & PAGE_SIZE-1);
			if (pgbrk-(size_t)base < this_max && mmap((void *)pgbrk, (size_t)base+this_max-pgbrk, prot, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) == MAP_FAILED) {
				munmap(map, map_len);
				return 0;
			}
		}
	}
	*lenp = map_len;
	*basep = base;
	*dynp = dyn;
	return map;
}

static int path_open(const char *name, const char *search)
{
	char buf[2*NAME_MAX+2];
	const char *s=search, *z;
	int l, fd;
	for (;;) {
		while (*s==':') s++;
		if (!*s) return -1;
		z = strchr(s, ':');
		l = z ? z-s : strlen(s);
		snprintf(buf, sizeof buf, "%.*s/%s", l, s, name);
		if ((fd = open(buf, O_RDONLY))>=0) return fd;
		s += l;
	}
}

static struct dso *load_library(const char *name)
{
	unsigned char *base, *map;
	size_t dyno, map_len;
	struct dso *p;
	size_t dyn[DYN_CNT] = {0};
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
					tail = libc;
				}
				return libc;
			}
		}
	}
	/* Search for the name to see if it's already loaded */
	for (p=head->next; p; p=p->next) {
		if (!strcmp(p->name, name)) {
			p->refcnt++;
			return p;
		}
	}
	if (strchr(name, '/')) {
		fd = open(name, O_RDONLY);
	} else {
		if (strlen(name) > NAME_MAX) return 0;
		fd = -1;
		if (env_path) fd = path_open(name, env_path);
		if (fd < 0) {
			if (!sys_path) {
				FILE *f = fopen(ETC_LDSO_PATH, "r");
				if (f) {
					if (getline(&sys_path, (size_t[1]){0}, f) > 0)
						sys_path[strlen(sys_path)-1]=0;
					fclose(f);
				}
			}
			if (sys_path) fd = path_open(name, sys_path);
			else fd = path_open(name, "/lib:/usr/local/lib:/usr/lib");
		}
	}
	if (fd < 0) return 0;
	if (fstat(fd, &st) < 0) {
		close(fd);
		return 0;
	}
	for (p=head->next; p; p=p->next) {
		if (p->dev == st.st_dev && p->ino == st.st_ino) {
			close(fd);
			p->refcnt++;
			return p;
		}
	}
	map = map_library(fd, &map_len, &base, &dyno);
	close(fd);
	if (!map) return 0;
	p = calloc(1, sizeof *p + strlen(name) + 1);
	if (!p) {
		munmap(map, map_len);
		return 0;
	}

	p->map = map;
	p->map_len = map_len;
	p->base = base;
	p->dynv = (void *)(base + dyno);
	decode_vec(p->dynv, dyn, DYN_CNT);

	p->syms = (void *)(base + dyn[DT_SYMTAB]);
	p->hashtab = (void *)(base + dyn[DT_HASH]);
	p->strings = (void *)(base + dyn[DT_STRTAB]);
	p->dev = st.st_dev;
	p->ino = st.st_ino;
	p->refcnt = 1;
	p->name = p->buf;
	strcpy(p->name, name);

	tail->next = p;
	p->prev = tail;
	tail = p;

	return p;
}

static void load_deps(struct dso *p)
{
	size_t i, ndeps=0;
	struct dso ***deps = &p->deps, **tmp, *dep;
	for (; p; p=p->next) {
		for (i=0; p->dynv[i]; i+=2) {
			if (p->dynv[i] != DT_NEEDED) continue;
			dep = load_library(p->strings + p->dynv[i+1]);
			if (!dep) {
				if (runtime) longjmp(rtld_fail, 1);
				dprintf(2, "%s: %m (needed by %s)\n",
					p->strings + p->dynv[i+1], p->name);
				_exit(127);
			}
			if (runtime) {
				tmp = realloc(*deps, sizeof(*tmp)*(ndeps+2));
				if (!tmp) longjmp(rtld_fail, 1);
				tmp[ndeps++] = dep;
				tmp[ndeps] = 0;
				*deps = tmp;
			}
		}
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
		do_relocs(p->base, (void *)(p->base+dyn[DT_JMPREL]), dyn[DT_PLTRELSZ],
			2+(dyn[DT_PLTREL]==DT_RELA), p->syms, p->strings, head);
		do_relocs(p->base, (void *)(p->base+dyn[DT_REL]), dyn[DT_RELSZ],
			2, p->syms, p->strings, head);
		do_relocs(p->base, (void *)(p->base+dyn[DT_RELA]), dyn[DT_RELASZ],
			3, p->syms, p->strings, head);
		p->relocated = 1;
	}
}

static void free_all(struct dso *p)
{
	struct dso *n;
	while (p) {
		n = p->next;
		if (p->map) free(p);
		p = n;
	}
}

void *__dynlink(int argc, char **argv, size_t *got)
{
	size_t *auxv, aux[AUX_CNT] = {0};
	size_t app_dyn[DYN_CNT] = {0};
	size_t lib_dyn[DYN_CNT] = {0};
	size_t i;
	Phdr *phdr;
	struct dso lib, app;

	/* Find aux vector just past environ[] */
	for (i=argc+1; argv[i]; i++)
		if (!memcmp(argv[i], "LD_LIBRARY_PATH=", 16))
			env_path = argv[i]+16;
	auxv = (void *)(argv+i+1);

	decode_vec(auxv, aux, AUX_CNT);

	/* Only trust user/env if kernel says we're not suid/sgid */
	if ((aux[0]&0x7800)!=0x7800 || aux[AT_UID]!=aux[AT_EUID]
	  || aux[AT_GID]!=aux[AT_EGID]) {
		env_path = 0;
	}

	/* Relocate ldso's DYNAMIC pointer and load vector */
	decode_vec((void *)(got[0] += aux[AT_BASE]), lib_dyn, DYN_CNT);

	/* Find the program image's DYNAMIC section and decode it */
	phdr = (void *)aux[AT_PHDR];
	for (i=aux[AT_PHNUM]; i--; phdr=(void *)((char *)phdr + aux[AT_PHENT])) {
		if (phdr->p_type == PT_DYNAMIC) {
			decode_vec((void *)phdr->p_vaddr, app_dyn, DYN_CNT);
			break;
		}
	}

	app = (struct dso){
		.base = 0,
		.strings = (void *)(app_dyn[DT_STRTAB]),
		.hashtab = (void *)(app_dyn[DT_HASH]),
		.syms = (void *)(app_dyn[DT_SYMTAB]),
		.dynv = (void *)(phdr->p_vaddr),
		.name = argv[0],
		.global = 1,
		.next = &lib
	};

	lib = (struct dso){
		.base = (void *)aux[AT_BASE],
		.strings = (void *)(aux[AT_BASE]+lib_dyn[DT_STRTAB]),
		.hashtab = (void *)(aux[AT_BASE]+lib_dyn[DT_HASH]),
		.syms = (void *)(aux[AT_BASE]+lib_dyn[DT_SYMTAB]),
		.dynv = (void *)(got[0]),
		.name = "libc.so",
		.global = 1,
		.relocated = 1
	};

	/* Relocate the dynamic linker/libc */
	do_relocs((void *)aux[AT_BASE], (void *)(aux[AT_BASE]+lib_dyn[DT_REL]),
		lib_dyn[DT_RELSZ], 2, lib.syms, lib.strings, &app);
	do_relocs((void *)aux[AT_BASE], (void *)(aux[AT_BASE]+lib_dyn[DT_RELA]),
		lib_dyn[DT_RELASZ], 3, lib.syms, lib.strings, &app);

	/* At this point the standard library is fully functional */

	head = tail = &app;
	libc = &lib;
	app.next = 0;
	load_deps(head);

	make_global(head);
	reloc_all(head);

	if (rtld_used) {
		runtime = 1;
		head->next->prev = malloc(sizeof *head);
		*head->next->prev = *head;
		head = head->next->prev;
		libc->prev->next = malloc(sizeof *libc);
		*libc->prev->next = *libc;
		libc = libc->prev->next;
		if (libc->next) libc->next->prev = libc;
	} else {
		free_all(head);
		free(sys_path);
	}

	errno = 0;
	return (void *)aux[AT_ENTRY];
}

void *dlopen(const char *file, int mode)
{
	struct dso *p, *orig_tail = tail, *next;
	size_t i;

	if (!file) return head;

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
		pthread_rwlock_unlock(&lock);
		return 0;
	}

	p = load_library(file);
	if (!p) return 0;

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

	pthread_rwlock_unlock(&lock);

	return p;
}

static void *do_dlsym(struct dso *p, const char *s)
{
	size_t i;
	uint32_t h;
	Sym *sym;
	if (p == head || p == RTLD_DEFAULT)
		return find_sym(head, s, 0);
	h = hash(s);
	sym = lookup(s, h, p->syms, p->hashtab, p->strings);
	if (sym && sym->st_value && (1<<(sym->st_info&0xf) & OK_TYPES))
		return p->base + sym->st_value;
	if (p->deps) for (i=0; p->deps[i]; i++) {
		sym = lookup(s, h, p->deps[i]->syms,
			p->deps[i]->hashtab, p->deps[i]->strings);
		if (sym && sym->st_value && (1<<(sym->st_info&0xf) & OK_TYPES))
			return p->deps[i]->base + sym->st_value;
	}
	return 0;
}

void *dlsym(void *p, const char *s)
{
	void *res;
	pthread_rwlock_rdlock(&lock);
	res = do_dlsym(p, s);
	pthread_rwlock_unlock(&lock);
	return res;
}

char *dlerror()
{
	return "unknown error";
}

int dlclose(void *p)
{
	return 0;
}
