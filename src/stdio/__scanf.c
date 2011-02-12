#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <float.h>

#include "__scanf.h"

static int read(rctx_t *r)
{
	if (--r->w < 0) return r->w = -1;
	if (r->u) r->u = 0;
	else r->read(r);
	return r->c;
}

static void unread(rctx_t *r)
{
	//if (r->u || r->w < 0) return;
	if (r->w < 0) return;
	r->w++;
	r->u = 1;
}

#define SIZE_hh -2
#define SIZE_h  -1
#define SIZE_def 0
#define SIZE_l   1
#define SIZE_ll  2
#define SIZE_L   3

static void store_int(void *dest, int size, int neg, unsigned long long i)
{
	if (!dest) return;
	if (neg) i = -i;
	switch (size) {
	case SIZE_hh:
		*(char *)dest = i;
		break;
	case SIZE_h:
		*(short *)dest = i;
		break;
	case SIZE_def:
		*(int *)dest = i;
		break;
	case SIZE_l:
		*(long *)dest = i;
		break;
	case SIZE_ll:
		*(long long *)dest = i;
		break;
	}
}

static void *arg_n(va_list ap, unsigned int n)
{
	void *p;
	unsigned int i;
	va_list ap2;
	va_copy(ap2, ap);
	for (i=n; i>1; i--) va_arg(ap2, void *);
	p = va_arg(ap2, void *);
	va_end(ap2);
	return p;
}

int __scanf(rctx_t *r, const wchar_t *fmt, va_list ap)
{
	int mode=0;
	int width;
	int size;
	const wchar_t *p, *z;
	int c, l, t, m;
	long long dummy;
	char *s;
	wchar_t *wcs;
	mbstate_t st;
	int wide = r->wide;
	void *dest=NULL;
	int invert;
	unsigned long long i=0;
	int neg=0;
	int matches=0;
	long double f;
	int (*is_space)(int) = r->is_space;

	for (p=fmt; *p; ) {
		if (is_space(*p)) {
			do p++; while (is_space(*p));
			do r->w=1; while (is_space(read(r)));
			unread(r);
			continue;
		} else if (*p != '%' || p[1] == '%') {
			if (*p == '%') p++;
			r->w = 1;
			if (*p++ != read(r))
				goto match_fail;
			continue;
		}
		p++;
		if (mode != 1) {
			for (z=p; isdigit(*z); z++);
			if (*z != '$' && *z != '*') {
				if (mode == 0) mode = 1;
				else goto fmt_fail;
			} else if (*z != '*') {
				int pos = 0;
				mode = 2;
				for (; p<z; p++) {
					pos = 10*pos + *p - '0';
				}
				p++;
				if (!pos) goto fmt_fail;
				dest = arg_n(ap, pos);
			}
		}
		if (*p == '*') {
			dest = NULL;
			p++;
		} else if (mode == 1) {
			dest = va_arg(ap, void *);
		}
		
		if (!*p) goto fmt_fail;

		width = 0;
		for (; isdigit(*p); p++) {
			width = 10*width + *p - '0';
		}

		size = 0;
		switch (*p++) {
		case 0:
			goto fmt_fail;
		case 'h':
			if (*p == 'h') p++, size = SIZE_hh;
			else size = SIZE_h;
			break;
		case 'l':
			if (*p == 'l') p++, size = SIZE_ll;
			else size = SIZE_l;
			break;
		case 'j':
			size = SIZE_ll;
			break;
		case 'z':
		case 't':
			size = SIZE_l;
			break;
		case 'L':
			size = SIZE_L;
			break;
		case 'd': case 'i': case 'o': case 'u': case 'x':
		case 'a': case 'e': case 'f': case 'g':
		case 'A': case 'E': case 'F': case 'G': case 'X':
		case 's': case 'c': case '[':
		case 'S': case 'C':
		case 'p': case 'n':
			p--;
			break;
		default:
			goto fmt_fail;
		}

		t = *p++;

		switch (t) {
		case 'C':
		case 'c':
			if (width < 1) width = 1;
		case 's':
			if (size == SIZE_l) t &= ~0x20;
		case 'd': case 'i': case 'o': case 'u': case 'x':
		case 'a': case 'e': case 'f': case 'g':
		case 'A': case 'E': case 'F': case 'G': case 'X':
		case '[': case 'S':
		case 'p': case 'n':
			if (width < 1) width = INT_MAX;
			break;
		default:
			goto fmt_fail;
		}

		r->w = width;

		if (t != 'n') {
			if (read(r) < 0) goto input_fail;
			unread(r);
		}

		switch (t) {
		case 'n':
			store_int(dest, size, 0, r->l - r->u);
			/* do not increment match count, etc! */
			continue;
		case 'C':
			wcs = dest ? dest : (void *)&dummy;
			st = (mbstate_t){ 0 };
			while ((c=read(r)) >= 0) {
				if (wide) {
					if (dest) *wcs++ = c;
				} else {
					char ch = c;
					switch (mbrtowc(wcs, &ch, 1, &st)) {
					case -1:
						goto enc_fail;
					case -2:
						break;
					default:
						if (dest) wcs++;
					}
				}
			}
			if (r->w > 0) goto match_fail;
			break;
		case 'c':
			s = dest ? dest : (void *)&dummy;
			while ((c=read(r)) >= 0) {
				if (wide) {
					if ((l=wctomb(s, c)) < 0)
						goto enc_fail;
					if (dest) s += l;
				} else {
					if (dest) *s++ = c;
				}
			}
			if (r->w > 0) goto match_fail;
			break;
		case '[':
			wcs = dest ? dest : (void *)&dummy;
			s = dest ? dest : (void *)&dummy;
			if (!wide && size == SIZE_l) st = (mbstate_t){ 0 };

			if (*p == '^') p++, invert = 1;
			else invert = 0;

			if (wide) {
				for (m=0; (c=read(r)) >= 0; m=1) {
					for (z=p; *z && *z != c && (*z != ']' || z==p); z++);
					if (!*z) goto fmt_fail;
					if (*z == c && (*z != ']' || z==p)) {
						if (invert) break;
					} else {
						if (!invert) break;
					}
					if (size == SIZE_l) {
						if (dest) *wcs++ = c;
					} else {
						if ((l=wctomb(s, c)) < 0)
							goto enc_fail;
						if (dest) s += l;
					}
				}
				for (p++; *p && *p != ']'; p++);
				p++;
			} else {
				unsigned char scanset[257];
				memset(scanset, invert, sizeof scanset);
				scanset[0] = 0;
				for (z=p; *z && (*z != ']' || z==p); z++)
					scanset[1+*z] = 1-invert;
				if (!*z) goto fmt_fail;
				p=z+1;
				c=0;
				for (m=0; scanset[(c=read(r))+1]; m=1) {
					if (size == SIZE_l) {
						char ch = c;
						switch (mbrtowc(wcs, &ch, 1, &st)) {
						case -1:
							goto enc_fail;
						case -2:
							break;
						default:
							if (dest) wcs++;
						}
					} else {
						if (dest) *s++ = c;
					}
				}
			}
			if (!m) goto match_fail;
			if (dest) {
				if (size == SIZE_l) *wcs++ = 0;
				else *s++ = 0;
			}
			break;
		default:
			/* read unlimited number of spaces, then reset width */
			do r->w = 1; while (is_space(c = read(r)));
			if (c < 0) goto input_fail;
			unread(r);
			r->w = width;
		}

		switch (t) {
		case 'p':
		case 'X':
			t = 'x';
		case 'd':
		case 'i':
		case 'o':
		case 'u':
		case 'x':
			i = m = neg = 0;
			if ((c=read(r)) == '-') neg=1;
			else if (c != '+') unread(r);
			switch (t) {
			case 'i':
			case 'x':
				if ((c=read(r)) != '0') {
					if (t == 'i') t = 'd';
					unread(r);
					break;
				}
				if (((c=read(r))|0x20) != 'x') {
					if (t == 'i') {
						t = 'o';
						/* lone 0 is valid octal */
						if ((unsigned)(c-'0') >= 8) {
							m = 1;
							goto int_finish;
						}
					}
					unread(r);
					break;
				}
				t = 'x';
			}
		}
		
		switch (t) {
		case 'd':
		case 'u':
			for (m=0; isdigit(c=read(r)); m=1)
				i = 10*i + c-'0';
			goto int_finish;
		case 'o':
			for (m=0; (unsigned)(c=read(r))-'0' < 8; m=1)
				i = (i<<3) + c-'0';
			goto int_finish;
		case 'x':
			for (m=0; ; m=1) {
				if (isdigit(c=read(r))) {
					i = (i<<4) + c-'0';
				} else if ((unsigned)(c|0x20)-'a' < 6) {
					i = (i<<4) + (c|0x20)-'a'+10;
				} else break;
			}
		int_finish:
			if (!m) goto match_fail;
			store_int(dest, size, neg, i);
			break;
		case 'a':
		case 'e':
		case 'f':
		case 'g':
			f = 0.0;
			neg = m = 0;
			if ((c=read(r)) == '-') neg=1;
			else if (c != '+') unread(r);
			/* FIXME: check for INF/NAN strings here */
			if (read(r)=='0' && (m=1, (read(r)|0x20) == 'x'))
				goto hexfloat;
			else unread(r);
			for (; isdigit(c=read(r)); m=1)
				f = 10.0 * f + (c-'0');
			if (c=='.') {
				double mag = 10.0;
				for (; isdigit(c=read(r)); mag*=10.0)
					f += (c-'0')/mag;
			}
			if ((c|0x20)=='e') {
				int ex=0, en=0;
				m = 0;
				if ((c=read(r))=='-') en=1;
				else if (c!='+') unread(r);
				for (; isdigit(c=read(r)); m=1)
					if (ex < LDBL_MAX_10_EXP)
						ex = 10 * ex + (c-'0');
				if (ex > LDBL_MAX_10_EXP)
					f = en ? 0 : INFINITY;
				else {
					if (en) while (ex--) f/=10.0;
					else while (ex--) f*=10.0;
				}
			}
			goto writefloat;
hexfloat:
			m = 0;
			for (; isxdigit(c=read(r)); m=1)
				if (isdigit(c)) f = 16.0*f + (c-'0');
				else f = 16.0*f + ((c|32)-'a'+10);
			if (c=='.') {
				double mag = 1/16.0;
				for (; isxdigit(c=read(r)); mag*=1/16.0)
					if (isdigit(c)) f += (c-'0')*mag;
					else f += ((c|32)-'a'+10)*mag;
			}
			if ((c|0x20)=='p') {
				int ex=0, en=0;
				m = 0;
				if ((c=read(r))=='-') en=1;
				else if (c!='+') unread(r);
				for (; isdigit(c=read(r)); m=1)
					if (ex < LDBL_MAX_EXP)
						ex = 10 * ex + (c-'0');
				if (ex > LDBL_MAX_EXP)
					f = en ? 0 : INFINITY;
				else {
					if (en) while (ex--) f*=0.5;
					else while (ex--) f*=2.0;
				}
			}
writefloat:
			if (!m) goto match_fail;
			if (neg) f *= -1.0;
			if (dest) switch (size) {
			case SIZE_def:
				*(float *)dest = f;
				break;
			case SIZE_l:
				*(double *)dest = f;
				break;
			case SIZE_L:
				*(long double *)dest = f;
				break;
			}
			break;
		case 'S':
			wcs = dest ? dest : (void *)&dummy;
			st = (mbstate_t){ 0 };
			while((c=read(r)) >= 0) {
				if (wide) {
					if (is_space(c)) break;
					if (dest) *wcs++ = c;
				} else {
					char ch = c;
					if (is_space(c)) break;
					switch (mbrtowc(wcs, &ch, 1, &st)) {
					case -1:
						goto enc_fail;
					case -2:
						break;
					default:
						if (dest) wcs++;
					}
				}
			}
			if (dest) *wcs++ = 0;
			break;
		case 's':
			s = dest ? dest : (void *)&dummy;
			while((c=read(r)) >= 0) {
				if (wide) {
					if (is_space(c)) break;
					if ((l=wctomb(s, c)) < 0)
						goto enc_fail;
					if (dest) s += l;
				} else {
					if (is_space(c)) break;
					if (dest) *s++ = c;
				}
			}
			if (dest) *s++ = 0;
			break;
		}

		/* unread will do nothing if field width was exhausted */
		unread(r);
		if (dest) matches++;
	}
	return matches;
enc_fail:
	errno = EILSEQ;
fmt_fail:
input_fail:
	if (!matches) matches--;
match_fail:
	unread(r);
	return matches;
}
