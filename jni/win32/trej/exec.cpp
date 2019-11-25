#include "stdafx.h"
#include "exec.h"
#include "trej.h"

jobject trej_regex_exec(JNIEnv *env, jobject object) {

	jobject match_result = nullptr;

	env->MonitorEnter(object);
	jclass regex_class = env->GetObjectClass(object);
	jclass result_class = env->FindClass("Lcom/trej/regex/MatchResult;");
	jclass match_class = env->FindClass("Lcom/trej/regex/Match;");
	trej_result_t* trej_match = new trej_result_t;

	trej_match->error_message = nullptr;
	trej_match->matchArray = nullptr;
	trej_match->nmatch = 0;

	trej_match->match_cost = 0;
	trej_match->insert_count = 0;
	trej_match->delete_count = 0;
	trej_match->substitution_count = 0;

	if(regex_class != nullptr && result_class != nullptr && match_class != nullptr) {
		jfieldID preg_id = env->GetFieldID(regex_class, "preg", "J");
		jfieldID input_id = env->GetFieldID(regex_class, "input", "Ljava/lang/String;");
		jfieldID error_message_id = env->GetFieldID(regex_class, "errorMessage", "Ljava/lang/String;");
		jfieldID error_value_id = env->GetFieldID(regex_class, "errorValue", "I");
		jstring input;
		regex_t* preg;

		if( (preg = reinterpret_cast<regex_t*>(env->GetLongField(object, preg_id))) != nullptr && 
			(input = static_cast<jstring>(env->GetObjectField(object, input_id))) ) {
				jint trej_error_value;
				jboolean isApproximate;

				auto mb_string = env->GetStringUTFChars(input, nullptr);
				wchar_t* wc_string = nullptr;
#if defined(TRE_WCHAR) && defined(HAVE_WCHAR_H)
				if(hasWideChar(mb_string)) {
					size_t newsize = strnlen(mb_string, TREJ_STR_MAX) + 1;
					wc_string = new wchar_t[newsize];
					size_t wc_string_len = 0;
					mbstowcs_s(&wc_string_len, wc_string, newsize, mb_string, _TRUNCATE);
				}
#endif
				if((isApproximate = tre_have_approx(preg)) == JNI_FALSE) {
					trej_error_value = (wc_string == nullptr) ? 
						matchNotApprox(preg, mb_string, 0, trej_match, 0) : 
						matchNotApprox(preg, wc_string, 0, trej_match, 0);
				} else {
					trej_error_value = (wc_string == nullptr) ? 
						matchApprox(preg, mb_string, 0, trej_match, 0) :
						matchApprox(preg, wc_string, 0, trej_match, 0);
				}

				env->ReleaseStringUTFChars(input, mb_string);
				if(wc_string != nullptr) {
					delete[] wc_string;
				}

				env->SetIntField(object, error_value_id, trej_error_value);
				env->SetObjectField(object, error_message_id, env->NewStringUTF(trej_match->error_message));
				delete[] trej_match->error_message;

				jsize nmatch = static_cast<jsize>(trej_match->nmatch);
				jobjectArray match_array = env->NewObjectArray(nmatch, match_class, 0);

				for(int index = 0; index < nmatch; index++) {
					trej_match_t* match = &trej_match->matchArray[index];
					jobject match_object = make_match(env, match_class, match->start_offset, match->end_offset);
					env->SetObjectArrayElement(match_array, index, match_object);
				}

				delete[] trej_match->matchArray;
				trej_match->matchArray = nullptr;

				match_result = make_match_result(env, result_class, object, match_array, isApproximate);

				if(match_result != nullptr && isApproximate == JNI_TRUE) {
					jfieldID match_cost_id = env->GetFieldID(result_class, "matchCost", "I");
					jfieldID insert_count_id = env->GetFieldID(result_class, "insertCount", "I");
					jfieldID delete_count_id = env->GetFieldID(result_class, "deleteCount", "I");
					jfieldID substitution_count = env->GetFieldID(result_class, "substitutionCount", "I");

					env->SetIntField(match_result, match_cost_id, trej_match->match_cost);
					env->SetIntField(match_result, insert_count_id, trej_match->insert_count);
					env->SetIntField(match_result, delete_count_id, trej_match->delete_count);
					env->SetIntField(match_result, substitution_count, trej_match->substitution_count);
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
		(match_object = env->NewObject(match_class, match_constructor)) ) {
			match_start_offset_id = env->GetFieldID(match_class, "startOffset", "I");
			match_end_offset_id = env->GetFieldID(match_class, "endOffset", "I");
			env->SetIntField(match_object, match_start_offset_id, start_offset);
			env->SetIntField(match_object, match_end_offset_id, end_offset);
			return match_object;
	}
	return nullptr;
}

jobject make_match_result(JNIEnv *env, jclass result_class, jobject regexParent,  jobjectArray match_array, jboolean isApproximate) {
	jmethodID result_constructor;
	jobject result_object = nullptr;

	if(result_constructor = env->GetMethodID(result_class, "<init>", "(Lcom/trej/regex/Regex;[Lcom/trej/regex/Match;Z)V")) {
		result_object = env->NewObject(result_class, result_constructor, regexParent, match_array, isApproximate);
	}

	return result_object;
}
