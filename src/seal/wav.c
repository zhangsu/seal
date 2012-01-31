/*
 * wav.c is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 */

#include <stdio.h>
#include <stddef.h>
#include <seal/wav.h>
#include <seal/core.h>
#include <seal/raw.h>
#include <seal/stream.h>
#include <seal/reader.h>
#include <seal/err.h>

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

enum reading_state_t
{
    NEED_MORE_CHUNKS,
    NO_MORE_CHUNKS,
    ERR_READING_CHUNKS
};

struct wav_stream_t
{
    FILE*    file;
    uint32_t base_offset;
    uint32_t offset;
    uint32_t end_offset;
};

typedef enum reading_state_t reading_state_t;
typedef struct wav_stream_t wav_stream_t;

static const uint16_t PCM_CODE         = 1;
static const uint32_t CHUNK_MIN_SIZE   = 4;

static reading_state_t read_chunks(seal_raw_t*, wav_stream_t*, FILE*);
static reading_state_t read_chunk(seal_raw_t*, wav_stream_t*, FILE*);
static reading_state_t read_fmt_(seal_raw_attr_t*, uint32_t, FILE*);
static reading_state_t read_data(seal_raw_t*, uint32_t, FILE*);
static void prepare_data(wav_stream_t*, uint32_t);

int
_seal_load_wav(seal_raw_t* raw, const char* filename)
{
    FILE* wav;
    reading_state_t state;
    seal_raw_t tmp_raw = SEAL_RAW_INIT_LST;

    wav = _seal_fopen(filename);
    if (wav == 0)
        return 0;
    state = read_chunks(&tmp_raw, 0, wav);
    _seal_fclose(wav);
    if (state == ERR_READING_CHUNKS)
        goto cleanup;
    SEAL_CHK_S(tmp_raw.data != 0 && tmp_raw.attr.freq != 0,
               SEAL_ABSENT_WAV_CHUNK, cleanup);

    *raw = tmp_raw;

    return 1;

cleanup:
    if (tmp_raw.data != 0)
        _seal_free(tmp_raw.data);

    return 0;
}

seal_stream_t*
_seal_init_wav_stream(seal_stream_t* stream, const char* filename)
{
    wav_stream_t* wav_stream;
    reading_state_t state;
    seal_raw_t tmp_raw = SEAL_RAW_INIT_LST;

    wav_stream = _seal_malloc(sizeof (wav_stream_t));
    if (wav_stream == 0)
        return 0;

    wav_stream->file = _seal_fopen(filename);
    if (wav_stream->file == 0)
        goto mem_cleanup;

    state = read_chunks(&tmp_raw, wav_stream, wav_stream->file);
    if (state == ERR_READING_CHUNKS)
        goto cleanup;
    SEAL_CHK_S(wav_stream->base_offset != wav_stream->end_offset
               && tmp_raw.attr.freq != 0, SEAL_ABSENT_WAV_CHUNK, cleanup);

    stream->id = wav_stream;
    stream->fmt = SEAL_WAV_FMT;
    stream->attr = tmp_raw.attr;
    _seal_rewind_wav_stream(stream);

    return stream;

cleanup:
    /* For streaming, close file only on errors. */
    _seal_fclose(wav_stream->file);
mem_cleanup:
    _seal_free(wav_stream);

    return 0;
}

int
_seal_stream_wav(seal_stream_t* stream, seal_raw_t* raw)
{
    void* data;
    size_t nbytes_left, nbytes;
    wav_stream_t* wav_stream;

    wav_stream = stream->id;
    if (wav_stream->offset >= wav_stream->end_offset)
        return 0;

    nbytes_left = wav_stream->end_offset - wav_stream->offset;
    nbytes = nbytes_left < raw->size ? nbytes_left : raw->size;

    data = _seal_malloc(nbytes);
    if (data == 0)
        return -1;

    wav_stream->offset += nbytes;
    nbytes_left = fread(data, 1, nbytes, wav_stream->file);

    raw->data = data;
    raw->attr = stream->attr;
    raw->size = nbytes;

    return nbytes;
}

void
_seal_rewind_wav_stream(seal_stream_t* stream)
{
    wav_stream_t* wav_stream;

    wav_stream = stream->id;
    wav_stream->offset = wav_stream->base_offset;
    fseek(wav_stream->file, wav_stream->base_offset, SEEK_SET);
}

void
_seal_close_wav_stream(seal_stream_t* stream)
{
    _seal_fclose(((wav_stream_t*) stream->id)->file);
    _seal_free(stream->id);
}

reading_state_t
read_chunks(seal_raw_t* raw, wav_stream_t* wav_stream, FILE* wav)
{
    reading_state_t state;

    /* Assumes the first 12 bytes are correct. */
    _seal_skip(12, wav);
    do state = read_chunk(raw, wav_stream, wav);
    while (state == NEED_MORE_CHUNKS);

    return state;
}

reading_state_t
read_chunk(seal_raw_t* raw, wav_stream_t* wav_stream, FILE* wav)
{
    uint32_t chunk_id, chunk_size = 0;

    _seal_read_uint32le(&chunk_id, 1, wav);
    if (feof(wav))
        return NO_MORE_CHUNKS;

    _seal_read_uint32le(&chunk_size, 1, wav);
    SEAL_CHK(chunk_size > CHUNK_MIN_SIZE, SEAL_BAD_WAV_CHUNK_SIZE,
             ERR_READING_CHUNKS);

    switch (chunk_id) {
    case FMT_:
        return read_fmt_(&raw->attr, chunk_size, wav);
    case DATA:
        if (wav_stream != 0)
            prepare_data(wav_stream, chunk_size);
        else
            return read_data(raw, chunk_size, wav);
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
        SEAL_ABORT(SEAL_BAD_WAV_CHUNK, ERR_READING_CHUNKS);
    }

    return NEED_MORE_CHUNKS;
}

reading_state_t
read_fmt_(seal_raw_attr_t* attr, uint32_t chunk_size, FILE* wav)
{
    uint16_t compression_code = 0;
    uint16_t nchannels = 0, bit_depth = 0;

    _seal_read_uint16le(&compression_code, 1, wav);
    SEAL_CHK(compression_code == PCM_CODE, SEAL_BAD_WAV_SUBTYPE,
             ERR_READING_CHUNKS);

    _seal_read_uint16le(&nchannels, 1, wav);

    _seal_read_uint32le((uint32_t*) &attr->freq, 1, wav);
    SEAL_CHK(attr->freq != 0, FILE_BAD_WAV_FREQ, ERR_READING_CHUNKS);

    /* average B/s and block align are ignored. */
    _seal_skip(6, wav);

    _seal_read_uint16le(&bit_depth, 1, wav);

    attr->nchannels = nchannels;
    attr->bit_depth = bit_depth;

    /* Extra format data are ignored. */
    _seal_skip(chunk_size - 16, wav);

    return NEED_MORE_CHUNKS;
}

reading_state_t
read_data(seal_raw_t* raw, uint32_t chunk_size, FILE* wav)
{
    if (seal_alloc_raw_data(raw, chunk_size) == 0)
        return ERR_READING_CHUNKS;

    fread(raw->data, 1, chunk_size, wav);

    return NEED_MORE_CHUNKS;
}

void
prepare_data(wav_stream_t* wav_stream, uint32_t chunk_size)
{
    wav_stream->offset = wav_stream->base_offset = ftell(wav_stream->file);
    wav_stream->end_offset = wav_stream->base_offset + chunk_size;
    _seal_skip(chunk_size, wav_stream->file);
}
