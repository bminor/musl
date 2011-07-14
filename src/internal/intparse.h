#include <stdint.h>
#include <stddef.h>

struct intparse {
	uintmax_t val;
	unsigned small;
	size_t cnt;
	char neg, base, state, err;
};

int __intparse(struct intparse *, const void *, size_t);
