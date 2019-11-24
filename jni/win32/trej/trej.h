
#include "tre/tre.h"

#ifdef TREJ_EXPORTS
#define TREJ_API __declspec(dllexport)
#else
#define TREJ_API __declspec(dllimport)
#endif

typedef struct {
  long start_offset;
  long end_offset;
} trej_match_t;

typedef struct {
  size_t nmatch;
  trej_match_t *matchArray;
  char* error_message;
  bool is_approx;
  /* approximate matching related*/
  int match_cost;
  int insert_count;
  int delete_count;
  int substitution_count;

} trej_result_t;

#define TREJ_STR_MAX		1024
#define TREJ_ERR_MSG_LEN	0Xff

template<typename T>
int matchNotApprox(const regex_t*, const T*, size_t, trej_result_t*, int);
template<typename T>
int matchApprox(const regex_t*, const T*, size_t, trej_result_t*, int);

#ifdef __cplusplus
extern "C" {
#endif

TREJ_API int trejMatch(const char*, const char*, trej_result_t*, int, int = 0);
TREJ_API int trejnMatch(const char*, const char*, size_t, trej_result_t*, int, int = 0);

#ifdef __cplusplus
}
#endif
