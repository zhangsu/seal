#include <stdlib.h>
#include <vorbis/codec.h>
#define OV_EXCLUDE_STATIC_CALLBACKS
#include <vorbis/vorbisfile.h>
#include <seal/raw.h>
#include <seal/stream.h>
#include <seal/err.h>
#include "ov.h"
#include "reader.h"

/* Initial buffer size for loading. */
static const int INITIAL_BUF_SIZE = 4096;

static
seal_err_t
setup(seal_raw_attr_t* attr, OggVorbis_File* ovf, const char* filename)
{
    vorbis_info* vi;
    FILE* ov;

    ov = _seal_fopen(filename);
    if (ov == 0)
        return SEAL_CANNOT_OPEN_FILE;
    if (ov_open(ov, ovf, 0, 0) < 0) {
        _seal_fclose(ov);
        return SEAL_CANNOT_OPEN_OV;
    }
    vi = ov_info(ovf, -1);
    attr->bit_depth = 16;
    attr->nchannels = vi->channels;
    attr->freq = vi->rate;

    return SEAL_OK;
}

/* Caller passes `nbytes_loaded' to be able to accumulate the value. */
static
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

static
seal_err_t
load(seal_raw_t* raw, OggVorbis_File* ovf)
{
    long nbytes_read;
    unsigned long nbytes_loaded = 0;
    seal_err_t err;

    if ((err = seal_alloc_raw_data(raw, INITIAL_BUF_SIZE)) != SEAL_OK)
        return err;

    do {
        if ((err = seal_ensure_raw_data_size(raw, nbytes_loaded)) != SEAL_OK)
            goto cleanup;
        nbytes_read = read(raw, &nbytes_loaded, ovf);
    } while (nbytes_read > 0);

    /* If `nbytes_read' < 0 then an error occurred. */
    if (nbytes_read < 0) {
        err = SEAL_CANNOT_READ_OV;
        goto cleanup;
    }

    raw->size = nbytes_loaded;

    return SEAL_OK;

cleanup:
    seal_free_raw_data(raw);

    return err;
}

seal_err_t
_seal_load_ov(seal_raw_t* raw, const char* filename)
{
    seal_raw_t tmp_raw;
    OggVorbis_File ovf;
    seal_err_t err;

    if ((err = setup(&tmp_raw.attr, &ovf, filename)) != SEAL_OK)
        return err;

    if ((err = load(&tmp_raw, &ovf)) == SEAL_OK)
        *raw = tmp_raw;

    /* This closes the opened file handle too. */
    ov_clear(&ovf);

    return err;
}

seal_err_t
_seal_init_ov_stream(seal_stream_t* stream, const char* filename)
{
    seal_raw_attr_t attr;
    OggVorbis_File* povf;
    seal_err_t err;

    povf = malloc(sizeof (OggVorbis_File));
    if (povf == 0)
        return SEAL_CANNOT_ALLOC_MEM;

    if ((err = setup(&attr, povf, filename)) != SEAL_OK) {
        free(povf);
        return err;
    }

    stream->id = povf;
    stream->fmt = SEAL_OV_FMT;
    stream->attr = attr;

    return SEAL_OK;
}

seal_err_t
_seal_stream_ov(seal_stream_t* stream, seal_raw_t* raw, size_t* psize)
{
    long nbytes_read;
    unsigned long nbytes_streamed = 0;
    seal_raw_t tmp_raw;
    seal_err_t err;

    if ((err = seal_alloc_raw_data(&tmp_raw, raw->size)) != SEAL_OK)
        return err;

    do nbytes_read = read(&tmp_raw, &nbytes_streamed, stream->id);
    while (nbytes_streamed < tmp_raw.size && nbytes_read > 0);

    if (nbytes_read < 0) {
        err = SEAL_CANNOT_READ_OV;
        seal_free_raw_data(&tmp_raw);
        return err;
    }

    if (nbytes_streamed != 0) {
        raw->data = tmp_raw.data;
        raw->size = nbytes_streamed;
        raw->attr = stream->attr;
    } else {
        seal_free_raw_data(&tmp_raw);
    }
    *psize = nbytes_streamed;

    return SEAL_OK;
}

seal_err_t
_seal_rewind_ov_stream(seal_stream_t* stream)
{
    if (ov_time_seek(stream->id, 0) != 0)
        return SEAL_CANNOT_REWIND_OV;
    return SEAL_OK;
}

seal_err_t
_seal_close_ov_stream(seal_stream_t* stream)
{
    if (ov_clear(stream->id) != 0)
        return SEAL_CANNOT_CLOSE_OV;

    free(stream->id);
    return SEAL_OK;
}
