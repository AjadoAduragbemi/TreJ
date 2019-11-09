
#include "stdafx.h"
#include "com_trej_regex_Regex.h"
#include "compile.h"
#include "exec.h"
#include "tre/tre.h"


JNIEXPORT jint JNICALL Java_com_trej_regex_Regex_compile
	(JNIEnv *env, jobject object) {
		return trej_regex_compile(env, object);
}

JNIEXPORT jobject JNICALL Java_com_trej_regex_Regex_exec
	(JNIEnv *env, jobject object) {
		return trej_regex_exec(env, object);
}

JNIEXPORT void JNICALL Java_com_trej_regex_Regex_free
	(JNIEnv *env, jobject object) {

		jclass regex_class = env->GetObjectClass(object);
		jfieldID preg_id;
		regex_t *preg;
		if(regex_class != nullptr && (preg_id = env->GetFieldID(regex_class, "preg", "J")) &&
			(preg = reinterpret_cast<regex_t*>(env->GetLongField(object, preg_id))) != nullptr ) {
				tre_regfree(preg);
				env->SetLongField(object, preg_id, 0);
		}
}