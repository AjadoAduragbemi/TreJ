
#include <jni.h>

jobject make_match(JNIEnv *, jclass, jint, jint);
jobject make_match_result(JNIEnv *, jclass, jobject,  jobjectArray);
jobject trej_regex_match(JNIEnv *env, jobject object);