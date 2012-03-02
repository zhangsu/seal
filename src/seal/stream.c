/*
 * stream.c is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 */

#include <seal/stream.h>
#include <seal/core.h>
#include <seal/raw.h>
#include <seal/fmt.h>
#include <seal/err.h>
#include <assert.h>
#include "ov.h"
#include "mpg.h"
#include "wav.h"

seal_stream_t*
seal_alloc_stream(void)
{
    seal_stream_t* stream;

    stream = _seal_calloc(1, sizeof (seal_stream_t));

    return stream;
}

seal_stream_t*
seal_init_stream(seal_stream_t* stream, const char* filename, seal_fmt_t fmt)
{
    assert(stream != 0 && filename != 0);

    SEAL_CHK(stream->id == 0, SEAL_STREAM_ALREADY_OPENED, 0);
    if (seal_ensure_fmt_known(filename, &fmt) == 0)
        return 0;

    switch (fmt) {
    case SEAL_WAV_FMT:
        return _seal_init_wav_stream(stream, filename);
    case SEAL_OV_FMT:
        return _seal_init_ov_stream(stream, filename);
    case SEAL_MPG_FMT:
        return _seal_init_mpg_stream(stream, filename);
    default:
        SEAL_ABORT(SEAL_BAD_AUDIO, 0);
    }
}

seal_stream_t*
seal_open_stream(const char* filename, seal_fmt_t fmt)
{
    seal_stream_t* stream;

    stream = seal_alloc_stream();
    if (stream == 0)
        return 0;

    if (seal_init_stream(stream, filename, fmt) == 0) {
        seal_close_stream(stream);
        return 0;
    }

    return stream;
}

int
seal_stream(seal_stream_t* stream, seal_raw_t* raw)
{
    assert(raw != 0 && raw->size > 0 && stream != 0 && stream->id != 0);

    SEAL_CHK(stream->id != 0, SEAL_STREAM_UNOPENED, 0);

    switch (stream->fmt) {
    case SEAL_WAV_FMT:
        return _seal_stream_wav(stream, raw);
    case SEAL_OV_FMT:
        return _seal_stream_ov(stream, raw);
    case SEAL_MPG_FMT:
        return _seal_stream_mpg(stream, raw);
    default:
        SEAL_ABORT(SEAL_BAD_AUDIO, 0);
    }
}

void
seal_rewind_stream(seal_stream_t* stream)
{
    assert(stream != 0 && stream->id != 0);

    if (stream->id != 0) {
        switch (stream->fmt) {
        case SEAL_WAV_FMT:
            _seal_rewind_wav_stream(stream);
            break;
        case SEAL_OV_FMT:
            _seal_rewind_ov_stream(stream);
            break;
        case SEAL_MPG_FMT:
            _seal_rewind_mpg_stream(stream);
            break;
        }
    }
}

int
seal_close_stream(seal_stream_t* stream)
{
    assert(stream != 0);

    SEAL_CHK(stream->id != 0, SEAL_STREAM_UNOPENED, 0);
    SEAL_CHK(!stream->in_use, SEAL_STREAM_INUSE, 0);

    switch (stream->fmt) {
    case SEAL_WAV_FMT:
        _seal_close_wav_stream(stream);
        break;
    case SEAL_OV_FMT:
        _seal_close_ov_stream(stream);
        break;
    case SEAL_MPG_FMT:
        _seal_close_mpg_stream(stream);
        break;
    }
    stream->id = 0;

    return 1;
}

int
seal_free_stream(seal_stream_t* stream)
{
    assert(stream != 0);

    if (stream->id != 0 && seal_close_stream(stream) == 0)
        return 0;

    _seal_free(stream);

    return 1;
}
