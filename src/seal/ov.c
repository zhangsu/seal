/*
 * ov.c is part of the Scorched End Audio Library (SEAL) and is
 * licensed under the terms of the GNU Lesser General Public License. See
 * COPYING attached with the library.
 */

#include <vorbis/codec.h>
#define OV_EXCLUDE_STATIC_CALLBACKS
#include <vorbis/vorbisfile.h>
#include <seal/core.h>
#include <seal/raw.h>
#include <seal/stream.h>
#include <seal/err.h>
#include "ov.h"
#include "reader.h"

/* Initial buffer size for loading. */
static const int INITIAL_BUF_SIZE = 4096;

static int setup(seal_raw_attr_t*, OggVorbis_File*, const char*);
static int load(seal_raw_t*, OggVorbis_File*);
static long read(seal_raw_t*, unsigned long*, OggVorbis_File*);

int
_seal_load_ov(seal_raw_t* raw, const char* filename)
{
    seal_raw_t tmp_raw;
    OggVorbis_File ovf;
    int successful = 0;

    if (setup(&tmp_raw.attr, &ovf, filename) == 0)
        return 0;

    if (load(&tmp_raw, &ovf) != 0) {
        *raw = tmp_raw;
        successful = 1;
    }

    /* This closes the opened file handle too. */
    ov_clear(&ovf);

    return successful;
}

seal_stream_t*
_seal_init_ov_stream(seal_stream_t* stream, const char* filename)
{
    seal_raw_attr_t attr;
    OggVorbis_File* povf;

    povf = _seal_malloc(sizeof (OggVorbis_File));
    if (povf == 0)
        return 0;

    if (setup(&attr, povf, filename) == 0) {
        _seal_free(povf);
        return 0;
    }

    stream->id = povf;
    stream->fmt = SEAL_OV_FMT;
    stream->attr = attr;

    return stream;
}

int
_seal_stream_ov(seal_stream_t* stream, seal_raw_t* raw)
{
    long nbytes_read;
    unsigned long nbytes_loaded = 0;
    seal_raw_t tmp_raw;

    if (seal_alloc_raw_data(&tmp_raw, raw->size) == 0)
        return -1;

    do nbytes_read = read(&tmp_raw, &nbytes_loaded, stream->id);
    while (nbytes_loaded < tmp_raw.size && nbytes_read > 0);

    /* Either error or end of stream. */
    if (nbytes_read < 0 || nbytes_loaded == 0) {
        if (nbytes_read < 0)
            _seal_set_err(SEAL_CANNOT_READ_OV);
        seal_free_raw_data(&tmp_raw);
        return nbytes_read;
    }

    raw->data = tmp_raw.data;
    raw->size = nbytes_loaded;
    raw->attr = stream->attr;

    return nbytes_loaded;
}

void
_seal_rewind_ov_stream(seal_stream_t* stream)
{
    ov_time_seek(stream->id, 0);
}

void
_seal_close_ov_stream(seal_stream_t* stream)
{
    ov_clear(stream->id);
    _seal_free(stream->id);
}

int
setup(seal_raw_attr_t* attr, OggVorbis_File* ovf, const char* filename)
{
    vorbis_info* vi;
    FILE* ov;

    ov = _seal_fopen(filename);
    if (ov == 0)
        return 0;
    SEAL_CHK_S(ov_open(ov, ovf, 0, 0) >= 0, SEAL_CANNOT_OPEN_OV, cleanup);
    vi = ov_info(ovf, -1);
    attr->bit_depth = 16;
    attr->nchannels = vi->channels;
    attr->freq = vi->rate;

    return 1;

cleanup:
    _seal_fclose(ov);

    return 0;
}

/* Caller passes `nbytes_loaded' to be able to accumulate the value. */
long
read(seal_raw_t* raw, unsigned long* nbytes_loaded, OggVorbis_File* ovf)
{
    long nbytes_read;
    int bitstream;

    nbytes_read = ov_read(ovf, (char*) raw->data + *nbytes_loaded,
                          raw->size - *nbytes_loaded, 0, 2, 1, &bitstream);
    *nbytes_loaded += nbytes_read;

    return nbytes_read;
}

int
load(seal_raw_t* raw, OggVorbis_File* ovf)
{
    long nbytes_read;
    unsigned long nbytes_loaded = 0;

    if (seal_alloc_raw_data(raw, INITIAL_BUF_SIZE) == 0)
        return 0;

    do {
        if (seal_ensure_raw_data_size(raw, nbytes_loaded) == 0)
            goto cleanup;
        nbytes_read = read(raw, &nbytes_loaded, ovf);
    } while (nbytes_read > 0);
    /* If `nbytes_read' < 0 then an error occurred. */
    SEAL_CHK_S(nbytes_read == 0, SEAL_CANNOT_READ_OV, cleanup);

    raw->size = nbytes_loaded;

    return 1;

cleanup:
    seal_free_raw_data(raw);

    return 0;
}
