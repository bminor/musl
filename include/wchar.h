#ifndef _WCHAR_H
#define _WCHAR_H

#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_FILE
#define __NEED_va_list
#define __NEED_size_t
#define __NEED_wchar_t
#define __NEED_wint_t
#define __NEED_wctype_t

#include <bits/alltypes.h>

#undef NULL
#define NULL ((void*)0)

#undef WCHAR_MIN
#undef WCHAR_MAX
#define WCHAR_MIN (-1-0x7fffffff)
#define WCHAR_MAX (0x7fffffff)

#undef WEOF
#define WEOF (-1)


typedef struct
{
	unsigned __opaque1, __opaque2;
} mbstate_t;

wchar_t *wcscpy (wchar_t *, const wchar_t *);
wchar_t *wcsncpy (wchar_t *, const wchar_t *, size_t);

wchar_t *wcscat (wchar_t *, const wchar_t *);
wchar_t *wcsncat (wchar_t *, const wchar_t *, size_t);

int wcscmp (const wchar_t *, const wchar_t *);
int wcsncmp (const wchar_t *, const wchar_t *, size_t);

size_t wcsxfrm (wchar_t *, const wchar_t *, size_t n);

wchar_t *wcschr (const wchar_t *, wchar_t);
wchar_t *wcsrchr (const wchar_t *, wchar_t);

size_t wcscspn (const wchar_t *, const wchar_t *);
size_t wcsspn (const wchar_t *, const wchar_t *);
wchar_t *wcspbrk (const wchar_t *, const wchar_t *);

wchar_t *wcstok (wchar_t *, const wchar_t *, wchar_t **);

size_t wcslen (const wchar_t *);

wchar_t *wcsstr (const wchar_t *, const wchar_t *);
wchar_t *wcswcs (const wchar_t *, const wchar_t *);

wchar_t *wmemchr (const wchar_t *, wchar_t, size_t);
int wmemcmp (const wchar_t *, const wchar_t *, size_t);
wchar_t *wmemcpy (wchar_t *, const wchar_t *, size_t);
wchar_t *wmemmove (wchar_t *, const wchar_t *, size_t);
wchar_t *wmemset (wchar_t *, wchar_t, size_t);

wint_t btowc (int);
int wctob (wint_t);

int mbsinit (const mbstate_t *);
size_t mbrtowc (wchar_t *, const char *, size_t, mbstate_t *);
size_t wcrtomb (char *, wchar_t, mbstate_t *);

size_t mbrlen (const char *, size_t, mbstate_t *);

size_t mbsrtowcs (wchar_t *, const char **, size_t, mbstate_t *);
size_t wcsrtombs (char *, const wchar_t **, size_t, mbstate_t *);

int wcwidth (wchar_t);
int wcswidth (const wchar_t *, size_t);

float wcstof (const wchar_t *, wchar_t **);
double wcstod (const wchar_t *, wchar_t **);
long double wcstold (const wchar_t *, wchar_t **);

long wcstol (const wchar_t *, wchar_t **, int);
unsigned long wcstoul (const wchar_t *, wchar_t **, int);

long long wcstoll (const wchar_t *, wchar_t **, int);
unsigned long long wcstoull (const wchar_t *, wchar_t **, int);



int fwide (FILE *, int);


int wprintf (const wchar_t *, ...);
int fwprintf (FILE *, const wchar_t *, ...);
int swprintf (wchar_t *, const wchar_t *, ...);

int vwprintf (const wchar_t *, va_list);
int vfwprintf (FILE *, const wchar_t *, va_list);
int vswprintf (wchar_t *, const wchar_t *, va_list);

int wscanf (const wchar_t *, ...);
int fwscanf (FILE *, const wchar_t *, ...);
int swscanf (const wchar_t *, const wchar_t *, ...);

int vwscanf (const wchar_t *, va_list);
int vfwscanf (FILE *, const wchar_t *, va_list);
int vswscanf (const wchar_t *, const wchar_t *, va_list);

wint_t fgetwc (FILE *);
wint_t getwc (FILE *);
wint_t getwchar (void);

wint_t fputwc (wchar_t, FILE *);
wint_t putwc (wchar_t, FILE *);
wint_t putwchar (wchar_t);

wchar_t *fgetws (wchar_t *, int, FILE *);
int fputws (const wchar_t *, FILE *);

wint_t ungetwc (wint_t, FILE *);

struct tm;
size_t wcsftime (wchar_t *, size_t, const wchar_t *, const struct tm *);

#undef iswdigit

int       iswalnum(wint_t);
int       iswalpha(wint_t);
int       iswblank(wint_t);
int       iswcntrl(wint_t);
int       iswdigit(wint_t);
int       iswgraph(wint_t);
int       iswlower(wint_t);
int       iswprint(wint_t);
int       iswpunct(wint_t);
int       iswspace(wint_t);
int       iswupper(wint_t);
int       iswxdigit(wint_t);
int       iswctype(wint_t, wctype_t);
wint_t    towlower(wint_t);
wint_t    towupper(wint_t);
wctype_t  wctype(const char *);

#define iswdigit(a) ((unsigned)(a)-'0' < 10)

#ifdef __cplusplus
}
#endif

#endif
