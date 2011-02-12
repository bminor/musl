#include <locale.h>
#include <string.h>
#include <stdlib.h>

struct lconv *localeconv(void)
{
	static struct lconv *posix_lconv;
	if (posix_lconv) return posix_lconv;
	posix_lconv = malloc(sizeof *posix_lconv);
	memset(posix_lconv, -1, sizeof *posix_lconv);
	posix_lconv->decimal_point = ".";
	posix_lconv->thousands_sep = "";
	posix_lconv->grouping = "\xff";
	posix_lconv->int_curr_symbol = ""; //"\xc2\xa4";
	posix_lconv->currency_symbol = "";
	posix_lconv->mon_decimal_point = "";
	posix_lconv->mon_thousands_sep = "";
	posix_lconv->mon_grouping = "\xff";
	posix_lconv->positive_sign = ""; // "+";
	posix_lconv->negative_sign = ""; // "-";
	return posix_lconv;
}
