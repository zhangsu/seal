/*
 * src.c is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 */

#include <string.h>
#include <stddef.h>
#include <al/al.h>
#include <seal/src.h>
#include <seal/core.h>
#include <seal/buf.h>
#include <seal/stream.h>
#include <seal/threading.h>
#include <seal/err.h>
#include <assert.h>

struct seal_src_t
{
    size_t         chunk_size  : 24;
    size_t         queue_size  : 6;
    unsigned int   looping     : 1;
    unsigned int   auto_update : 1;
    unsigned int   id;
    seal_buf_t*    buf;
    seal_stream_t* stream;
    _seal_thread_t updater;
};

enum
{
    /* 2^24=16777216. */
    CHUNK_STORAGE_CAP = 16777216,
    /* LCM(36, 1024)=9216. */
    MIN_CHUNK_SIZE = 9216
};

static const size_t MIN_QUEUE_SIZE     = 2;
static const size_t MAX_QUEUE_SIZE     = 63;
static const size_t DEFAULT_QUEUE_SIZE = 3;
static const size_t DEFAULT_CHUNK_SIZE = MIN_CHUNK_SIZE << 2;
static const size_t MAX_CHUNK_SIZE     = CHUNK_STORAGE_CAP -
                                         CHUNK_STORAGE_CAP % MIN_CHUNK_SIZE;

/* Getters and setters without error checking. */
static void set3f(seal_src_t*, int, float, float, float);
static void get3f(seal_src_t*, int, float*, float*, float*);
static int geti(seal_src_t*, int);
static float getf(seal_src_t*, int);
/* Thread-safe setters with error checking. */
static int seti_s(seal_src_t*, int, int);
static int setf_s(seal_src_t*, int, float);
/* Queue operations. */
static void clean_queue(seal_src_t*);
static void stop_then_clean_queue(seal_src_t*);
static void restart_queuing(seal_src_t*);
static void empty_queue(seal_src_t*);
static void ensure_queue_empty(seal_src_t*);
static void ensure_stream_released(seal_src_t*);
static void wait4updater(seal_src_t*);
static _seal_routine update;

seal_src_t*
seal_alloc_src(void)
{
    seal_src_t* src = _seal_calloc(1, sizeof (seal_src_t));
    if (src == 0)
        return 0;

    _seal_lock_al();
    alGenSources(1, &src->id);
    SEAL_CHK_AL2_S(AL_OUT_OF_MEMORY, SEAL_ALLOC_SRC_FAILED,
                   AL_INVALID_VALUE, SEAL_ALLOC_SRC_FAILED, cleanup);

    src->queue_size = DEFAULT_QUEUE_SIZE;
    src->chunk_size = DEFAULT_CHUNK_SIZE;
    src->auto_update = 1;

    return src;

cleanup:
    _seal_free(src);

    return 0;
}

void
seal_free_src(seal_src_t* src)
{
    assert(src != 0);

    if (alIsSource(src->id)) {
        ensure_queue_empty(src);
        alDeleteSources(1, &src->id);
    }
    ensure_stream_released(src);
    /* Wait before making `src' a dangling pointer. */
    wait4updater(src);
    _seal_free(src);
}

int
seal_play_src(seal_src_t* src)
{
    assert(src != 0 && alIsSource(src->id));

    if (src->stream != 0) {
        seal_src_state_t state = seal_get_src_state(src);
        if (state == SEAL_PLAYING) {
            /* Source and its updater will be stopped after this. */
            restart_queuing(src);
        } else {
            /* In case the old updater is not done. */
            wait4updater(src);
        }
        /* Stream some data so plackback can start immediately. */
        if (seal_update_src(src) < 0)
            return 0;
        if (src->auto_update)
            src->updater = _seal_create_thread(update, src);
    }
    alSourcePlay(src->id);

    return 1;
}

void
seal_pause_src(seal_src_t* src)
{
    assert(src != 0 && alIsSource(src->id));

    alSourcePause(src->id);
}

void
seal_stop_src(seal_src_t* src)
{
    assert(src != 0 && alIsSource(src->id));

    alSourceStop(src->id);
    if (src->stream != 0) {
        /* Already stopped so all buffers are proccessed. */
        clean_queue(src);
        seal_rewind_stream(src->stream);
    }
}

void
seal_rewind_src(seal_src_t* src)
{
    assert(src != 0 && alIsSource(src->id));

    if (src->stream != 0) {
        seal_src_state_t state = seal_get_src_state(src);
        if (state == SEAL_PLAYING || state == SEAL_PAUSED)
            restart_queuing(src);
    }
    alSourceRewind(src->id);
}

void
seal_detach_src_audio(seal_src_t* src)
{
    assert(src != 0 && alIsSource(src->id));

    alSourcePlay(src->id);
    alSourceStop(src->id);
    if (src->stream != 0)
        clean_queue(src);
    /* Sets the state to `SEAL_INITIAL' for consistency. */
    alSourceRewind(src->id);

    ensure_stream_released(src);
    seti_s(src, AL_BUFFER, AL_NONE);
    src->buf = 0;
    /* Wait before nullifying stream pointer. */
    wait4updater(src);
    src->stream = 0;
}

int
seal_set_src_buf(seal_src_t* src, seal_buf_t* buf)
{
    assert(src != 0 && alIsSource(src->id) && buf != 0);

    /* Make sure `src' is not currently a streaming source. */
    SEAL_CHK(src->stream == 0, SEAL_MIXING_SRC_TYPE, 0);

    if (seti_s(src, AL_BUFFER, _seal_get_buf_id(buf)) == 0)
        return 0;
    /* Carry the previous looping state over for static sources. */
    alSourcei(src->id, AL_LOOPING, src->looping);
     
    src->buf = buf;

    return 1;
}

int
seal_set_src_stream(seal_src_t* src, seal_stream_t* stream)
{
    assert(src != 0 && alIsSource(src->id) && stream != 0);

    if (stream == src->stream)
        return 1;
    /* Make sure `src' is not currently a static source. */
    SEAL_CHK(src->buf == 0, SEAL_MIXING_SRC_TYPE, 0);
    /* Cannot associate an unopened stream. */
    SEAL_CHK(stream->id != 0, SEAL_STREAM_UNOPENED, 0);
    SEAL_CHK(!stream->in_use, SEAL_STREAM_INUSE, 0);
    /* Cannot associate a stream with a different audio format. */
    if (src->stream != 0) {
        SEAL_CHK(memcmp(&stream->attr, &src->stream->attr,
                        sizeof (seal_raw_attr_t)) == 0,
                 SEAL_MIXING_STREAM_FMT, 0);
    }

    ensure_stream_released(src);
    /* Never use AL_LOOPING for streaming sources. */
    alSourcei(src->id, AL_LOOPING, 0);
    src->stream = stream;
    stream->in_use = 1;

    /* Immediately update the queue to become `AL_STREAMING'. */
    return seal_update_src(src) >= 0;
}

int
seal_update_src(seal_src_t* src)
{
    unsigned int buf;
    seal_raw_t raw;
    int nbytes_streamed;
    int updater_is_working;

    assert(src != 0 && alIsSource(src->id));

    if (src->stream == 0)
        return 0;

    updater_is_working = src->updater != 0
                         && !_seal_calling_thread_is(src->updater);
    if (updater_is_working)
        return 1;

    raw.size = src->chunk_size;

    for (;;) {
        unsigned int nbufs_queued, nbufs_processed;

        alGetSourcei(src->id, AL_BUFFERS_QUEUED, &nbufs_queued);
        alGetSourcei(src->id, AL_BUFFERS_PROCESSED, &nbufs_processed);

        if (nbufs_processed > 0) {
            alSourceUnqueueBuffers(src->id, 1, &buf);
            /* Queue is too long. */
            if (nbufs_queued > src->queue_size) {
                /* Reduce the size of queue. */
                alDeleteBuffers(1, &buf);
                continue;
            }
        /* Queue is too short. */
        } else if (nbufs_queued < src->queue_size) {
            /* Generate new buffers. */
            _seal_lock_al();
            alGenBuffers(1, &buf);
            SEAL_CHK_AL(AL_OUT_OF_MEMORY, SEAL_ALLOC_BUF_FAILED, -1);
        /* The queue is up-to-date. */
        } else {
            return 1;
        }

start_streaming:
        nbytes_streamed = seal_stream(src->stream, &raw);
        if (nbytes_streamed > 0) {
            int buffer_filled;
            /* Fill or refill the current buffer. */
            _seal_lock_al();
            alBufferData(buf, _seal_get_buf_fmt(raw.attr.nchannels,
                                                raw.attr.bit_depth),
                         raw.data, raw.size, raw.attr.freq);
            buffer_filled = _seal_get_al_err() == AL_NO_ERROR;
            _seal_free(raw.data);
            if (buffer_filled) {
                alSourceQueueBuffers(src->id, 1, &buf);
            } else {
                _seal_set_err(SEAL_MEM_ALLOC_FAILED);
                break;
            }
        /* End of stream reached. */
        } else if (nbytes_streamed == 0) {
            /* Rewind the stream if looping. */
            if (src->looping) {
                seal_rewind_stream(src->stream);
                raw.size = src->chunk_size;
                goto start_streaming;
            } else {
                break;
            }
        } else {
            break;
        }
    } /* for (;;) */

    alDeleteBuffers(1, &buf);

    return nbytes_streamed;
}

int
seal_set_src_queue_size(seal_src_t* src, size_t size)
{
    assert(src != 0 && alIsSource(src->id));

    SEAL_CHK(MIN_QUEUE_SIZE <= size && size <= MAX_QUEUE_SIZE,
             SEAL_BAD_SRC_ATTR_VAL, 0);

    src->queue_size = size;

    return 1;
}

int
seal_set_src_chunk_size(seal_src_t* src, size_t size)
{
    assert(src != 0 && alIsSource(src->id));

    SEAL_CHK(MIN_CHUNK_SIZE <= size && size <= MAX_CHUNK_SIZE
             && size % MIN_CHUNK_SIZE == 0, SEAL_BAD_SRC_ATTR_VAL, 0);

    src->chunk_size = size;

    return 1;
}

void
seal_set_src_pos(seal_src_t* src, float x, float y, float z)
{
    set3f(src, AL_POSITION, x, y, z);
}

void
seal_set_src_vel(seal_src_t* src, float x, float y, float z)
{
    set3f(src, AL_VELOCITY, x, y, z);
}

int
seal_set_src_pitch(seal_src_t* src, float pitch)
{
    return setf_s(src, AL_PITCH, pitch);
}

int
seal_set_src_gain(seal_src_t* src, float gain)
{
    return setf_s(src, AL_GAIN, gain);
}

int
seal_set_src_auto_update(seal_src_t* src, int auto_update)
{
    SEAL_CHK(auto_update == 0 || auto_update == 1, SEAL_BAD_SRC_ATTR_VAL, 0);

    src->auto_update = auto_update;

    return 1;
}

int
seal_set_src_relative(seal_src_t* src, int relative)
{
    return seti_s(src, AL_SOURCE_RELATIVE, relative);
}

int
seal_set_src_looping(seal_src_t* src, int looping)
{
    SEAL_CHK(looping == 0 || looping == 1, SEAL_BAD_SRC_ATTR_VAL, 0);

    src->looping = looping;
    /*
     * Streaming does not work with OpenAL's looping as the queuing buffers
     * will never become `processed' when `AL_LOOPING' is true, so set
     * `AL_LOOPING' only for static sources.
     */
    if (src->stream == 0)
        alSourcei(src->id, AL_LOOPING, looping);

    return 1;
}

seal_buf_t*
seal_get_src_buf(seal_src_t* src)
{
    assert(src != 0);

    return src->buf;
}

seal_stream_t* seal_get_src_stream(seal_src_t* src)
{
    assert(src != 0);

    return src->stream;
}

size_t
seal_get_src_queue_size(seal_src_t* src)
{
    assert(src != 0);

    return src->queue_size;
}

size_t
seal_get_src_chunk_size(seal_src_t* src)
{
    assert(src != 0);

    return src->chunk_size;
}

void
seal_get_src_pos(seal_src_t* src, float* x, float* y, float* z)
{
    get3f(src, AL_POSITION, x, y, z);
}

void
seal_get_src_vel(seal_src_t* src, float* x, float* y, float* z)
{
    get3f(src, AL_VELOCITY, x, y, z);
}

float
seal_get_src_pitch(seal_src_t* src)
{
    return getf(src, AL_PITCH);
}

float
seal_get_src_gain(seal_src_t* src)
{
    return getf(src, AL_GAIN);
}


int
seal_is_src_auto_updated(seal_src_t* src)
{
    return src->auto_update;
}

int
seal_is_src_relative(seal_src_t* src)
{
    return geti(src, AL_SOURCE_RELATIVE);
}

int
seal_is_src_looping(seal_src_t* src)
{
    return src->looping;
}

seal_src_type_t
seal_get_src_type(seal_src_t* src)
{
    switch (geti(src, AL_SOURCE_TYPE)) {
    case AL_STATIC:
        return SEAL_STATIC;
    case AL_STREAMING:
        return SEAL_STREAMING;
    default:
        return SEAL_UNDETERMINED;
    }
}

seal_src_state_t
seal_get_src_state(seal_src_t* src)
{
    switch (geti(src, AL_SOURCE_STATE)) {
    case AL_PLAYING:
        return SEAL_PLAYING;
    case AL_PAUSED:
        return SEAL_PAUSED;
    case AL_STOPPED:
        return SEAL_STOPPED;
    default:
        return SEAL_INITIAL;
    }
}

void
set3f(seal_src_t* src, int attr, float x, float y, float z)
{
    assert(src != 0 && alIsSource(src->id));

    alSource3f(src->id, attr, x, y, z);
}

void
get3f(seal_src_t* src, int attr, float* x, float* y, float* z)
{
    assert(src != 0 && alIsSource(src->id));

    alGetSource3f(src->id, attr, x, y, z);
}

int
geti(seal_src_t* src, int attr)
{
    int i;

    assert(src != 0 && alIsSource(src->id));

    alGetSourcei(src->id, attr, &i);

    return i;
}

float getf(seal_src_t* src, int attr)
{
    float f;

    assert(src != 0 && alIsSource(src->id));
    
    alGetSourcef(src->id, attr, &f);

    return f;
}

int
seti_s(seal_src_t* src, int attr, int i)
{
    assert(alIsSource(src->id));

    _seal_lock_al();
    alSourcei(src->id, attr, i);
    SEAL_CHK_AL2(AL_INVALID_VALUE, SEAL_BAD_SRC_ATTR_VAL,
                 AL_INVALID_OPERATION, SEAL_BAD_SRC_OP, 0);

    return 1;
}

int
setf_s(seal_src_t* src, int attr, float f)
{
    assert(alIsSource(src->id));

    _seal_lock_al();
    alSourcef(src->id, attr, f);
    SEAL_CHK_AL2(AL_INVALID_VALUE, SEAL_BAD_SRC_ATTR_VAL,
                 AL_INVALID_OPERATION, SEAL_BAD_SRC_OP, 0);

    return 1;
}

void
clean_queue(seal_src_t* src)
{
    int nbufs;
    alGetSourcei(src->id, AL_BUFFERS_PROCESSED, &nbufs);
    while (--nbufs >= 0) {
        unsigned int buf;
        alSourceUnqueueBuffers(src->id, 1, &buf);
        alDeleteBuffers(1, &buf);
    }
}

/*
 * Stopping a source will mark all the buffers in its queue processed so that
 * they can be unqueued.
 */
void
stop_then_clean_queue(seal_src_t* src)
{
    alSourceStop(src->id);
    /* Do not let the updater touch anything when cleaning the queue. */
    wait4updater(src);
    clean_queue(src);
}

void
restart_queuing(seal_src_t* src)
{
    stop_then_clean_queue(src);
    seal_rewind_stream(src->stream);
}

void
empty_queue(seal_src_t* src)
{
    /* Need to be playing first in order to become stopped. */
    alSourcePlay(src->id);
    stop_then_clean_queue(src);
}

void
ensure_queue_empty(seal_src_t* src)
{
    if (src->stream != 0)
        empty_queue(src);
}


void
ensure_stream_released(seal_src_t* src)
{
    if (src->stream != 0)
        src->stream->in_use = 0;
}

/* The updater thread routine. */
void*
update(void* args)
{
    seal_src_t* src = args;

    while (alIsSource(src->id) && seal_get_src_state(src) == SEAL_PLAYING) {
        if (seal_update_src(src) < 0)
            return 0;
        _seal_sleep(50);
    }

    return (void*) 1;
}

void
wait4updater(seal_src_t* src)
{
    if (src->updater != 0) {
        _seal_join_thread(src->updater);
        src->updater = 0;
    }
}