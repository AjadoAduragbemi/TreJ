
#include "stdafx.h"
#include "match.h"
#include "trej.h"
#include "tre/tre.h"

jobject trej_regex_match(JNIEnv *env, jobject object) {

	jobject match_result = nullptr;

	env->MonitorEnter(object);
	jclass regex_class = env->GetObjectClass(object);
	jclass result_class = env->FindClass("Lcom/trej/regex/MatchResult;");
	jclass match_class = env->FindClass("Lcom/trej/regex/Match;");
	trej_result_t* trej_match = new trej_result_t;

	trej_match->error_message = nullptr;
	trej_match->matchArray = nullptr;
	trej_match->nmatch = 0;

	if(regex_class != nullptr && result_class != nullptr && match_class != nullptr) {
		jfieldID pattern_id = env->GetFieldID(regex_class, "pattern", "Ljava/lang/String;");
		jfieldID input_id = env->GetFieldID(regex_class, "input", "Ljava/lang/String;");
		jfieldID error_message_id = env->GetFieldID(regex_class, "errorMessage", "Ljava/lang/String;");
		jfieldID error_value_id = env->GetFieldID(regex_class, "errorValue", "I");
		jfieldID cflags_id = env->GetFieldID(regex_class, "cflags", "I");
		jstring pattern, input;
		jint cflags;

		if( (pattern_id != nullptr) &&
			(input_id != nullptr) &&
			(error_message_id != nullptr) &&
			(error_value_id != nullptr) &&
			(cflags = env->GetIntField(object, cflags_id)) && 
			(pattern = static_cast<jstring>(env->GetObjectField(object, pattern_id))) && 
			(input = static_cast<jstring>(env->GetObjectField(object, input_id))) ) {

				auto regex = env->GetStringUTFChars(pattern, nullptr);
				auto string = env->GetStringUTFChars(input, nullptr);

				jint trej_error_value = trejMatch(regex, string, trej_match, cflags, REG_NOTBOL);

				env->ReleaseStringUTFChars(pattern, regex);
				env->ReleaseStringUTFChars(input, string);

				env->SetIntField(object, error_value_id, trej_error_value);
				env->SetObjectField(object, error_message_id, env->NewStringUTF(trej_match->error_message));
				if(trej_error_value >= REG_OK && trej_error_value <= REG_NOMATCH) {

					jsize len = static_cast<jsize>(trej_match->nmatch);
					jobjectArray match_array = env->NewObjectArray(len, match_class, 0);

					for(int index = 0; index < len; index++) {
						trej_match_t* match = &trej_match->matchArray[index];
						jobject match_object = make_match(env, match_class, match->start_offset, match->end_offset);
						env->SetObjectArrayElement(match_array, index, match_object);
					}

					match_result = make_match_result(env, result_class, object, match_array);

				} else {

				}
		}
	}

	delete trej_match;

	env->MonitorExit(object);

	return match_result;
}

jobject make_match(JNIEnv *env, jclass match_class, jint start_offset, jint end_offset) {
	jmethodID match_constructor;
	jobject match_object;
	jfieldID match_start_offset_id, match_end_offset_id;

	if( (match_constructor = env->GetMethodID(match_class, "<init>", "()V")) &&
		(match_object = env->NewObject(match_class, match_constructor)) &&
		(match_start_offset_id = env->GetFieldID(match_class, "startOffset", "I")) &&
		(match_end_offset_id = env->GetFieldID(match_class, "endOffset", "I")) ) {

			env->SetIntField(match_object, match_start_offset_id, start_offset);
			env->SetIntField(match_object, match_end_offset_id, end_offset);

			return match_object;
	}

	return nullptr;
}

jobject make_match_result(JNIEnv *env, jclass result_class, jobject regexParent,  jobjectArray match_array) {
	jmethodID result_constructor;
	jobject result_object;

	if( (result_constructor = env->GetMethodID(result_class, "<init>", "(Lcom/trej/regex/Regex;[Lcom/trej/regex/Match;)V")) &&
		(result_object = env->NewObject(result_class, result_constructor, regexParent, match_array))) {

			return result_object;

	}

	return nullptr;
}