#include <stdio.h>
#include <setjmp.h>
#include <seal.h>
#include "test.h"

#ifdef _MSC_VER
# include <crtdbg.h>
#endif
#ifdef _WIN32
# include <Windows.h>
#endif

const char* TEST_FILENAME = ".test";

jmp_buf next_test;

static int passed = 1;

void test_src(void);
void test_listener(void);

int
main()
{
    test_src();
    test_listener();

    if (passed) {
        const char* str = "All tests passed!\n";
        printf("%s\n", str);
#ifdef _WIN32
        OutputDebugString(str);
#endif
#ifdef _MSC_VER
        /* Check memory leak. */
        _CrtDumpMemoryLeaks();
#endif
    }

    return 0;
}

void
assert(int expr, const char* filename, int linenum)
{
    if (!expr) {
        char str[512];
        sprintf(str, "%s(%d): Test failed!\n", filename, linenum);
        fprintf(stderr, "%s", str);
#ifdef _WIN32
        OutputDebugString(str);
#endif
        passed = 0;
        longjmp(next_test, 1);
    }
}

void
assert_ok(seal_err_t err, const char* filename, int linenum)
{
    assert(err == SEAL_OK, filename, linenum);
}
