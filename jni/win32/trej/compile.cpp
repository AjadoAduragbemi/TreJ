
#include "stdafx.h"
#include "cstdint"
#include "compile.h"
#include "trej.h"

jint trej_regex_compile(JNIEnv *env, jobject object) {
	jint error_value = -1;
	jclass regex_class = env->GetObjectClass(object);
	jfieldID pattern_id = env->GetFieldID(regex_class, "pattern", "Ljava/lang/String;");
	jfieldID cflags_id = env->GetFieldID(regex_class, "cflags", "I");
	jfieldID preg_id = env->GetFieldID(regex_class, "preg", "J");
	jfieldID error_message_id = env->GetFieldID(regex_class, "errorMessage", "Ljava/lang/String;");
	jstring pattern;
	jint cflags;

	if(regex_class != nullptr &&
		(cflags = env->GetIntField(object, cflags_id)) && 
		(pattern = static_cast<jstring>(env->GetObjectField(object, pattern_id))) ) {
			regex_t *preg = new regex_t;
			auto regex = env->GetStringUTFChars(pattern, nullptr);

#if defined(TRE_WCHAR) && defined(HAVE_WCHAR_H)
			if(hasWideChar(regex)) {
				size_t pattern_len = strnlen(regex, TREJ_STR_MAX) + 1;
				wchar_t* wc_regex = new wchar_t[pattern_len];
				size_t wc_regex_len = 0;
				mbstowcs_s(&wc_regex_len, wc_regex, pattern_len, regex, _TRUNCATE);
				error_value = tre_regwcomp(preg, wc_regex, REG_EXTENDED|cflags);
				delete[] wc_regex;
			} else {
#endif
				error_value = tre_regcomp(preg, regex, REG_EXTENDED|cflags);
#if defined(TRE_WCHAR) && defined(HAVE_WCHAR_H)
			}
#endif

			env->ReleaseStringUTFChars(pattern, regex);

			if(error_value == REG_OK) {
				env->SetLongField(object, preg_id, reinterpret_cast<std::uintptr_t>(preg));
			}

			char *error_message = new char[TREJ_ERR_MSG_LEN];
			tre_regerror(error_value, preg, error_message, TREJ_ERR_MSG_LEN);
			env->SetObjectField(object, error_message_id, env->NewStringUTF(error_message));
			delete[] error_message;
	}

	return error_value;
}