#include <assert.h>
#include <al/al.h>
#include <al/efx.h>
#include <seal/reverb.h>
#include <seal/core.h>
#include <seal/err.h>

seal_err_t
seal_init_reverb(seal_reverb_t* reverb)
{
    seal_err_t err;

    if ((err = _seal_init_obj(reverb, alGenEffects)) != SEAL_OK)
        return err;

    alEffecti(reverb->id, AL_EFFECT_TYPE, AL_EFFECT_REVERB);

    return _seal_get_openal_err();
}

seal_err_t
seal_destroy_reverb(seal_reverb_t* reverb)
{
    return _seal_destroy_obj(reverb, alDeleteEffects, alIsEffect);
}

seal_err_t
seal_set_reverb_density(seal_reverb_t* reverb, float density)
{
    return _seal_setf(reverb, AL_REVERB_DENSITY, density, alEffectf,
                      alIsEffect);
}

seal_err_t
seal_set_reverb_diffusion(seal_reverb_t* reverb, float diffusion)
{
    return _seal_setf(reverb, AL_REVERB_DIFFUSION, diffusion, alEffectf,
                      alIsEffect);
}

seal_err_t
seal_set_reverb_gain(seal_reverb_t* reverb, float gain)
{
    return _seal_setf(reverb, AL_REVERB_GAIN, gain, alEffectf, alIsEffect);
}

seal_err_t
seal_set_reverb_hfgain(seal_reverb_t* reverb, float hfgain)
{
    return _seal_setf(reverb, AL_REVERB_GAINHF, hfgain, alEffectf,
                      alIsEffect);
}

seal_err_t
seal_set_reverb_decay_time(seal_reverb_t* reverb, float time)
{
    return _seal_setf(reverb, AL_REVERB_DECAY_TIME, time, alEffectf,
                      alIsEffect);
}

seal_err_t
seal_set_reverb_hfdecay_ratio(seal_reverb_t* reverb, float ratio)
{
    return _seal_setf(reverb, AL_REVERB_DECAY_HFRATIO, ratio, alEffectf,
                      alIsEffect);
}

seal_err_t
seal_set_reverb_reflections_gain(seal_reverb_t* reverb, float gain)
{
    return _seal_setf(reverb, AL_REVERB_REFLECTIONS_GAIN, gain, alEffectf,
                      alIsEffect);
}

seal_err_t
seal_set_reverb_reflections_delay(seal_reverb_t* reverb, float delay)
{
    return _seal_setf(reverb, AL_REVERB_REFLECTIONS_DELAY, delay, alEffectf,
                      alIsEffect);
}

seal_err_t
seal_set_reverb_late_gain(seal_reverb_t* reverb, float gain)
{
    return _seal_setf(reverb, AL_REVERB_LATE_REVERB_GAIN, gain, alEffectf,
                      alIsEffect);
}

seal_err_t
seal_set_reverb_late_delay(seal_reverb_t* reverb, float delay)
{
    return _seal_setf(reverb, AL_REVERB_LATE_REVERB_DELAY, delay, alEffectf,
                      alIsEffect);
}

seal_err_t
seal_set_reverb_air_absorbtion_hfgain(seal_reverb_t* reverb, float hfgain)
{
    return _seal_setf(reverb, AL_REVERB_AIR_ABSORPTION_GAINHF, hfgain,
                      alEffectf, alIsEffect);
}

seal_err_t
seal_set_reverb_room_rolloff_factor(seal_reverb_t* reverb, float factor)
{
    return _seal_setf(reverb, AL_REVERB_ROOM_ROLLOFF_FACTOR, factor,
                      alEffectf, alIsEffect);
}

seal_err_t
seal_set_reverb_hfdecay_limited(seal_reverb_t* reverb, char limited)
{
    assert(alIsEffect(reverb->id));

    alEffecti(reverb->id, AL_REVERB_DECAY_HFLIMIT, limited);

    return _seal_get_openal_err();
}

seal_err_t
seal_get_reverb_density(seal_reverb_t* reverb, float* pdensity)
{
    return _seal_getf(reverb, AL_REVERB_DENSITY, pdensity, alGetEffectf,
                      alIsEffect);
}

seal_err_t
seal_get_reverb_diffusion(seal_reverb_t* reverb, float* pdiffusion)
{
    return _seal_getf(reverb,  AL_REVERB_DIFFUSION, pdiffusion, alGetEffectf,
                      alIsEffect);
}

seal_err_t
seal_get_reverb_gain(seal_reverb_t* reverb, float* pgain)
{
    return _seal_getf(reverb,  AL_REVERB_GAIN, pgain, alGetEffectf,
                      alIsEffect);
}

seal_err_t
seal_get_reverb_hfgain(seal_reverb_t* reverb, float* phfgain)
{
    return _seal_getf(reverb,  AL_REVERB_GAINHF, phfgain, alGetEffectf,
                      alIsEffect);
}

seal_err_t
seal_get_reverb_decay_time(seal_reverb_t* reverb, float* ptime)
{
    return _seal_getf(reverb,  AL_REVERB_DECAY_TIME, ptime, alGetEffectf,
                      alIsEffect);
}

seal_err_t
seal_get_reverb_hfdecay_ratio(seal_reverb_t* reverb, float* pratio)
{
    return _seal_getf(reverb,  AL_REVERB_DECAY_HFRATIO, pratio, alGetEffectf,
                      alIsEffect);
}

seal_err_t
seal_get_reverb_reflections_gain(seal_reverb_t* reverb, float* pgain)
{
    return _seal_getf(reverb,  AL_REVERB_REFLECTIONS_GAIN, pgain,
                      alGetEffectf, alIsEffect);
}

seal_err_t
seal_get_reverb_reflections_delay(seal_reverb_t* reverb, float* pdelay)
{
    return _seal_getf(reverb,  AL_REVERB_REFLECTIONS_DELAY, pdelay,
                      alGetEffectf, alIsEffect);
}

seal_err_t
seal_get_reverb_late_gain(seal_reverb_t* reverb, float* pgain)
{
    return _seal_getf(reverb,  AL_REVERB_LATE_REVERB_GAIN, pgain,
                      alGetEffectf, alIsEffect);
}

seal_err_t
seal_get_reverb_late_delay(seal_reverb_t* reverb, float* pdelay)
{
    return _seal_getf(reverb,  AL_REVERB_LATE_REVERB_DELAY, pdelay,
                      alGetEffectf, alIsEffect);
}

seal_err_t
seal_get_reverb_air_absorbtion_hfgain(seal_reverb_t* reverb, float* phfgain)
{
    return _seal_getf(reverb,  AL_REVERB_ROOM_ROLLOFF_FACTOR, phfgain,
                      alGetEffectf, alIsEffect);
}

seal_err_t
seal_get_reverb_room_rolloff_factor(seal_reverb_t* reverb, float* pfactor)
{
    return _seal_getf(reverb,  AL_REVERB_AIR_ABSORPTION_GAINHF, pfactor,
                      alGetEffectf, alIsEffect);
}

seal_err_t
seal_is_reverb_hfdecay_limited(seal_reverb_t* reverb, char* plimited)
{
    int limited;
    seal_err_t err;

    assert(alIsEffect(reverb->id));

    alGetEffecti(reverb->id, AL_REVERB_DECAY_HFLIMIT, &limited);
    if ((err = _seal_get_openal_err()) == SEAL_OK)
        *plimited = limited;

    return err;
}
