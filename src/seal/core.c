/*
 * core.c is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 */

#include <stddef.h>
#include <malloc.h>
#include <al/al.h>
#include <al/alc.h>
#include <mpg123/mpg123.h>
#include <seal/core.h>
#include <seal/threading.h>
#include <seal/mid.h>
#include <seal/err.h>

/* Defined in err.c. */
extern _seal_tls_t _seal_err;
/* Global lock on OpenAL functions. */
static _seal_lock_t al_lock;

const char*
seal_get_verion(void)
{
    return "0.2.0";
}

/*
 * Initializes the specified device and creates a single context. SEAL
 * currently does not make use of multiple contexts.
 */
int
seal_startup(const char* device_name)
{
    ALCdevice* device;
    ALCcontext* context;

    device = alcOpenDevice(device_name);
    SEAL_CHK(device != 0, SEAL_OPEN_DEVICE_FAILED, 0);

    context = alcCreateContext(device, 0);
    switch (alcGetError(device)) {
    case ALC_INVALID_VALUE:
        SEAL_ABORT_S(SEAL_CREATE_CONTEXT_FAILED, cleanup);
    case ALC_INVALID_DEVICE:
        SEAL_ABORT_S(SEAL_BAD_DEVICE, cleanup);
    }

    alcMakeContextCurrent(context);

    /* Reset OpenAL's error state. */
    alGetError();

    /* `mpg123_init' is thread-unsafe. */
    SEAL_CHK_S(mpg123_init() == MPG123_OK && seal_midi_startup() != 0,
               SEAL_INIT_MPG_FAILED, cleanup);

    al_lock = _seal_create_lock();
    _seal_err = _seal_alloc_tls();
    _seal_set_tls(_seal_err, (void*) SEAL_OK);

    return 1;

cleanup:
    alcCloseDevice(device);

    return 0;
}

/* Finalizes the current device and context. */
void
seal_cleanup(void)
{
    ALCdevice* device;
    ALCcontext* context;

    _seal_free_tls(_seal_err);
    _seal_destroy_lock(al_lock);

    mpg123_exit();
    seal_midi_cleanup();

    context = alcGetCurrentContext();
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(0);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

void
_seal_lock_al(void)
{
    _seal_lock(al_lock);
}

void
_seal_unlock_al(void)
{
    _seal_unlock(al_lock);
}

void*
_seal_malloc(size_t size)
{
    void* mem;

    mem = malloc(size);
    SEAL_CHK(mem != 0, SEAL_MEM_ALLOC_FAILED, 0);

    return mem;
}

void*
_seal_calloc(size_t count, size_t size)
{
    void* mem;

    mem = calloc(count, size);
    SEAL_CHK(mem != 0, SEAL_MEM_ALLOC_FAILED, 0);

    return mem;
}

void*
_seal_realloc(void* mem, size_t size)
{
    mem = realloc(mem, size);
    SEAL_CHK(mem != 0, SEAL_MEM_ALLOC_FAILED, 0);

    return mem;
}

void
_seal_free(void* mem)
{
    free(mem);
}
