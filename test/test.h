#ifndef _SEAL_TEST_H_
#define _SEAL_TEST_H_

#include <setjmp.h>
#include <seal/err.h>

extern jmp_buf next_test;
extern const char* TEST_FILENAME;

#define ASSERT(expr) assert(expr, __FILE__, __LINE__)
#define ASSERT_NO_ERR() assert_no_err(__FILE__, __LINE__)
#define ASSERT_OK(expr) assert_ok(expr, __FILE__, __LINE__)
#define ASSERT_FAIL(expr, err) assert_fail(expr, err, __FILE__, __LINE__)

void assert(int /*expr*/, const char* /*filename*/, int /*linenum*/);
void assert_no_err(const char* /*filename*/, int /*linenum*/);
void assert_ok(int /*expr*/, const char* /*filename*/, int /*linenum*/);
void assert_fail(int /*expr*/, seal_err_t, const char* /*filename*/,
                 int /*linenum*/);

#endif /* _SEAL_TEST_H_ */