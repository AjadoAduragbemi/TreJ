
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
  bool is_approx;
  size_t nmatch;
  trej_match_t *matchArray;
  char* error_message;
} trej_result_t;


#ifdef __cplusplus
extern "C" {
#endif

int matchNotApprox(const regex_t*, const char*, size_t, trej_result_t*, int);
int matchApprox(const regex_t*, const char*, size_t, trej_result_t*, int);

TREJ_API int trejMatch(const char*, const char*, trej_result_t*, int, int = 0);
TREJ_API int trejnMatch(const char*, const char*, size_t, trej_result_t*, int, int = 0);

#ifdef __cplusplus
}
#endif