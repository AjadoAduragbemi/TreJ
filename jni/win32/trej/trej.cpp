// trej.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "trej.h"

#if defined _M_IX86
#pragma comment(lib, "..\\lib\\x86\\tre.lib")
#elif defined _M_X64
#pragma comment(lib, "..\\lib\\x64\\tre.lib")
#endif

regmatch_t* create_regmatch(size_t nmatch) {
	regmatch_t* pmatch = new regmatch_t[nmatch];
	for(int i = 0; i < nmatch; i++) {
		pmatch[i].rm_so = -1;
		pmatch[i].rm_eo = -1;
	}
	return pmatch;
}

void setUpMatchResult(trej_result_t* p_trej_match, regmatch_t* pmatch, size_t nmatch) {
	if(p_trej_match != nullptr) {

		p_trej_match->nmatch = 0;
		p_trej_match->matchArray = new trej_match_t[nmatch];

		for(int i = 0; i < nmatch; i++) {
			regmatch_t* match = &pmatch[i];
			if(match->rm_so == -1 && match->rm_eo == -1) {
				break;
			}
			p_trej_match->matchArray[i].start_offset = match->rm_so;
			p_trej_match->matchArray[i].end_offset = match->rm_eo;
			p_trej_match->nmatch++;
		}
	}
}

template<> 
int matchNotApprox<wchar_t>(const regex_t* preg, const wchar_t* string, size_t len, trej_result_t* p_trej_match, int eflags) {
	size_t nmatch = preg->re_nsub + 1;
	regmatch_t* pmatch = create_regmatch(nmatch);

	int error_value = 1;
#if defined(TRE_WCHAR) && defined(HAVE_WCHAR_H)
	if(len > 0) {
		error_value = tre_regwnexec(preg, string, len, nmatch, pmatch, eflags);
	} else {
		error_value = tre_regwexec(preg, string, nmatch, pmatch, eflags);
	}
#endif

	setUpMatchResult(p_trej_match, pmatch, nmatch);

	p_trej_match->error_message = new char[TREJ_ERR_MSG_LEN];
	tre_regerror(error_value, preg, p_trej_match->error_message, TREJ_ERR_MSG_LEN);

	delete[] pmatch;
	pmatch = nullptr;	// doesn't do no harm
	return error_value;
}

template <>
int matchNotApprox<char>(const regex_t* preg, const char* string, size_t len, trej_result_t* p_trej_match, int eflags) {
	size_t nmatch = preg->re_nsub + 1;
	regmatch_t* pmatch = create_regmatch(nmatch);

	int error_value;
	if(len > 0) {
		error_value = tre_regnexec(preg, string, len, nmatch, pmatch, eflags);
	} else {
		error_value = tre_regexec(preg, string, nmatch, pmatch, eflags);
	}

	setUpMatchResult(p_trej_match, pmatch, nmatch);

	p_trej_match->error_message = new char[TREJ_ERR_MSG_LEN];
	tre_regerror(error_value, preg, p_trej_match->error_message, TREJ_ERR_MSG_LEN);

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
	p_amatch->pmatch = create_regmatch(nmatch);

	int error_value;
	if(len > 0) {
		error_value = tre_reganexec(preg, string, len, p_amatch, aparams, eflags);
	} else {
		error_value = tre_regaexec(preg, string, p_amatch, aparams, eflags);
		len = strnlen_s(string, TREJ_STR_MAX);
	}

	setUpMatchResult(p_trej_match, p_amatch->pmatch, nmatch);

	p_trej_match->error_message = new char[TREJ_ERR_MSG_LEN];
	tre_regerror(error_value, preg, p_trej_match->error_message, TREJ_ERR_MSG_LEN);

	if(error_value == REG_OK) {
		p_trej_match->match_cost = p_amatch->cost;
		p_trej_match->insert_count = p_amatch->num_ins;
		p_trej_match->delete_count = p_amatch->num_del;
		p_trej_match->substitution_count = p_amatch->num_subst;
	}

	delete[] p_amatch->pmatch;
	p_amatch->pmatch = nullptr;
	delete p_amatch;
	p_amatch = nullptr;	// doesn't do no harm

	return error_value;
}

template<> 
int matchApprox<wchar_t>(const regex_t* preg, const wchar_t* string, size_t len, trej_result_t* p_trej_match, int eflags) {
	size_t nmatch = preg->re_nsub + 1;
	regaparams_t aparams;
	regamatch_t* p_amatch = new regamatch_t;

	tre_regaparams_default(&aparams);

	p_amatch->nmatch = nmatch;
	p_amatch->pmatch = create_regmatch(nmatch);

	int error_value = 1;
#if defined(TRE_WCHAR) && defined(HAVE_WCHAR_H)
	if(len > 0) {
		error_value = tre_regawnexec(preg, string, len, p_amatch, aparams, eflags);
	} else {
		error_value = tre_regawexec(preg, string, p_amatch, aparams, eflags);
	}
#endif

	setUpMatchResult(p_trej_match, p_amatch->pmatch, nmatch);

	p_trej_match->error_message = new char[TREJ_ERR_MSG_LEN];
	tre_regerror(error_value, preg, p_trej_match->error_message, TREJ_ERR_MSG_LEN);

	if(error_value == REG_OK) {
		p_trej_match->match_cost = p_amatch->cost;
		p_trej_match->insert_count = p_amatch->num_ins;
		p_trej_match->delete_count = p_amatch->num_del;
		p_trej_match->substitution_count = p_amatch->num_subst;
	}

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

