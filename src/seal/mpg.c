/*
 * mpg.c is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 */

#include <stdio.h>
#include <stddef.h>
#include <seal/pstdint.h>
#include <mpg123/mpg123.h>
#include <seal/mpg.h>
#include <seal/raw.h>
#include <seal/stream.h>
#include <seal/err.h>

/* Initial buffer size for loading. */
static const int INITIAL_BUF_SIZE = 32768;

static mpg123_handle* setup(seal_raw_attr_t*, const char*);
static int load(seal_raw_t*, mpg123_handle*);
static int read(seal_raw_t*, size_t*, mpg123_handle*);

int
_seal_load_mpg(seal_raw_t* raw, const char* filename)
{
    mpg123_handle* mh;
    seal_raw_t tmp_raw;
    int successful = 0;

    /* Use the default decoder. */
    mh = setup(&tmp_raw.attr, filename);
    if (mh == 0)
        return 0;

    if (load(&tmp_raw, mh) != 0) {
        *raw = tmp_raw;
        successful = 1;
    }

    mpg123_close(mh);

    return successful;
}

seal_stream_t*
_seal_init_mpg_stream(seal_stream_t* stream, const char* filename)
{
    mpg123_handle* mh;
    seal_raw_attr_t attr;

    mh = setup(&attr, filename);
    if (mh == 0)
        return 0;

    stream->id = mh;
    stream->fmt = SEAL_MPG_FMT;
    stream->attr = attr;

    return stream;
}

int
_seal_stream_mpg(seal_stream_t* stream, seal_raw_t* raw)
{
    size_t nbytes_loaded = 0;
    seal_raw_t tmp_raw;
    int err;

    if (seal_alloc_raw_data(&tmp_raw, raw->size) == 0)
        return -1;

    do err = read(&tmp_raw, &nbytes_loaded, stream->id);
    while (err == MPG123_OK && nbytes_loaded < tmp_raw.size);

    SEAL_CHK_S(nbytes_loaded != 0 || err == MPG123_DONE,
               SEAL_READ_MPG_FAILED, cleanup);

    raw->data = tmp_raw.data;
    raw->size = nbytes_loaded;
    raw->attr = stream->attr;

    return nbytes_loaded;

cleanup:
    seal_free_raw_data(&tmp_raw);

    return -1;
}

void
_seal_rewind_mpg_stream(seal_stream_t* stream)
{
    mpg123_seek(stream->id, 0, SEEK_SET);
}

void
_seal_close_mpg_stream(seal_stream_t* stream)
{
    mpg123_close(stream->id);
}

mpg123_handle*
setup(seal_raw_attr_t* attr, const char* filename)
{
    mpg123_handle* mh;
    long freq;
    int encoding;

    mh = mpg123_new(0, 0);
    SEAL_CHK(mh != 0, SEAL_INIT_MPG_FAILED, 0);

    SEAL_CHK_S(mpg123_open(mh, filename) == MPG123_OK,
               SEAL_INIT_MPG_FAILED, cleanup);

    mpg123_param(mh, MPG123_FLAGS, MPG123_QUIET, 0);

    attr->bit_depth = 16;
    /* Default encoding is MPG123_ENC_SIGNED_16. */
    mpg123_getformat(mh, &freq, &attr->nchannels, &encoding);
    attr->freq = freq;

    return mh;

cleanup:
    mpg123_close(mh);

    return 0;
}

int
read(seal_raw_t* raw, size_t* nbytes_loaded, mpg123_handle* mh)
{
    int err;
    size_t nbytes_read = 0;

    err = mpg123_read(mh, (unsigned char*) raw->data + *nbytes_loaded,
                        raw->size - *nbytes_loaded, &nbytes_read);
    *nbytes_loaded += nbytes_read;

    return err;
}

int
load(seal_raw_t* raw, mpg123_handle* mh)
{
    size_t nbytes_loaded = 0;

    if (seal_alloc_raw_data(raw, INITIAL_BUF_SIZE) == 0)
        return 0;

    do {
        if (seal_ensure_raw_data_size(raw, nbytes_loaded) == 0)
            goto cleanup;
    } while (read(raw, &nbytes_loaded, mh) == MPG123_OK);

    SEAL_CHK_S(nbytes_loaded != 0, SEAL_READ_MPG_FAILED, cleanup);
    
    raw->size = nbytes_loaded;

    return 1;

cleanup:
    seal_free_raw_data(raw);

    return 0;
}