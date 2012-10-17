/*
 * stream.c is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 */

#include <stddef.h>
#include <seal/stream.h>
#include <seal/raw.h>
#include <seal/fmt.h>
#include <seal/err.h>
#include "ov.h"
#include "mpg.h"
#include "wav.h"

seal_err_t
seal_open_stream(seal_stream_t* stream, const char* filename, seal_fmt_t fmt)
{
    seal_err_t err;

    if ((err = seal_ensure_fmt_known(filename, &fmt)) != SEAL_OK)
        return err;

    switch (fmt) {
    case SEAL_WAV_FMT:
        err = _seal_init_wav_stream(stream, filename);
        break;
    case SEAL_OV_FMT:
        err =  _seal_init_ov_stream(stream, filename);
        break;
    case SEAL_MPG_FMT:
        err =  _seal_init_mpg_stream(stream, filename);
        break;
    default:
        return SEAL_BAD_AUDIO;
    }

    return err;
}

seal_err_t
seal_stream(seal_stream_t* stream, seal_raw_t* raw, size_t* psize)
{
    if (stream->id == 0)
        return SEAL_STREAM_UNOPENED;

    switch (stream->fmt) {
    case SEAL_WAV_FMT:
        return _seal_stream_wav(stream, raw, psize);
    case SEAL_OV_FMT:
        return _seal_stream_ov(stream, raw, psize);
    case SEAL_MPG_FMT:
        return _seal_stream_mpg(stream, raw, psize);
    default:
        return SEAL_BAD_AUDIO;
    }
}

seal_err_t
seal_rewind_stream(seal_stream_t* stream)
{
    if (stream->id == 0)
        return SEAL_STREAM_UNOPENED;

    switch (stream->fmt) {
    case SEAL_WAV_FMT:
        return _seal_rewind_wav_stream(stream);
    case SEAL_OV_FMT:
        return _seal_rewind_ov_stream(stream);
    case SEAL_MPG_FMT:
        return _seal_rewind_mpg_stream(stream);
    default:
        return SEAL_BAD_AUDIO;
    }

    return SEAL_OK;
}

seal_err_t
seal_close_stream(seal_stream_t* stream)
{
    seal_err_t err;

    if (stream->id == 0)
        return SEAL_STREAM_UNOPENED;

    switch (stream->fmt) {
    case SEAL_WAV_FMT:
        err = _seal_close_wav_stream(stream);
        break;
    case SEAL_OV_FMT:
        err = _seal_close_ov_stream(stream);
        break;
    case SEAL_MPG_FMT:
        err = _seal_close_mpg_stream(stream);
        break;
    default:
        return SEAL_BAD_AUDIO;
    }

    if (err == SEAL_OK) {
        stream->id = 0;
        stream->attr.freq = 0;
        stream->attr.nchannels = 1;
        stream->attr.bit_depth = 16;
    }

    return err;
}
