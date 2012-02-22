/*
 * buf.c is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 */

#include <stddef.h>
#include <al/al.h>
#include <seal/buf.h>
#include <seal/core.h>
#include <seal/raw.h>
#include <seal/fmt.h>
#include <seal/wav.h>
#include <seal/ov.h>
#include <seal/mpg.h>
#include <seal/err.h>
#include <assert.h>

struct seal_buf_t
{
    unsigned int id;
};

static int get_attr(seal_buf_t*, int);

seal_buf_t*
seal_alloc_buf(void)
{
    return seal_alloc_nbuf(1);
}

seal_buf_t*
seal_alloc_nbuf(unsigned int n)
{
    seal_buf_t* buf;

    assert(n > 0);

    buf = _seal_malloc(sizeof (seal_buf_t) * n);
    if (buf == 0)
        return 0;

    _seal_lock_openal();
    /* Can't do this if `struct seal_buf_t' gets extended. */
    alGenBuffers(n, (unsigned int*) buf);
    SEAL_CHK_AL_S(AL_OUT_OF_MEMORY, SEAL_ALLOC_BUF_FAILED, cleanup);

    return buf;

cleanup:
    _seal_free(buf);

    return 0;
}

/*
 * Currently `seal_init_buf' does the same thing as `seal_load2buf' (except
 * that their return values are different), but its behaviour may get extended
 * in the future...
 */
seal_buf_t*
seal_init_buf(seal_buf_t* buf, const char* filename, seal_fmt_t fmt)
{
    if (seal_load2buf(buf, filename, fmt) == 0)
        return 0;

    return buf;
}

seal_buf_t*
seal_new_buf(const char* filename, seal_fmt_t fmt)
{
    seal_buf_t* buf;

    assert(filename != 0);

    buf = seal_alloc_buf();
    if (buf == 0)
        return 0;

    if (seal_init_buf(buf, filename, fmt) == 0) {
        seal_free_buf(buf);
        return 0;
    }

    return buf;
}

int
seal_free_buf(seal_buf_t* buf)
{
    return seal_free_nbuf(buf, 1);
}

int
seal_free_nbuf(seal_buf_t* buf, unsigned int n)
{
    assert(buf != 0 && n > 0);

    if (alIsBuffer(buf->id)) {
        _seal_lock_openal();
        /* Can't do this if `struct seal_buf_t' gets extended. */
        alDeleteBuffers(n, (unsigned int*) buf);
        SEAL_CHK_AL2(AL_INVALID_OPERATION, SEAL_BUF_INUSE,
                     AL_INVALID_VALUE, SEAL_FREE_BUF_FAILED, 0);
    }

    _seal_free(buf);

    return 1;
}

int
seal_load2buf(seal_buf_t* buf, const char* filename, seal_fmt_t fmt)
{
    int successful;
    seal_raw_t raw;

    assert(buf != 0 && filename != 0 && alIsBuffer(buf->id));

    /* `raw.data' will be dynamically allocated by the callee. */
    if (seal_load(&raw, filename, fmt) == 0)
        return 0;

    successful = seal_raw2buf(buf, &raw);
    _seal_free(raw.data);

    return successful;
}

int
seal_raw2buf(seal_buf_t* buf, seal_raw_t* raw)
{
    assert(buf != 0 && raw != 0 && alIsBuffer(buf->id));

    _seal_lock_openal();
    alBufferData(buf->id, _seal_get_buf_fmt(raw->attr.nchannels,
                                            raw->attr.bit_depth),
                 raw->data, raw->size, raw->attr.freq);
    SEAL_CHK_AL3(AL_OUT_OF_MEMORY, SEAL_MEM_ALLOC_FAILED,
                 AL_INVALID_VALUE, SEAL_BUF_INUSE,
                 AL_INVALID_OPERATION, SEAL_BUF_INUSE, 0);

    return 1;
}

unsigned int
_seal_get_buf_id(seal_buf_t* buf)
{
    assert(buf != 0 && alIsBuffer(buf->id));

    return buf->id;
}

size_t
seal_get_buf_size(seal_buf_t* buf)
{
    return get_attr(buf, AL_SIZE);
}

int
seal_get_buf_freq(seal_buf_t* buf)
{
    return get_attr(buf, AL_FREQUENCY);
}

int
seal_get_buf_bps(seal_buf_t* buf)
{
    return get_attr(buf, AL_BITS);
}

int
seal_get_buf_nchannels(seal_buf_t* buf)
{
    return get_attr(buf, AL_CHANNELS);
}

int
get_attr(seal_buf_t* buf, int key)
{
    int ret;

    assert(buf != 0 && alIsBuffer(buf->id));

    alGetBufferi(buf->id, key, &ret);

    return ret;
}

int
seal_load(seal_raw_t* raw, const char* filename, seal_fmt_t fmt)
{
    assert(raw != 0 && filename != 0);

    if (seal_ensure_fmt_known(filename, &fmt) == 0)
        return 0;

    switch (fmt) {
    case SEAL_WAV_FMT:
        return _seal_load_wav(raw, filename);
    case SEAL_OV_FMT:
        return _seal_load_ov(raw, filename);
    case SEAL_MPG_FMT:
        return _seal_load_mpg(raw, filename);
    default:
        SEAL_ABORT(SEAL_BAD_AUDIO, 0);
    }
}

/* All argument will have a single evaluation here. */
#define RETURN_FMT_CONST(bit_depth, type) do                                \
{                                                                           \
    switch (bit_depth) {                                                    \
    case 8:                                                                 \
        return AL_FORMAT_##type##8;                                         \
    case 16:                                                                \
        return AL_FORMAT_##type##16;                                        \
    default:                                                                \
        SEAL_ABORT(SEAL_BAD_WAV_BPS, 0);                                    \
    }                                                                       \
} while (0)

int
_seal_get_buf_fmt(uint32_t nchannels, uint32_t bit_depth)
{
    if (nchannels == 1)
        RETURN_FMT_CONST(bit_depth, MONO);
    else if (nchannels > 1)
        RETURN_FMT_CONST(bit_depth, STEREO);
    else
        SEAL_ABORT(SEAL_BAD_WAV_NCHANNELS, 0);
}
