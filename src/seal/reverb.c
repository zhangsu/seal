#include <assert.h>
#include <al/al.h>
#include <al/efx.h>
#include <seal/reverb.h>
#include <seal/core.h>
#include <seal/err.h>

static seal_err_t
setf(seal_reverb_t* reverb, int key, float value)
{
    assert(alIsEffect(reverb->id));

    _seal_lock_openal();
    alEffectf(reverb->id, key, value);

    return _seal_get_openal_err();
}

static seal_err_t
getf(seal_reverb_t* reverb, int key, float* pvalue)
{
    assert(alIsEffect(reverb->id));

    _seal_lock_openal();
    alGetEffectf(reverb->id, key, pvalue);

    return _seal_get_openal_err();
}

seal_err_t
seal_init_reverb(seal_reverb_t* reverb)
{
    seal_err_t err;

    if ((err = _seal_init_obj(reverb, alGenEffects)) != SEAL_OK)
        return err;

    _seal_lock_openal();
    alEffecti(reverb->id, AL_EFFECT_TYPE, AL_EFFECT_REVERB);

    return _seal_get_openal_err();
}

seal_err_t
seal_destroy_reverb(seal_reverb_t* reverb)
{
    if (alIsEffect == 0)
        return SEAL_OK;

    return _seal_destroy_obj(reverb, alDeleteEffects, alIsEffect);
}

seal_err_t
seal_set_reverb_density(seal_reverb_t* reverb, float density)
{
    return setf(reverb, AL_REVERB_DENSITY, density);
}

seal_err_t
seal_set_reverb_diffusion(seal_reverb_t* reverb, float diffusion)
{
    return setf(reverb, AL_REVERB_DIFFUSION, diffusion);
}

seal_err_t
seal_set_reverb_gain(seal_reverb_t* reverb, float gain)
{
    return setf(reverb, AL_REVERB_GAIN, gain);
}

seal_err_t
seal_set_reverb_hfgain(seal_reverb_t* reverb, float hfgain)
{
    return setf(reverb, AL_REVERB_GAINHF, hfgain);
}

seal_err_t
seal_set_reverb_decay_time(seal_reverb_t* reverb, float time)
{
    return setf(reverb, AL_REVERB_DECAY_TIME, time);
}

seal_err_t
seal_set_reverb_hfdecay_ratio(seal_reverb_t* reverb, float ratio)
{
    return setf(reverb, AL_REVERB_DECAY_HFRATIO, ratio);
}

seal_err_t
seal_set_reverb_reflections_gain(seal_reverb_t* reverb, float gain)
{
    return setf(reverb, AL_REVERB_REFLECTIONS_GAIN, gain);
}

seal_err_t
seal_set_reverb_reflections_delay(seal_reverb_t* reverb, float delay)
{
    return setf(reverb, AL_REVERB_REFLECTIONS_DELAY, delay);
}

seal_err_t
seal_set_reverb_late_gain(seal_reverb_t* reverb, float gain)
{
    return setf(reverb, AL_REVERB_LATE_REVERB_GAIN, gain);
}

seal_err_t
seal_set_reverb_late_delay(seal_reverb_t* reverb, float delay)
{
    return setf(reverb, AL_REVERB_LATE_REVERB_DELAY, delay);
}

seal_err_t
seal_set_reverb_air_absorbtion_hfgain(seal_reverb_t* reverb, float hfgain)
{
    return setf(reverb, AL_REVERB_AIR_ABSORPTION_GAINHF, hfgain);
}

seal_err_t
seal_set_reverb_room_rolloff_factor(seal_reverb_t* reverb, float factor)
{
    return setf(reverb, AL_REVERB_ROOM_ROLLOFF_FACTOR, factor);
}

seal_err_t
seal_set_reverb_hfdecay_limited(seal_reverb_t* reverb, char limited)
{
    assert(alIsEffect(reverb->id));

    _seal_lock_openal();
    alEffecti(reverb->id, AL_REVERB_DECAY_HFLIMIT, limited);

    return _seal_get_openal_err();
}

seal_err_t
seal_get_reverb_density(seal_reverb_t* reverb, float* pdensity)
{
    return getf(reverb, AL_REVERB_DENSITY, pdensity);
}

seal_err_t
seal_get_reverb_diffusion(seal_reverb_t* reverb, float* pdiffusion)
{
    return getf(reverb,  AL_REVERB_DIFFUSION, pdiffusion);
}

seal_err_t
seal_get_reverb_gain(seal_reverb_t* reverb, float* pgain)
{
    return getf(reverb,  AL_REVERB_GAIN, pgain);
}

seal_err_t
seal_get_reverb_hfgain(seal_reverb_t* reverb, float* phfgain)
{
    return getf(reverb,  AL_REVERB_GAINHF, phfgain);
}

seal_err_t
seal_get_reverb_decay_time(seal_reverb_t* reverb, float* ptime)
{
    return getf(reverb,  AL_REVERB_DECAY_TIME, ptime);
}

seal_err_t
seal_get_reverb_hfdecay_ratio(seal_reverb_t* reverb, float* pratio)
{
    return getf(reverb,  AL_REVERB_DECAY_HFRATIO, pratio);
}

seal_err_t
seal_get_reverb_reflections_gain(seal_reverb_t* reverb, float* pgain)
{
    return getf(reverb,  AL_REVERB_REFLECTIONS_GAIN, pgain);
}

seal_err_t
seal_get_reverb_reflections_delay(seal_reverb_t* reverb, float* pdelay)
{
    return getf(reverb,  AL_REVERB_REFLECTIONS_DELAY, pdelay);
}

seal_err_t
seal_get_reverb_late_gain(seal_reverb_t* reverb, float* pgain)
{
    return getf(reverb,  AL_REVERB_LATE_REVERB_GAIN, pgain);
}

seal_err_t
seal_get_reverb_late_delay(seal_reverb_t* reverb, float* pdelay)
{
    return getf(reverb,  AL_REVERB_LATE_REVERB_DELAY, pdelay);
}

seal_err_t
seal_get_reverb_air_absorbtion_hfgain(seal_reverb_t* reverb, float* phfgain)
{
    return getf(reverb,  AL_REVERB_ROOM_ROLLOFF_FACTOR, phfgain);
}

seal_err_t
seal_get_reverb_room_rolloff_factor(seal_reverb_t* reverb, float* pfactor)
{
    return getf(reverb,  AL_REVERB_AIR_ABSORPTION_GAINHF, pfactor);
}

seal_err_t
seal_is_reverb_hfdecay_limited(seal_reverb_t* reverb, char* plimited)
{
    int limited;
    seal_err_t err;

    assert(alIsEffect(reverb->id));

    _seal_lock_openal();
    alGetEffecti(reverb->id, AL_REVERB_DECAY_HFLIMIT, &limited);
    if ((err = _seal_get_openal_err()) == SEAL_OK)
        *plimited = limited;

    return err;
}
