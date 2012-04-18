#include <al/al.h>
#include <al/efx.h>
#include <al/efx-presets.h>
#include <seal/rvb.h>
#include <seal/core.h>
#include <seal/err.h>

static void
load_properties(seal_rvb_t* rvb, EFXEAXREVERBPROPERTIES properties)
{
    seal_set_rvb_density(rvb, properties.flDensity);
    seal_set_rvb_diffusion(rvb, properties.flDiffusion);
    seal_set_rvb_gain(rvb, properties.flGain);
    seal_set_rvb_hfgain(rvb, properties.flGainHF);
    seal_set_rvb_decay_time(rvb, properties.flDecayTime);
    seal_set_rvb_hfdecay_ratio(rvb, properties.flDecayHFRatio);
    seal_set_rvb_reflections_gain(rvb, properties.flReflectionsGain);
    seal_set_rvb_reflections_delay(rvb, properties.flReflectionsDelay);
    seal_set_rvb_late_gain(rvb, properties.flLateReverbGain);
    seal_set_rvb_late_delay(rvb, properties.flLateReverbDelay);
    seal_set_rvb_air_absorbtion_hfgain(rvb, properties.flAirAbsorptionGainHF);
    seal_set_rvb_room_rolloff_factor(rvb, properties.flRoomRolloffFactor);
    seal_set_rvb_hfdecay_limited(rvb, properties.iDecayHFLimit);
}

#define LOAD_PROPERTIES(rvb, properties) do                                 \
{                                                                           \
    EFXEAXREVERBPROPERTIES __properties__ = properties;                     \
    load_properties(rvb, __properties__);                                   \
} while (0)

seal_err_t
seal_init_rvb(seal_rvb_t* rvb)
{
    seal_err_t err;

    if ((err = _seal_init_obj(rvb, alGenEffects)) != SEAL_OK)
        return err;
    alEffecti(rvb->id, AL_EFFECT_TYPE, AL_EFFECT_REVERB);

    return _seal_get_openal_err();
}

seal_err_t
seal_destroy_rvb(seal_rvb_t* rvb)
{
    return _seal_destroy_obj(rvb, alDeleteEffects, alIsEffect);
}

seal_err_t
seal_load_rvb_preset(seal_rvb_t* rvb, seal_rvb_preset_t preset)
{
    switch (preset) {
    case SEAL_GENERIC_REVERB:
        LOAD_PROPERTIES(rvb, EFX_REVERB_PRESET_GENERIC);
        break;
    case SEAL_MOUNTAINS_REVERB:
        LOAD_PROPERTIES(rvb, EFX_REVERB_PRESET_MOUNTAINS);
        break;
    }
    return SEAL_OK;
}

seal_err_t
seal_set_rvb_density(seal_rvb_t* rvb, float density)
{
    return _seal_setf(rvb, AL_REVERB_DENSITY, density, alEffectf);
}

seal_err_t
seal_set_rvb_diffusion(seal_rvb_t* rvb, float diffusion)
{
    return _seal_setf(rvb, AL_REVERB_DIFFUSION, diffusion, alEffectf);
}

seal_err_t
seal_set_rvb_gain(seal_rvb_t* rvb, float gain)
{
    return _seal_setf(rvb, AL_REVERB_GAIN, gain, alEffectf);
}

seal_err_t
seal_set_rvb_hfgain(seal_rvb_t* rvb, float hfgain)
{
    return _seal_setf(rvb, AL_REVERB_GAINHF, hfgain, alEffectf);
}

seal_err_t
seal_set_rvb_decay_time(seal_rvb_t* rvb, float time)
{
    return _seal_setf(rvb, AL_REVERB_DECAY_TIME, time, alEffectf);
}

seal_err_t
seal_set_rvb_hfdecay_ratio(seal_rvb_t* rvb, float ratio)
{
    return _seal_setf(rvb, AL_REVERB_DECAY_HFRATIO, ratio, alEffectf);
}

seal_err_t
seal_set_rvb_reflections_gain(seal_rvb_t* rvb, float gain)
{
    return _seal_setf(rvb, AL_REVERB_REFLECTIONS_GAIN, gain, alEffectf);
}

seal_err_t
seal_set_rvb_reflections_delay(seal_rvb_t* rvb, float delay)
{
    return _seal_setf(rvb, AL_REVERB_REFLECTIONS_DELAY, delay, alEffectf);
}

seal_err_t
seal_set_rvb_late_gain(seal_rvb_t* rvb, float gain)
{
    return _seal_setf(rvb, AL_REVERB_LATE_REVERB_GAIN, gain, alEffectf);
}

seal_err_t
seal_set_rvb_late_delay(seal_rvb_t* rvb, float delay)
{
    return _seal_setf(rvb, AL_REVERB_LATE_REVERB_DELAY, delay, alEffectf);
}

seal_err_t
seal_set_rvb_air_absorbtion_hfgain(seal_rvb_t* rvb, float hfgain)
{
    return _seal_setf(rvb, AL_REVERB_AIR_ABSORPTION_GAINHF, hfgain,
                      alEffectf);
}

seal_err_t
seal_set_rvb_room_rolloff_factor(seal_rvb_t* rvb, float factor)
{
    return _seal_setf(rvb, AL_REVERB_ROOM_ROLLOFF_FACTOR, factor, alEffectf);
}

seal_err_t
seal_set_rvb_hfdecay_limited(seal_rvb_t* rvb, char limited)
{
    return _seal_seti(rvb, AL_REVERB_DECAY_HFLIMIT, limited, alEffecti);
}

seal_err_t
seal_get_rvb_density(seal_rvb_t* rvb, float* pdensity)
{
    return _seal_getf(rvb, AL_REVERB_DENSITY, pdensity, alGetEffectf);
}

seal_err_t
seal_get_rvb_diffusion(seal_rvb_t* rvb, float* pdiffusion)
{
    return _seal_getf(rvb,  AL_REVERB_DIFFUSION, pdiffusion, alGetEffectf);
}

seal_err_t
seal_get_rvb_gain(seal_rvb_t* rvb, float* pgain)
{
    return _seal_getf(rvb,  AL_REVERB_GAIN, pgain, alGetEffectf);
}

seal_err_t
seal_get_rvb_hfgain(seal_rvb_t* rvb, float* phfgain)
{
    return _seal_getf(rvb,  AL_REVERB_GAINHF, phfgain, alGetEffectf);
}

seal_err_t
seal_get_rvb_decay_time(seal_rvb_t* rvb, float* ptime)
{
    return _seal_getf(rvb,  AL_REVERB_DECAY_TIME, ptime, alGetEffectf);
}

seal_err_t
seal_get_rvb_hfdecay_ratio(seal_rvb_t* rvb, float* pratio)
{
    return _seal_getf(rvb,  AL_REVERB_DECAY_HFRATIO, pratio, alGetEffectf);
}

seal_err_t
seal_get_rvb_reflections_gain(seal_rvb_t* rvb, float* pgain)
{
    return _seal_getf(rvb,  AL_REVERB_REFLECTIONS_GAIN, pgain,
                      alGetEffectf);
}

seal_err_t
seal_get_rvb_reflections_delay(seal_rvb_t* rvb, float* pdelay)
{
    return _seal_getf(rvb,  AL_REVERB_REFLECTIONS_DELAY, pdelay,
                      alGetEffectf);
}

seal_err_t
seal_get_rvb_late_gain(seal_rvb_t* rvb, float* pgain)
{
    return _seal_getf(rvb,  AL_REVERB_LATE_REVERB_GAIN, pgain,
                      alGetEffectf);
}

seal_err_t
seal_get_rvb_late_delay(seal_rvb_t* rvb, float* pdelay)
{
    return _seal_getf(rvb,  AL_REVERB_LATE_REVERB_DELAY, pdelay,
                      alGetEffectf);
}

seal_err_t
seal_get_rvb_air_absorbtion_hfgain(seal_rvb_t* rvb, float* phfgain)
{
    return _seal_getf(rvb,  AL_REVERB_ROOM_ROLLOFF_FACTOR, phfgain,
                      alGetEffectf);
}

seal_err_t
seal_get_rvb_room_rolloff_factor(seal_rvb_t* rvb, float* pfactor)
{
    return _seal_getf(rvb,  AL_REVERB_AIR_ABSORPTION_GAINHF, pfactor,
                      alGetEffectf);
}

seal_err_t
seal_is_rvb_hfdecay_limited(seal_rvb_t* rvb, char* plimited)
{
    return _seal_getb(rvb, AL_REVERB_DECAY_HFLIMIT, plimited,
                      alGetEffecti);
}
