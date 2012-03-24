/*
 * core.c is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 */

#include <stdlib.h>
#include <stddef.h>
#include <al/al.h>
#include <al/alc.h>
#include <al/efx.h>
#include <mpg123/mpg123.h>
#include <seal/core.h>
#include <seal/err.h>
#include "threading.h"

/* Global lock on OpenAL functions. */
static void* openal_lock;
static int neffects_per_src = -1;

_seal_openal_initializer_t* alGenEffects;
_seal_openal_destroyer_t* alDeleteEffects;
_seal_openal_validator_t* alIsEffect;
LPALEFFECTF alEffectf;
LPALEFFECTI alEffecti;
LPALGETEFFECTF alGetEffectf;
LPALGETEFFECTI alGetEffecti;
_seal_openal_initializer_t* alGenAuxiliaryEffectSlots;
_seal_openal_destroyer_t* alDeleteAuxiliaryEffectSlots;
_seal_openal_validator_t* alIsAuxiliaryEffectSlot;
LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti;
LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf;
LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti;
LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf;

static seal_err_t
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
    if (alGenEffects && alDeleteEffects && alIsEffect && alGetEffectf
        && alGetEffecti && alEffectf && alGenAuxiliaryEffectSlots
        && alDeleteAuxiliaryEffectSlots && alIsAuxiliaryEffectSlot
        && alAuxiliaryEffectSloti && alAuxiliaryEffectSlotf
        && alGetAuxiliaryEffectSloti && alGetAuxiliaryEffectSlotf)
        return SEAL_OK;
    else
        return SEAL_NO_EXT_FUNC;
}

static void
reset_ext_proc(void)
{
    alIsEffect = alIsAuxiliaryEffectSlot = 0;
}

const char*
seal_get_version(void)
{
    return "0.2.0";
}

/*
 * Initializes the specified device and creates a single context. SEAL
 * currently does not make use of multiple contexts.
 */
seal_err_t
seal_startup(const char* device_name)
{
    ALCdevice* device;
    ALCcontext* context;
    seal_err_t err;
    ALint attr[] = { ALC_MAX_AUXILIARY_SENDS, 4, 0, 0 };

    /* Initialize device. */
    device = alcOpenDevice(device_name);
    if (device == 0)
        return SEAL_CANNOT_OPEN_DEVICE;
    alcGetError(device);

    /* Initialize extensions. */
    if (!alcIsExtensionPresent(device, ALC_EXT_EFX_NAME)) {
        err = SEAL_NO_EFX;
        goto clean_device;
    }

    /* Initialize context. */
    context = alcCreateContext(device, attr);
    switch (alcGetError(device)) {
    case ALC_INVALID_VALUE:
        err = SEAL_CANNOT_CREATE_CONTEXT;
        goto clean_device;
    case ALC_INVALID_DEVICE:
        err = SEAL_BAD_DEVICE;
        goto clean_device;
    }
    alcMakeContextCurrent(context);

    err = init_ext_proc();
    if (err != SEAL_OK)
        goto clean_all;

    /* Initialize libmpg123 (thread-unsafe). */
    if (mpg123_init() != MPG123_OK) {
        err = SEAL_CANNOT_INIT_MPG;
        goto clean_all;
    }

    /* Reset OpenAL's error state. */
    alGetError();

    alcGetIntegerv(device, ALC_MAX_AUXILIARY_SENDS, 1, &neffects_per_src);

    openal_lock = _seal_create_lock();

    return SEAL_OK;

clean_all:
    alcMakeContextCurrent(0);
    alcDestroyContext(context);
clean_device:
    alcCloseDevice(device);

    return err;
}

/* Finalizes the current device and context. */
void
seal_cleanup(void)
{
    ALCdevice* device;
    ALCcontext* context;

    if (openal_lock == 0)
        return;

    _seal_destroy_lock(openal_lock);
    openal_lock = 0;

    mpg123_exit();

    context = alcGetCurrentContext();
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(0);
    alcDestroyContext(context);
    alcCloseDevice(device);

    reset_ext_proc();
}

int
seal_get_neffects_per_src(void)
{
    return neffects_per_src;
}


seal_err_t _seal_gen_objs(int n, unsigned int* objs,
                          _seal_openal_initializer_t* generate)
{
    _seal_lock_openal();
    generate(n, objs);

    return _seal_get_openal_err();
}

seal_err_t
_seal_delete_objs(int n, const unsigned int* objs,
                  _seal_openal_destroyer_t* destroy)
{
    _seal_lock_openal();
    destroy(n, objs);

    return _seal_get_openal_err();
}

seal_err_t
_seal_init_obj(void* obj, _seal_openal_initializer_t* allocate)
{
    /* Hack: assuming the object id is always at offset 0. */
    return _seal_gen_objs(1, obj, allocate);
}

seal_err_t
_seal_destroy_obj(void* obj, _seal_openal_destroyer_t* destroy,
                  _seal_openal_validator_t* validate)
{
    /* Hack: assuming the object id is always at offset 0. */
    if (validate(*(unsigned int*) obj))
        return _seal_delete_objs(1, obj, destroy);

    return SEAL_OK;
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

#endif /* __unix__, _WIN32 */

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

