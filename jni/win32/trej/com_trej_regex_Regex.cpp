
#include "stdafx.h"
#include "com_trej_regex_Regex.h"
#include "compile.h"
#include "exec.h"


JNIEXPORT jint JNICALL Java_com_trej_regex_Regex_compile
	(JNIEnv *env, jobject object) {
		return trej_regex_compile(env, object);
}

JNIEXPORT jobject JNICALL Java_com_trej_regex_Regex_exec
	(JNIEnv *env, jobject object) {
		return trej_regex_exec(env, object);
}