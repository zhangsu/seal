#ifndef _SEAL_TEST_H_
#define _SEAL_TEST_H_

#include <setjmp.h>
#include <seal/err.h>

extern jmp_buf next_test;
extern const char* TEST_FILENAME;

#define ASSERT(expr) assert(expr, __FILE__, __LINE__)
#define ASSERT_OK(err) assert_ok(err, __FILE__, __LINE__)

void assert(int /*expr*/, const char* /*filename*/, int /*linenum*/);
void assert_ok(seal_err_t, const char* /*filename*/, int /*linenum*/);

#endif /* _SEAL_TEST_H_ */