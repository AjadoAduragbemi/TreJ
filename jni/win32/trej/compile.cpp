
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
			error_value = tre_regcomp(preg, regex, REG_EXTENDED|cflags);
			env->ReleaseStringUTFChars(pattern, regex);
			if(error_value == REG_OK) {
				env->SetLongField(object, preg_id, reinterpret_cast<std::uintptr_t>(preg));
			}
			char *error_message = new char[MAXBYTE];
			tre_regerror(error_value, preg, error_message, MAXBYTE);
			env->SetObjectField(object, error_message_id, env->NewStringUTF(error_message));
			delete[] error_message;
	}

	return error_value;
}