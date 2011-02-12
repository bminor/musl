/* 
 * This code was written by Rich Felker in 2010; no copyright is claimed.
 * This code is in the public domain. Attribution is appreciated but
 * unnecessary.
 */

#include <stdlib.h>
#include <inttypes.h>
#include <wchar.h>
#include <errno.h>

#include "internal.h"

size_t wcstombs(char *s, const wchar_t *ws, size_t n)
{
	return wcsrtombs(s, &ws, n, 0);
}
