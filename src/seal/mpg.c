/*
 * mpg.c is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 */

#include <stdio.h>
#include <stddef.h>
#include <mpg123/mpg123.h>
#include <seal/raw.h>
#include <seal/stream.h>
#include <seal/err.h>
#include "mpg.h"

/* Initial buffer size for loading. */
static const int INITIAL_BUF_SIZE = 32768;


static mpg123_handle*
setup(seal_raw_attr_t* attr, const char* filename)
{
    mpg123_handle* mh;
    long freq;
    int encoding;

    /* Use the default decoder. */
    mh = mpg123_new(0, 0);
    if (mh == 0)
        return 0;

    if (mpg123_open(mh, filename) != MPG123_OK)
        goto cleanup;

#ifndef NDEBUG
    mpg123_param(mh, MPG123_FLAGS, MPG123_QUIET, 0);
#endif

    attr->bit_depth = 16;
    /* Default encoding is MPG123_ENC_SIGNED_16. */
    if (mpg123_getformat(mh, &freq, &attr->nchannels, &encoding) != MPG123_OK)
        goto cleanup;
    attr->freq = freq;

    return mh;

cleanup:
    mpg123_close(mh);

    return 0;
}

static int
read(seal_raw_t* raw, size_t* nbytes_loaded, mpg123_handle* mh)
{
    int err;
    size_t nbytes_read = 0;

    err = mpg123_read(mh, (unsigned char*) raw->data + *nbytes_loaded,
                      raw->size - *nbytes_loaded, &nbytes_read);
    *nbytes_loaded += nbytes_read;

    return err;
}

static seal_err_t
load(seal_raw_t* raw, mpg123_handle* mh)
{
    size_t nbytes_loaded = 0;
    seal_err_t err;
    
    if ((err = seal_alloc_raw_data(raw, INITIAL_BUF_SIZE)) != SEAL_OK)
        return err;

    do {
        if ((err = seal_ensure_raw_data_size(raw, nbytes_loaded)) != SEAL_OK)
            goto cleanup;
    } while (read(raw, &nbytes_loaded, mh) == MPG123_OK);

    if (nbytes_loaded == 0) {
        err = SEAL_CANNOT_READ_MPG;
        goto cleanup;
    }

    raw->size = nbytes_loaded;

    return SEAL_OK;

cleanup:
    seal_free_raw_data(raw);

    return err;
}

seal_err_t
_seal_load_mpg(seal_raw_t* raw, const char* filename)
{
    mpg123_handle* mh;
    seal_raw_t tmp_raw;
    seal_err_t err;

    mh = setup(&tmp_raw.attr, filename);
    if (mh == 0)
        return SEAL_CANNOT_INIT_MPG;

    if ((err = load(&tmp_raw, mh)) == SEAL_OK)
        *raw = tmp_raw;

    mpg123_close(mh);

    return err;
}

seal_err_t
_seal_init_mpg_stream(seal_stream_t* stream, const char* filename)
{
    mpg123_handle* mh;
    seal_raw_attr_t tmp_attr;

    mh = setup(&tmp_attr, filename);
    if (mh == 0)
        return SEAL_CANNOT_INIT_MPG;

    stream->id = mh;
    stream->fmt = SEAL_MPG_FMT;
    stream->attr = tmp_attr;

    return SEAL_OK;
}

seal_err_t
_seal_stream_mpg(seal_stream_t* stream, seal_raw_t* raw, size_t* psize)
{
    seal_raw_t tmp_raw;
    int mpg123_err;
    size_t nbytes_streamed = 0;
    seal_err_t err;
    
    if ((err = seal_alloc_raw_data(&tmp_raw, raw->size)) != SEAL_OK)
        return err;

    do mpg123_err = read(&tmp_raw, &nbytes_streamed, stream->id);
    while (mpg123_err == MPG123_OK && nbytes_streamed < tmp_raw.size);

    if (nbytes_streamed == 0 && mpg123_err != MPG123_DONE) {
        seal_free_raw_data(&tmp_raw);
        return SEAL_CANNOT_READ_MPG;
    }

    raw->data = tmp_raw.data;
    raw->size = nbytes_streamed;
    raw->attr = stream->attr;
    *psize = nbytes_streamed;

    return SEAL_OK;
}

seal_err_t
_seal_rewind_mpg_stream(seal_stream_t* stream)
{
    if (mpg123_seek(stream->id, 0, SEEK_SET) < 0)
        return SEAL_CANNOT_REWIND_MPG;
    return SEAL_OK;
}

seal_err_t
_seal_close_mpg_stream(seal_stream_t* stream)
{
    if (mpg123_close(stream->id) != MPG123_OK)
        return SEAL_CANNOT_CLOSE_MPG;
    return SEAL_OK;
}
