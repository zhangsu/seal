#include <stdlib.h>
#include <stdio.h>
#include <seal/raw.h>
#include <seal/stream.h>
#include <seal/err.h>
#include "reader.h"
#include "wav.h"

enum
{
    FMT_ = SEAL_MKTAG('f', 'm', 't', ' '),
    DATA = SEAL_MKTAG('d', 'a', 't', 'a'),
    FACT = SEAL_MKTAG('f', 'a', 'c', 't'),
    WAVL = SEAL_MKTAG('w', 'a', 'v', 'l'),
    SLNT = SEAL_MKTAG('s', 'l', 'n', 't'),
    CUE_ = SEAL_MKTAG('c', 'u', 'e', ' '),
    PLST = SEAL_MKTAG('p', 'l', 's', 't'),
    LIST = SEAL_MKTAG('l', 'i', 's', 't'),
    LABL = SEAL_MKTAG('l', 'a', 'b', 'l'),
    NOTE = SEAL_MKTAG('n', 'o', 't', 'e'),
    LTXT = SEAL_MKTAG('l', 't', 'x', 't'),
    SMPL = SEAL_MKTAG('s', 'm', 'p', 'l'),
    INST = SEAL_MKTAG('i', 'n', 's', 't')
};

enum io_state_t
{
    NEED_MORE_CHUNKS,
    NO_MORE_CHUNKS
};

struct wav_stream_t
{
    FILE*    file;
    uint32_t base_offset;
    uint32_t offset;
    uint32_t end_offset;
};

typedef enum io_state_t io_state_t;
typedef struct wav_stream_t wav_stream_t;

static const uint16_t PCM_CODE       = 1;
static const uint32_t CHUNK_MIN_SIZE = 4;

static
seal_err_t
read_fmt_(seal_raw_attr_t* attr, uint32_t chunk_size, FILE* wav)
{
    uint16_t compression_code = 0;
    uint16_t nchannels = 0, bit_depth = 0;

    _seal_read_uint16le(&compression_code, 1, wav);
    if (compression_code != PCM_CODE)
        return SEAL_BAD_WAV_SUBTYPE;

    _seal_read_uint16le(&nchannels, 1, wav);

    _seal_read_uint32le((uint32_t*) &attr->freq, 1, wav);
    if (attr->freq == 0)
        return FILE_BAD_WAV_FREQ;

    /* average B/s and block align are ignored. */
    _seal_skip(6, wav);

    _seal_read_uint16le(&bit_depth, 1, wav);

    attr->nchannels = nchannels;
    attr->bit_depth = bit_depth;

    /* Extra format data are ignored. */
    _seal_skip(chunk_size - 16, wav);

    return SEAL_OK;
}

static
seal_err_t
read_data(seal_raw_t* raw, uint32_t chunk_size, FILE* wav)
{
    seal_err_t err;

    if ((err = seal_alloc_raw_data(raw, chunk_size)) != SEAL_OK)
        return err;

    fread(raw->data, 1, chunk_size, wav);

    return SEAL_OK;
}

static
void
prepare_data(wav_stream_t* wav_stream, uint32_t chunk_size)
{
    wav_stream->offset = wav_stream->base_offset = ftell(wav_stream->file);
    wav_stream->end_offset = wav_stream->base_offset + chunk_size;
    _seal_skip(chunk_size, wav_stream->file);
}

static
seal_err_t
read_chunk(seal_raw_t* raw, wav_stream_t* wav_stream, FILE* wav,
           io_state_t* pstate)
{
    uint32_t chunk_id, chunk_size = 0;
    seal_err_t err = SEAL_OK;

    _seal_read_uint32le(&chunk_id, 1, wav);
    if (feof(wav)) {
        *pstate = NO_MORE_CHUNKS;
        return SEAL_OK;
    }

    _seal_read_uint32le(&chunk_size, 1, wav);
    if (chunk_size <= CHUNK_MIN_SIZE)
        return SEAL_BAD_WAV_CHUNK_SIZE;

    switch (chunk_id) {
    case FMT_:
        err = read_fmt_(&raw->attr, chunk_size, wav);
        break;
    case DATA:
        if (wav_stream != 0)
            prepare_data(wav_stream, chunk_size);
        else
            err = read_data(raw, chunk_size, wav);
        break;
    case FACT:
    case WAVL:
    case SLNT:
    case CUE_:
    case PLST:
    case LIST:
    case LABL:
    case NOTE:
    case LTXT:
    case SMPL:
    case INST:
        /* Ignores all other chunks. */
        _seal_skip(chunk_size, wav);
        break;
    default:
        return SEAL_BAD_WAV_CHUNK;
    }

    *pstate = NEED_MORE_CHUNKS;

    return err;
}

static
seal_err_t
read_chunks(seal_raw_t* raw, wav_stream_t* wav_stream, FILE* wav)
{
    io_state_t state;
    seal_err_t err;

    /* Assumes the first 12 bytes are correct. */
    _seal_skip(12, wav);
    do {
        if ((err = read_chunk(raw, wav_stream, wav, &state)) != SEAL_OK)
            return err;
    } while (state == NEED_MORE_CHUNKS);

    return SEAL_OK;
}

seal_err_t
_seal_load_wav(seal_raw_t* raw, const char* filename)
{
    FILE* wav;
    seal_raw_t tmp_raw = SEAL_RAW_INIT_LST;
    seal_err_t err;

    wav = _seal_fopen(filename);
    if (wav == 0)
        return SEAL_CANNOT_OPEN_FILE;
    err = read_chunks(&tmp_raw, 0, wav);
    _seal_fclose(wav);
    if (err != SEAL_OK)
        goto cleanup;
    if (tmp_raw.data == 0 || tmp_raw.attr.freq == 0) {
        err = SEAL_ABSENT_WAV_CHUNK;
        goto cleanup;
    }

    *raw = tmp_raw;

    return SEAL_OK;

cleanup:
    if (tmp_raw.data != 0)
        free(tmp_raw.data);

    return err;
}

seal_err_t
_seal_init_wav_stream(seal_stream_t* stream, const char* filename)
{
    wav_stream_t* wav_stream;
    seal_raw_t tmp_raw = SEAL_RAW_INIT_LST;
    seal_err_t err;

    wav_stream = malloc(sizeof (wav_stream_t));
    if (wav_stream == 0)
        return SEAL_CANNOT_ALLOC_MEM;

    wav_stream->file = _seal_fopen(filename);
    if (wav_stream->file == 0) {
        err = SEAL_CANNOT_OPEN_FILE;
        goto mem_cleanup;
    }

    err = read_chunks(&tmp_raw, wav_stream, wav_stream->file);
    if (err != SEAL_OK)
        goto cleanup;
    if (wav_stream->base_offset == wav_stream->end_offset
        || tmp_raw.attr.freq == 0) {
        err = SEAL_ABSENT_WAV_CHUNK;
        goto cleanup;
    }

    stream->id = wav_stream;
    stream->fmt = SEAL_WAV_FMT;
    stream->attr = tmp_raw.attr;
    _seal_rewind_wav_stream(stream);

    return SEAL_OK;

cleanup:
    /* For streaming, close file only on errors. */
    _seal_fclose(wav_stream->file);
mem_cleanup:
    free(wav_stream);

    return err;
}

seal_err_t
_seal_stream_wav(seal_stream_t* stream, seal_raw_t* raw, size_t* psize)
{
    void* data;
    size_t nbytes_left, nbytes = 0;
    wav_stream_t* wav_stream;

    wav_stream = stream->id;
    if (wav_stream->offset >= wav_stream->end_offset)
        goto done;

    nbytes_left = wav_stream->end_offset - wav_stream->offset;
    nbytes = nbytes_left < raw->size ? nbytes_left : raw->size;

    data = malloc(nbytes);
    if (data == 0)
        return SEAL_CANNOT_ALLOC_MEM;

    wav_stream->offset += nbytes;
    nbytes_left = fread(data, 1, nbytes, wav_stream->file);

    raw->data = data;
    raw->attr = stream->attr;
    raw->size = nbytes;
done:
    *psize = nbytes;

    return SEAL_OK;
}

seal_err_t
_seal_rewind_wav_stream(seal_stream_t* stream)
{
    wav_stream_t* wav_stream;

    wav_stream = stream->id;
    wav_stream->offset = wav_stream->base_offset;
    if (fseek(wav_stream->file, wav_stream->base_offset, SEEK_SET) != 0)
        return SEAL_CANNOT_REWIND_WAV;

    return SEAL_OK;
}

seal_err_t
_seal_close_wav_stream(seal_stream_t* stream)
{
    _seal_fclose(((wav_stream_t*) stream->id)->file);
    free(stream->id);

    return SEAL_OK;
}
