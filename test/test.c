#include <stdio.h>
#include <setjmp.h>
#include <seal.h>
#include <seal/threading.h>
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

void
test_random(void)
{
    int i;
    seal_src_t* src;
    seal_buf_t* buf;
    seal_stream_t* stream0;
    seal_stream_t* stream1;
    seal_stream_t* stream2;
    
    seal_startup(0);
    src = seal_alloc_src();
    //buf = seal_new_buf("D:/music/云南民歌 - 阿佤人民唱新歌.mp3", SEAL_UNKNOWN_FMT);
    stream0 = seal_open_stream("D:/music/谢安琪 - 大城小动.mp3", SEAL_UNKNOWN_FMT);
    stream1 = seal_open_stream("C:/Users/Su/Desktop/央金兰泽 - 遇上你是我的缘.wav", SEAL_UNKNOWN_FMT);
    stream2 = seal_open_stream("C:/Users/Su/Desktop/梁咏琪 - 胆小鬼.ogg", SEAL_UNKNOWN_FMT);
    //seal_set_src_buf(src, buf);
    seal_set_src_stream(src, stream0);
    seal_set_src_pitch(src, 1.2f);
    seal_set_src_looping(src, 1);
    seal_play_src(src);
    for (i = 0; i <= 3200; ++i) {
        seal_src_state_t state = seal_get_src_state(src);
        if (SEAL_PLAYING != state && SEAL_PAUSED != state)
            break;
        seal_update_src(src);
        _seal_sleep(10);
        if (i % 100 == 0)
            seal_set_src_queue_size(src, rand() % 5 + 2);
        else if ((i + 50) % 100 == 0)
            seal_set_src_chunk_size(src, (rand() % 2 + 1) * 9216);
        switch (i) {
        case 400:
            //seal_detach_src_audio(src);
            seal_set_src_stream(src, stream1);
            //seal_play_src(src);
            break;
        case 800:
            seal_set_src_stream(src, stream2);
            break;
        case 1200:
            seal_set_src_stream(src, stream0);
            break;
        case 1600:
            seal_set_src_stream(src, stream1);
            break;
        case 2000:
            seal_set_src_stream(src, stream2);
            break;
        case 2400:
            seal_set_src_stream(src, stream0);
            break;
        case 2800:
            seal_set_src_stream(src, stream1);
            break;
        }
    }
    seal_free_src(src);
    //seal_free_buf(buf);
    seal_free_stream(stream0);
    seal_free_stream(stream1);
    seal_free_stream(stream2);
    seal_cleanup();
}

int
main()
{
    test_src();
    test_listener();
    //test_random();

    if (passed) {
        const char* str = "All tests passed!\n";
        printf(str);
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
assert_no_err(const char* filename, int linenum)
{
    assert(seal_get_err() == SEAL_OK, filename, linenum);
}

void
assert_ok(int expr, const char* filename, int linenum)
{
    assert(expr, filename, linenum);
    assert_no_err(filename, linenum);
}

void
assert_fail(int expr, seal_err_t err, const char* filename, int linenum)
{
    assert(expr, filename, linenum);
    assert(seal_get_err() == err, filename, linenum);
}