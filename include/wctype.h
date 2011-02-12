#ifndef _WCTYPE_H
#define _WCTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_wint_t
#define __NEED_wctrans_t
#define __NEED_wctype_t

#include <bits/alltypes.h>

#undef WEOF
#define WEOF (-1)

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
wint_t    towctrans(wint_t, wctrans_t);
wint_t    towlower(wint_t);
wint_t    towupper(wint_t);
wctrans_t wctrans(const char *);
wctype_t  wctype(const char *);

#define iswdigit(a) ((unsigned)((a)-L'0') < 10)

#ifdef __cplusplus
}
#endif

#endif
