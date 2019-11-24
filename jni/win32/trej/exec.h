
#include "jni.h"
#include "trej_functions.h"

jobject make_match(JNIEnv *, jclass, jint, jint);
jobject make_match_result(JNIEnv *, jclass, jobject,  jobjectArray, jboolean);
jobject trej_regex_exec(JNIEnv *env, jobject object);