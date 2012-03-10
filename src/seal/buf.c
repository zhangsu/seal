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
#include <seal/err.h>
#include <assert.h>
#include "ov.h"
#include "mpg.h"
#include "wav.h"

struct seal_buf_t
{
    unsigned int id;
};

static int
geti(seal_buf_t* buf, int key)
{
    int value;

    assert(alIsBuffer(buf->id));

    _seal_lock_openal();
    alGetBufferi(buf->id, key, &value);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return value;
}

seal_buf_t*
seal_alloc_buf(void)
{
    return _seal_alloc_obj(sizeof (seal_buf_t), alGenBuffers);
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
    return _seal_free_obj(buf, alDeleteBuffers, alIsBuffer);
}

int
seal_load2buf(seal_buf_t* buf, const char* filename, seal_fmt_t fmt)
{
    int successful;
    seal_raw_t raw;

    assert(alIsBuffer(buf->id));

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
    assert(raw != 0 && alIsBuffer(buf->id));

    _seal_lock_openal();
    alBufferData(buf->id, _seal_get_buf_fmt(raw->attr.nchannels,
                                            raw->attr.bit_depth),
                 raw->data, raw->size, raw->attr.freq);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return 1;
}

unsigned int
_seal_get_buf_id(seal_buf_t* buf)
{
    assert(alIsBuffer(buf->id));

    return buf->id;
}

size_t
seal_get_buf_size(seal_buf_t* buf)
{
    return geti(buf, AL_SIZE);
}

int
seal_get_buf_freq(seal_buf_t* buf)
{
    return geti(buf, AL_FREQUENCY);
}

int
seal_get_buf_bps(seal_buf_t* buf)
{
    return geti(buf, AL_BITS);
}

int
seal_get_buf_nchannels(seal_buf_t* buf)
{
    return geti(buf, AL_CHANNELS);
}

int
seal_load(seal_raw_t* raw, const char* filename, seal_fmt_t fmt)
{
    assert(raw != 0);

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
_seal_get_buf_fmt(int nchannels, int bit_depth)
{
    if (nchannels == 1)
        RETURN_FMT_CONST(bit_depth, MONO);
    else if (nchannels > 1)
        RETURN_FMT_CONST(bit_depth, STEREO);
    else
        SEAL_ABORT(SEAL_BAD_WAV_NCHANNELS, 0);
}
