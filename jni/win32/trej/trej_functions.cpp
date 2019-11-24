#include "stdafx.h"

bool hasWideChar(const char* string) {
	bool has_wide_char;
	while(*string != '\0') {
		if((has_wide_char = (*string & 0x80) != 0)) {
			break;
		}
		*string++;
	}
	return has_wide_char;
}
