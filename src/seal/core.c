/*
 * core.c is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 */

#include <stddef.h>
#include <malloc.h>
#include <al/al.h>
#include <al/alc.h>
#include <al/efx.h>
#include <mpg123/mpg123.h>
#include <seal/core.h>
#include <seal/mid.h>
#include <seal/err.h>
#include "threading.h"

/* Defined in err.c. */
extern _seal_tls_t _seal_err;
/* Global lock on OpenAL functions. */
static _seal_lock_t openal_lock;
/* Number of auxiliary sends per source. */
static int neffects_per_src = 1;

_seal_openal_allocator_t* alGenEffects;
_seal_openal_deallocator_t* alDeleteEffects;
_seal_openal_validator_t* alIsEffect;
LPALEFFECTF alEffectf;
LPALEFFECTI alEffecti;
LPALGETEFFECTF alGetEffectf;
LPALGETEFFECTI alGetEffecti;
_seal_openal_allocator_t* alGenAuxiliaryEffectSlots;
_seal_openal_deallocator_t* alDeleteAuxiliaryEffectSlots;
_seal_openal_validator_t* alIsAuxiliaryEffectSlot;
LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti;
LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf;
LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti;
LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf;

static int init_ext_proc(void);

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
    ALint attr[] = { ALC_MAX_AUXILIARY_SENDS, 4, 0, 0 };

    _seal_err = _seal_alloc_tls();
    _seal_set_tls(_seal_err, (void*) SEAL_OK);

    device = alcOpenDevice(device_name);
    SEAL_CHK(device != 0, SEAL_CANNOT_OPEN_DEVICE, 0);
    SEAL_CHK_S(alcIsExtensionPresent(device, ALC_EXT_EFX_NAME),
               SEAL_NO_EFX, clean_device);

    context = alcCreateContext(device, attr);
    switch (alcGetError(device)) {
    case ALC_INVALID_VALUE:
        SEAL_ABORT_S(SEAL_CANNOT_CREATE_CONTEXT, clean_device);
    case ALC_INVALID_DEVICE:
        SEAL_ABORT_S(SEAL_BAD_DEVICE, clean_device);
    }
    alcMakeContextCurrent(context);

    if (init_ext_proc() == 0)
        goto clean_all;

    /* `mpg123_init' is thread-unsafe. */
    SEAL_CHK_S(mpg123_init() == MPG123_OK && seal_midi_startup() != 0,
               SEAL_CANNOT_INIT_MPG, clean_all);

    /* Reset OpenAL's error state. */
    alGetError();
    alcGetError(device);

    alcGetIntegerv(device, ALC_MAX_AUXILIARY_SENDS, 1, &neffects_per_src);

    openal_lock = _seal_create_lock();

    return 1;

clean_all:
    alcMakeContextCurrent(0);
    alcDestroyContext(context);
clean_device:
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
    _seal_destroy_lock(openal_lock);

    mpg123_exit();

    context = alcGetCurrentContext();
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(0);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

int
seal_get_neffects_per_src(void)
{
    return neffects_per_src;
}

#if defined (__unix__)

#include <unistd.h>

void
_seal_sleep(unsigned int millisec)
{
    usleep(millisec * 1000);
}

#elif defined (_WIN32)

#include <Windows.h>

void
_seal_sleep(unsigned int millisec)
{
    SleepEx(millisec, 0);
}

void
_seal_lock_openal(void)
{
    _seal_lock(openal_lock);
}

void
_seal_unlock_openal(void)
{
    _seal_unlock(openal_lock);
}

void*
_seal_malloc(size_t size)
{
    void* mem;

    mem = malloc(size);
    SEAL_CHK(mem != 0, SEAL_CANNOT_ALLOC_MEM, 0);

    return mem;
}

void*
_seal_calloc(size_t count, size_t size)
{
    void* mem;

    mem = calloc(count, size);
    SEAL_CHK(mem != 0, SEAL_CANNOT_ALLOC_MEM, 0);

    return mem;
}

void*
_seal_realloc(void* mem, size_t size)
{
    mem = realloc(mem, size);
    SEAL_CHK(mem != 0, SEAL_CANNOT_ALLOC_MEM, 0);

    return mem;
}

void
_seal_free(void* mem)
{
    free(mem);
}

void*
_seal_alloc_obj(size_t size, _seal_openal_allocator_t* allocate)
{
    void* obj;

    obj = _seal_malloc(size);
    if (obj == 0)
        return 0;

    _seal_lock_openal();
    /* Hack: assuming the id is always at offset 0. */
    allocate(1, (unsigned int*) obj);
    if (_seal_chk_openal_err() == 0)
        goto cleanup;

    return obj;

cleanup:
    _seal_free(obj);

    return 0;
}

int
_seal_free_obj(void* obj, _seal_openal_deallocator_t* deallocate,
               _seal_openal_validator_t* validate)
{
    /* Hack: assuming the id is always at offset 0. */
    if (validate(*(unsigned int*) obj)) {
        _seal_lock_openal();
        deallocate(1, obj);
        if (_seal_chk_openal_err() == 0)
            return 0;
    }

    _seal_free(obj);

    return 1;
}

static int
init_ext_proc(void)
{
    alGenEffects = alGetProcAddress("alGenEffects");
    alDeleteEffects = alGetProcAddress("alDeleteEffects");
    alIsEffect = alGetProcAddress("alIsEffect");
    alEffectf = alGetProcAddress("alEffectf");
    alEffecti = alGetProcAddress("alEffecti");
    alGetEffectf = alGetProcAddress("alGetEffectf");
    alGetEffecti = alGetProcAddress("alGetEffecti");
    alGenAuxiliaryEffectSlots = alGetProcAddress("alGenAuxiliaryEffectSlots");
    alDeleteAuxiliaryEffectSlots =
        alGetProcAddress("alDeleteAuxiliaryEffectSlots");
    alIsAuxiliaryEffectSlot = alGetProcAddress("alIsAuxiliaryEffectSlot");
    alAuxiliaryEffectSloti = alGetProcAddress("alAuxiliaryEffectSloti");
    alAuxiliaryEffectSlotf = alGetProcAddress("alAuxiliaryEffectSlotf");
    alGetAuxiliaryEffectSloti = alGetProcAddress("alGetAuxiliaryEffectSloti");
    alGetAuxiliaryEffectSlotf = alGetProcAddress("alGetAuxiliaryEffectSlotf");
    SEAL_CHK(alGenEffects && alDeleteEffects && alIsEffect && alGetEffectf
             && alGetEffecti && alEffectf && alGenAuxiliaryEffectSlots
             && alDeleteAuxiliaryEffectSlots && alIsAuxiliaryEffectSlot
             && alAuxiliaryEffectSloti && alAuxiliaryEffectSlotf
             && alGetAuxiliaryEffectSloti && alGetAuxiliaryEffectSlotf,
             SEAL_NO_EXT_FUNC, 0);

    return 1;
}

#endif /* __unix__, _WIN32 */
