// trej.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "trej.h"

#if defined _M_IX86
#pragma comment(lib, "..\\lib\\x86\\tre.lib")
#elif defined _M_X64
#pragma comment(lib, "..\\lib\\x64\\tre.lib")
#endif

#define IS_CHAR 1
#define IS_WCHAR (IS_CHAR << 1)

INT typeof(const char* p) { return IS_CHAR; }
INT typeof(const wchar_t* p) { return IS_WCHAR; }

void setUpMatchResult(trej_result_t* p_trej_match, regmatch_t* pmatch, size_t nmatch, size_t maxlen) {
	if(p_trej_match != nullptr) {

		p_trej_match->nmatch = 0;
		p_trej_match->matchArray = new trej_match_t[nmatch];

		for(int i = 0; i < nmatch; i++) {
			regmatch_t* match = &pmatch[i];
			if(match->rm_so < 0 || match->rm_so >= maxlen || match->rm_eo <= 0 || match->rm_eo > maxlen) {
				break;
			}
			p_trej_match->matchArray[i].start_offset = match->rm_so;
			p_trej_match->matchArray[i].end_offset = match->rm_eo;
			p_trej_match->nmatch++;
		}
	}
}

int matchNotApprox(const regex_t* preg, const char* string, size_t len, trej_result_t* p_trej_match, int eflags) {
	size_t nmatch = preg->re_nsub + 1;
	regmatch_t* pmatch = new regmatch_t[nmatch];

	int error_value;
	if(len > 0) {
		error_value = tre_regnexec(preg, string, len, nmatch, pmatch, eflags);
	} else {
		error_value = tre_regexec(preg, string, nmatch, pmatch, eflags);
		len = strnlen_s(string, 1024);
	}

	setUpMatchResult(p_trej_match, pmatch, nmatch, len);

	p_trej_match->error_message = new char[MAXBYTE];
	tre_regerror(error_value, preg, p_trej_match->error_message, MAXBYTE);

	delete[] pmatch;
	pmatch = nullptr;	// doesn't do no harm
	return error_value;
}

int matchApprox(const regex_t* preg, const char* string, size_t len, trej_result_t* p_trej_match, int eflags) {
	size_t nmatch = preg->re_nsub + 1;
	regaparams_t aparams;
	regamatch_t* p_amatch = new regamatch_t;

	tre_regaparams_default(&aparams);

	p_amatch->nmatch = nmatch;
	p_amatch->pmatch = new regmatch_t[nmatch];

	int error_value;
	if(len > 0) {
		error_value = tre_reganexec(preg, string, len, p_amatch, aparams, eflags);
	} else {
		error_value = tre_regaexec(preg, string, p_amatch, aparams, eflags);
		len = strnlen_s(string, 1024);
	}

	setUpMatchResult(p_trej_match, p_amatch->pmatch, nmatch, len);

	p_trej_match->error_message = new char[MAXBYTE];
	tre_regerror(error_value, preg, p_trej_match->error_message, MAXBYTE);

	delete[] p_amatch->pmatch;
	p_amatch->pmatch = nullptr;
	delete p_amatch;
	p_amatch = nullptr;	// doesn't do no harm

	return error_value;
}



TREJ_API int trejMatch(const char* pattern, const char* string, trej_result_t* p_trej_match, int cflags, int eflags) {
	regex_t preg;
	if(p_trej_match != nullptr) {
		int error_value = tre_regcomp(&preg, pattern, REG_EXTENDED|cflags);
		if(error_value == REG_OK) {
			if(tre_have_approx(&preg) == FALSE) {
				error_value = matchNotApprox(&preg, string, 0, p_trej_match, eflags);
			} else {
				error_value = matchApprox(&preg, string, 0, p_trej_match, eflags);
			}
		}
		tre_regfree(&preg);
		return error_value;
	}
	return -1;
}

TREJ_API int trejnMatch(const char* pattern, const char* string, size_t len, trej_result_t* p_trej_match, int cflags, int eflags) {
	regex_t preg;
	if(p_trej_match != nullptr) {
		int error_value = tre_regncomp(&preg, pattern, len, REG_EXTENDED|cflags);
		if(error_value == REG_OK) {
			if(tre_have_approx(&preg) == FALSE) {
				error_value = matchNotApprox(&preg, string, len, p_trej_match, eflags);
			} else {
				error_value = matchApprox(&preg, string, len, p_trej_match, eflags);
			}
		}
		tre_regfree(&preg);
		return error_value;
	}
	return -1;
}

