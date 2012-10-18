#include <seal.h>

int main()
{
    seal_err_t err;
    seal_src_t src;
    seal_stream_t stream;
 
    err = seal_startup(0);

    err = seal_init_src(&src);
    err = seal_open_stream(&stream, "\x44\x3a\x2f\x6d\x75\x73\x69\x63\x2f\xe9\x87\x91\xe6\xb1\xa0\x20\x2d\x20\xe5\xbf\x83\xe5\x9c\xa8\xe8\xb7\xb3\x2e\x6d\x70\x33", SEAL_UNKNOWN_FMT);
    err = seal_set_src_stream(&src, &stream);
    err = seal_set_src_pitch(&src, 1);
    err = seal_play_src(&src);

    _seal_sleep(10000);

    seal_destroy_src(&src);
    seal_close_stream(&stream);

    seal_cleanup();

    return 0;
}