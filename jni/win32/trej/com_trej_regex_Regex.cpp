
#include "stdafx.h"
#include "com_trej_regex_Regex.h"
#include "match.h"

JNIEXPORT jobject JNICALL Java_com_trej_regex_Regex_match
	(JNIEnv *env, jobject object) {
		return trej_regex_match(env, object);
}