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

static int neffects_per_src = -1;

void _seal_nop() {}
void* _seal_nop_func() { return 0; }

_seal_openal_initializer_t* alGenEffects = (void*) _seal_nop;
_seal_openal_destroyer_t* alDeleteEffects = (void*) _seal_nop;
_seal_openal_validator_t* alIsEffect = (void*) _seal_nop_func;
LPALEFFECTF alEffectf = (void*) _seal_nop;
LPALEFFECTI alEffecti = (void*) _seal_nop;
LPALGETEFFECTF alGetEffectf = (void*) _seal_nop;
LPALGETEFFECTI alGetEffecti = (void*) _seal_nop;
_seal_openal_initializer_t* alGenAuxiliaryEffectSlots = (void*) _seal_nop;
_seal_openal_destroyer_t* alDeleteAuxiliaryEffectSlots = (void*) _seal_nop;
_seal_openal_validator_t* alIsAuxiliaryEffectSlot = (void*) _seal_nop_func;
LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti = (void*) _seal_nop;
LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf = (void*) _seal_nop;
LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti = (void*) _seal_nop;
LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf = (void*) _seal_nop;

const char*
seal_get_version(void)
{
    return "0.2.0";
}

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
    alGenEffects = (void*) _seal_nop;
    alDeleteEffects = (void*) _seal_nop;
    alIsEffect = (void*) _seal_nop_func;
    alEffectf = (void*) _seal_nop;
    alEffecti = (void*) _seal_nop;
    alGetEffectf = (void*) _seal_nop;
    alGetEffecti = (void*) _seal_nop;
    alGenAuxiliaryEffectSlots = (void*) _seal_nop;
    alDeleteAuxiliaryEffectSlots = (void*) _seal_nop;
    alIsAuxiliaryEffectSlot = (void*) _seal_nop_func;
    alAuxiliaryEffectSloti = (void*) _seal_nop;
    alAuxiliaryEffectSlotf = (void*) _seal_nop;
    alGetAuxiliaryEffectSloti = (void*) _seal_nop;
    alGetAuxiliaryEffectSlotf = (void*) _seal_nop;
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

unsigned int
_seal_openal_id(void* obj)
{
    /* Hack: assuming the object id is always at offset 0. */
    return *(unsigned int*) obj;
}

seal_err_t
_seal_gen_objs(int n, unsigned int* objs,
                          _seal_openal_initializer_t* generate)
{
    generate(n, objs);

    return _seal_get_openal_err();
}

seal_err_t
_seal_delete_objs(int n, const unsigned int* objs,
                  _seal_openal_destroyer_t* destroy)
{
    destroy(n, objs);

    return _seal_get_openal_err();
}

seal_err_t
_seal_init_obj(void* obj, _seal_openal_initializer_t* allocate)
{
    return _seal_gen_objs(1, obj, allocate);
}

seal_err_t
_seal_destroy_obj(void* obj, _seal_openal_destroyer_t* destroy,
                  _seal_openal_validator_t* valid)
{
    if (valid(_seal_openal_id(obj)))
        return _seal_delete_objs(1, obj, destroy);

    return SEAL_OK;
}

seal_err_t
_seal_setf(void* obj, int key, float val, _seal_openal_setterf* set)
{
    set(_seal_openal_id(obj), key, val);

    return _seal_get_openal_err();
}

seal_err_t
_seal_getf(void* obj, int key, float* pval, _seal_openal_getterf* get)
{
    get(_seal_openal_id(obj), key, pval);

    return _seal_get_openal_err();
}

seal_err_t
_seal_seti(void* obj, int key, int val, _seal_openal_setteri* set)
{
    set(_seal_openal_id(obj), key, val);

    return _seal_get_openal_err();
}

seal_err_t
_seal_geti(void* obj, int key, int* pval, _seal_openal_getteri* get)
{
    get(_seal_openal_id(obj), key, pval);

    return _seal_get_openal_err();
}

seal_err_t
_seal_getb(void* obj, int key, char* pval, _seal_openal_getteri* get)
{
    int val;
    seal_err_t err;

    err = _seal_geti(obj, key, &val, get);
    if (err == SEAL_OK)
        *pval = val;

    return err;
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
