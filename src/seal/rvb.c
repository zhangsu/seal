#include <al/al.h>
#include <al/efx.h>
#include <seal/rvb.h>
#include <seal/core.h>
#include <seal/err.h>

seal_err_t
seal_init_rvb(seal_rvb_t* reverb)
{
    seal_err_t err;

    if ((err = _seal_init_obj(reverb, alGenEffects)) != SEAL_OK)
        return err;
    alEffecti(reverb->id, AL_EFFECT_TYPE, AL_EFFECT_REVERB);

    return _seal_get_openal_err();
}

seal_err_t
seal_destroy_rvb(seal_rvb_t* reverb)
{
    return _seal_destroy_obj(reverb, alDeleteEffects, alIsEffect);
}

seal_err_t
seal_set_rvb_density(seal_rvb_t* reverb, float density)
{
    return _seal_setf(reverb, AL_REVERB_DENSITY, density, alEffectf);
}

seal_err_t
seal_set_rvb_diffusion(seal_rvb_t* reverb, float diffusion)
{
    return _seal_setf(reverb, AL_REVERB_DIFFUSION, diffusion, alEffectf);
}

seal_err_t
seal_set_rvb_gain(seal_rvb_t* reverb, float gain)
{
    return _seal_setf(reverb, AL_REVERB_GAIN, gain, alEffectf);
}

seal_err_t
seal_set_rvb_hfgain(seal_rvb_t* reverb, float hfgain)
{
    return _seal_setf(reverb, AL_REVERB_GAINHF, hfgain, alEffectf);
}

seal_err_t
seal_set_rvb_decay_time(seal_rvb_t* reverb, float time)
{
    return _seal_setf(reverb, AL_REVERB_DECAY_TIME, time, alEffectf);
}

seal_err_t
seal_set_rvb_hfdecay_ratio(seal_rvb_t* reverb, float ratio)
{
    return _seal_setf(reverb, AL_REVERB_DECAY_HFRATIO, ratio, alEffectf);
}

seal_err_t
seal_set_rvb_reflections_gain(seal_rvb_t* reverb, float gain)
{
    return _seal_setf(reverb, AL_REVERB_REFLECTIONS_GAIN, gain, alEffectf);
}

seal_err_t
seal_set_rvb_reflections_delay(seal_rvb_t* reverb, float delay)
{
    return _seal_setf(reverb, AL_REVERB_REFLECTIONS_DELAY, delay, alEffectf);
}

seal_err_t
seal_set_rvb_late_gain(seal_rvb_t* reverb, float gain)
{
    return _seal_setf(reverb, AL_REVERB_LATE_REVERB_GAIN, gain, alEffectf);
}

seal_err_t
seal_set_rvb_late_delay(seal_rvb_t* reverb, float delay)
{
    return _seal_setf(reverb, AL_REVERB_LATE_REVERB_DELAY, delay, alEffectf);
}

seal_err_t
seal_set_rvb_air_absorbtion_hfgain(seal_rvb_t* reverb, float hfgain)
{
    return _seal_setf(reverb, AL_REVERB_AIR_ABSORPTION_GAINHF, hfgain,
                      alEffectf);
}

seal_err_t
seal_set_rvb_room_rolloff_factor(seal_rvb_t* reverb, float factor)
{
    return _seal_setf(reverb, AL_REVERB_ROOM_ROLLOFF_FACTOR, factor,
                      alEffectf);
}

seal_err_t
seal_set_rvb_hfdecay_limited(seal_rvb_t* reverb, char limited)
{
    return _seal_seti(reverb, AL_REVERB_DECAY_HFLIMIT, limited, alEffecti);
}

seal_err_t
seal_get_rvb_density(seal_rvb_t* reverb, float* pdensity)
{
    return _seal_getf(reverb, AL_REVERB_DENSITY, pdensity, alGetEffectf);
}

seal_err_t
seal_get_rvb_diffusion(seal_rvb_t* reverb, float* pdiffusion)
{
    return _seal_getf(reverb,  AL_REVERB_DIFFUSION, pdiffusion, alGetEffectf);
}

seal_err_t
seal_get_rvb_gain(seal_rvb_t* reverb, float* pgain)
{
    return _seal_getf(reverb,  AL_REVERB_GAIN, pgain, alGetEffectf);
}

seal_err_t
seal_get_rvb_hfgain(seal_rvb_t* reverb, float* phfgain)
{
    return _seal_getf(reverb,  AL_REVERB_GAINHF, phfgain, alGetEffectf);
}

seal_err_t
seal_get_rvb_decay_time(seal_rvb_t* reverb, float* ptime)
{
    return _seal_getf(reverb,  AL_REVERB_DECAY_TIME, ptime, alGetEffectf);
}

seal_err_t
seal_get_rvb_hfdecay_ratio(seal_rvb_t* reverb, float* pratio)
{
    return _seal_getf(reverb,  AL_REVERB_DECAY_HFRATIO, pratio, alGetEffectf);
}

seal_err_t
seal_get_rvb_reflections_gain(seal_rvb_t* reverb, float* pgain)
{
    return _seal_getf(reverb,  AL_REVERB_REFLECTIONS_GAIN, pgain,
                      alGetEffectf);
}

seal_err_t
seal_get_rvb_reflections_delay(seal_rvb_t* reverb, float* pdelay)
{
    return _seal_getf(reverb,  AL_REVERB_REFLECTIONS_DELAY, pdelay,
                      alGetEffectf);
}

seal_err_t
seal_get_rvb_late_gain(seal_rvb_t* reverb, float* pgain)
{
    return _seal_getf(reverb,  AL_REVERB_LATE_REVERB_GAIN, pgain,
                      alGetEffectf);
}

seal_err_t
seal_get_rvb_late_delay(seal_rvb_t* reverb, float* pdelay)
{
    return _seal_getf(reverb,  AL_REVERB_LATE_REVERB_DELAY, pdelay,
                      alGetEffectf);
}

seal_err_t
seal_get_rvb_air_absorbtion_hfgain(seal_rvb_t* reverb, float* phfgain)
{
    return _seal_getf(reverb,  AL_REVERB_ROOM_ROLLOFF_FACTOR, phfgain,
                      alGetEffectf);
}

seal_err_t
seal_get_rvb_room_rolloff_factor(seal_rvb_t* reverb, float* pfactor)
{
    return _seal_getf(reverb,  AL_REVERB_AIR_ABSORPTION_GAINHF, pfactor,
                      alGetEffectf);
}

seal_err_t
seal_is_rvb_hfdecay_limited(seal_rvb_t* reverb, char* plimited)
{
    return _seal_getb(reverb, AL_REVERB_DECAY_HFLIMIT, plimited,
                      alGetEffecti);
}
