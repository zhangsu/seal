#include <stdlib.h>
#include <al/al.h>
#include <seal/buf.h>
#include <seal/core.h>
#include <seal/raw.h>
#include <seal/fmt.h>
#include <seal/err.h>
#include "ov.h"
#include "mpg.h"
#include "wav.h"

seal_err_t
seal_init_buf(seal_buf_t* buf)
{
    return _seal_init_obj(buf, alGenBuffers);
}

seal_err_t
seal_destroy_buf(seal_buf_t* buf)
{
    return _seal_destroy_obj(buf, alDeleteBuffers, alIsBuffer);
}

seal_err_t
seal_load2buf(seal_buf_t* buf, const char* filename, seal_fmt_t fmt)
{
    seal_raw_t raw;
    seal_err_t err;

    /* `raw.data' will be dynamically allocated by the callee. */
    if ((err = seal_load(&raw, filename, fmt)) != SEAL_OK)
        return err;

    err = seal_raw2buf(buf, &raw);
    free(raw.data);

    return err;
}

seal_err_t
seal_raw2buf(seal_buf_t* buf, seal_raw_t* raw)
{
    return _seal_raw2buf(buf->id, raw);
}

seal_err_t
_seal_raw2buf(unsigned int buf, seal_raw_t* raw)
{
    alBufferData(buf, _seal_get_buf_fmt(raw->attr.nchannels,
                                        raw->attr.bit_depth),
                 raw->data, raw->size, raw->attr.freq);

    return _seal_get_openal_err();
}

seal_err_t
seal_get_buf_size(seal_buf_t* buf, int* psize)
{
    return _seal_geti(buf, AL_SIZE, psize, alGetBufferi);
}

seal_err_t
seal_get_buf_freq(seal_buf_t* buf, int* pfreq)
{
    return _seal_geti(buf, AL_FREQUENCY, pfreq, alGetBufferi);
}

seal_err_t
seal_get_buf_bps(seal_buf_t* buf, int* pbps)
{
    return _seal_geti(buf, AL_BITS, pbps, alGetBufferi);
}

seal_err_t
seal_get_buf_nchannels(seal_buf_t* buf, int* pnchannels)
{
    return _seal_geti(buf, AL_CHANNELS, pnchannels, alGetBufferi);
}

seal_err_t
seal_load(seal_raw_t* raw, const char* filename, seal_fmt_t fmt)
{
    seal_err_t err;

    if ((err = seal_ensure_fmt_known(filename, &fmt)) != SEAL_OK)
        return err;

    switch (fmt) {
    case SEAL_WAV_FMT:
        return _seal_load_wav(raw, filename);
    case SEAL_OV_FMT:
        return _seal_load_ov(raw, filename);
    case SEAL_MPG_FMT:
        return _seal_load_mpg(raw, filename);
    default:
        return SEAL_BAD_AUDIO;
    }
}

/* All argument will have a single evaluation here. */
#define RETURN_FMT_CONST(bit_depth, type) do                                \
{                                                                           \
    switch (bit_depth) {                                                    \
    case 8:                                                                 \
        return AL_FORMAT_##type##8;                                         \
    default:                                                                \
        return AL_FORMAT_##type##16;                                        \
    }                                                                       \
} while (0)

int
_seal_get_buf_fmt(int nchannels, int bit_depth)
{
    if (nchannels == 1)
        RETURN_FMT_CONST(bit_depth, MONO);
    else
        RETURN_FMT_CONST(bit_depth, STEREO);
}
