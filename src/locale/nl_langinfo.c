#include <langinfo.h>

// FIXME: other items

char *nl_langinfo(nl_item item)
{
	switch (item) {
	case CODESET:
		return "UTF-8";
	default:
		return "";
	}
}
