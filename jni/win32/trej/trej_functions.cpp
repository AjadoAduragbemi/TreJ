#include "stdafx.h"

bool hasWideChar(const char* string) {
	bool has_wide_char = false;
#if defined(TRE_WCHAR) && defined(HAVE_WCHAR_H)
	while(*string != '\0') {
		if((has_wide_char = (*string & 0x80) != 0)) {
			break;
		}
		*string++;
	}
#endif
	return has_wide_char;
}
