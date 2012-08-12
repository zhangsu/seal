#include <al/al.h>
#include <al/efx.h>
#include <al/efx-presets.h>
#include <seal/rvb.h>
#include <seal/core.h>
#include <seal/err.h>

static
void
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

/* A macro tied very closely to `seal_load_rvb'. */
#define CASE_LOAD(rvb, preset)                                              \
    case SEAL_##preset##_REVERB:                                            \
    {                                                                       \
        EFXEAXREVERBPROPERTIES __properties__ = EFX_REVERB_PRESET_##preset; \
        load_properties(rvb, __properties__);                               \
        break;                                                              \
    }

seal_err_t
seal_load_rvb(seal_rvb_t* rvb, seal_rvb_preset_t preset)
{
    switch (preset) {
        /* Default presets */
        CASE_LOAD(rvb, GENERIC)
        CASE_LOAD(rvb, PADDEDCELL)
        CASE_LOAD(rvb, ROOM)
        CASE_LOAD(rvb, BATHROOM)
        CASE_LOAD(rvb, LIVINGROOM)
        CASE_LOAD(rvb, STONEROOM)
        CASE_LOAD(rvb, AUDITORIUM)
        CASE_LOAD(rvb, CONCERTHALL)
        CASE_LOAD(rvb, CAVE)
        CASE_LOAD(rvb, ARENA)
        CASE_LOAD(rvb, HANGAR)
        CASE_LOAD(rvb, CARPETEDHALLWAY)
        CASE_LOAD(rvb, HALLWAY)
        CASE_LOAD(rvb, STONECORRIDOR)
        CASE_LOAD(rvb, ALLEY)
        CASE_LOAD(rvb, FOREST)
        CASE_LOAD(rvb, CITY)
        CASE_LOAD(rvb, MOUNTAINS)
        CASE_LOAD(rvb, QUARRY)
        CASE_LOAD(rvb, PLAIN)
        CASE_LOAD(rvb, PARKINGLOT)
        CASE_LOAD(rvb, SEWERPIPE)
        CASE_LOAD(rvb, UNDERWATER)
        CASE_LOAD(rvb, DRUGGED)
        CASE_LOAD(rvb, DIZZY)
        CASE_LOAD(rvb, PSYCHOTIC)

        /* Castle presets */
        CASE_LOAD(rvb, CASTLE_SMALLROOM)
        CASE_LOAD(rvb, CASTLE_SHORTPASSAGE)
        CASE_LOAD(rvb, CASTLE_MEDIUMROOM)
        CASE_LOAD(rvb, CASTLE_LARGEROOM)
        CASE_LOAD(rvb, CASTLE_LONGPASSAGE)
        CASE_LOAD(rvb, CASTLE_HALL)
        CASE_LOAD(rvb, CASTLE_CUPBOARD)
        CASE_LOAD(rvb, CASTLE_COURTYARD)
        CASE_LOAD(rvb, CASTLE_ALCOVE)

        /* Factory presets */
        CASE_LOAD(rvb, FACTORY_SMALLROOM)
        CASE_LOAD(rvb, FACTORY_SHORTPASSAGE)
        CASE_LOAD(rvb, FACTORY_MEDIUMROOM)
        CASE_LOAD(rvb, FACTORY_LARGEROOM)
        CASE_LOAD(rvb, FACTORY_LONGPASSAGE)
        CASE_LOAD(rvb, FACTORY_HALL)
        CASE_LOAD(rvb, FACTORY_CUPBOARD)
        CASE_LOAD(rvb, FACTORY_COURTYARD)
        CASE_LOAD(rvb, FACTORY_ALCOVE)

        /* Ice palace presets */
        CASE_LOAD(rvb, ICEPALACE_SMALLROOM)
        CASE_LOAD(rvb, ICEPALACE_SHORTPASSAGE)
        CASE_LOAD(rvb, ICEPALACE_MEDIUMROOM)
        CASE_LOAD(rvb, ICEPALACE_LARGEROOM)
        CASE_LOAD(rvb, ICEPALACE_LONGPASSAGE)
        CASE_LOAD(rvb, ICEPALACE_HALL)
        CASE_LOAD(rvb, ICEPALACE_CUPBOARD)
        CASE_LOAD(rvb, ICEPALACE_COURTYARD)
        CASE_LOAD(rvb, ICEPALACE_ALCOVE)

        /* Space station presets */
        CASE_LOAD(rvb, SPACESTATION_SMALLROOM)
        CASE_LOAD(rvb, SPACESTATION_SHORTPASSAGE)
        CASE_LOAD(rvb, SPACESTATION_MEDIUMROOM)
        CASE_LOAD(rvb, SPACESTATION_LARGEROOM)
        CASE_LOAD(rvb, SPACESTATION_LONGPASSAGE)
        CASE_LOAD(rvb, SPACESTATION_HALL)
        CASE_LOAD(rvb, SPACESTATION_CUPBOARD)
        CASE_LOAD(rvb, SPACESTATION_ALCOVE)

        /* Wooden Galleon presets */
        CASE_LOAD(rvb, WOODEN_SMALLROOM)
        CASE_LOAD(rvb, WOODEN_SHORTPASSAGE)
        CASE_LOAD(rvb, WOODEN_MEDIUMROOM)
        CASE_LOAD(rvb, WOODEN_LARGEROOM)
        CASE_LOAD(rvb, WOODEN_LONGPASSAGE)
        CASE_LOAD(rvb, WOODEN_HALL)
        CASE_LOAD(rvb, WOODEN_CUPBOARD)
        CASE_LOAD(rvb, WOODEN_COURTYARD)
        CASE_LOAD(rvb, WOODEN_ALCOVE)

        /* Sports presets */
        CASE_LOAD(rvb, SPORT_EMPTYSTADIUM)
        CASE_LOAD(rvb, SPORT_SQUASHCOURT)
        CASE_LOAD(rvb, SPORT_SMALLSWIMMINGPOOL)
        CASE_LOAD(rvb, SPORT_LARGESWIMMINGPOOL)
        CASE_LOAD(rvb, SPORT_GYMNASIUM)
        CASE_LOAD(rvb, SPORT_FULLSTADIUM)
        CASE_LOAD(rvb, SPORT_STADIUMTANNOY)

        /* Prefab presets */
        CASE_LOAD(rvb, PREFAB_WORKSHOP)
        CASE_LOAD(rvb, PREFAB_SCHOOLROOM)
        CASE_LOAD(rvb, PREFAB_PRACTISEROOM)
        CASE_LOAD(rvb, PREFAB_OUTHOUSE)
        CASE_LOAD(rvb, PREFAB_CARAVAN)

        /* Dome presets */
        CASE_LOAD(rvb, DOME_TOMB)
        CASE_LOAD(rvb, DOME_SAINTPAULS)

        /* Pipe presets. */
        CASE_LOAD(rvb, PIPE_SMALL)
        CASE_LOAD(rvb, PIPE_LONGTHIN)
        CASE_LOAD(rvb, PIPE_LARGE)
        CASE_LOAD(rvb, PIPE_RESONANT)

        /* Outdoors presets */
        CASE_LOAD(rvb, OUTDOORS_BACKYARD)
        CASE_LOAD(rvb, OUTDOORS_ROLLINGPLAINS)
        CASE_LOAD(rvb, OUTDOORS_DEEPCANYON)
        CASE_LOAD(rvb, OUTDOORS_CREEK)
        CASE_LOAD(rvb, OUTDOORS_VALLEY)

        /* Mood presets */
        CASE_LOAD(rvb, MOOD_HEAVEN)
        CASE_LOAD(rvb, MOOD_HELL)
        CASE_LOAD(rvb, MOOD_MEMORY)

        /* Driving presets */
        CASE_LOAD(rvb, DRIVING_COMMENTATOR)
        CASE_LOAD(rvb, DRIVING_PITGARAGE)
        CASE_LOAD(rvb, DRIVING_INCAR_RACER)
        CASE_LOAD(rvb, DRIVING_INCAR_SPORTS)
        CASE_LOAD(rvb, DRIVING_INCAR_LUXURY)
        CASE_LOAD(rvb, DRIVING_FULLGRANDSTAND)
        CASE_LOAD(rvb, DRIVING_EMPTYGRANDSTAND)
        CASE_LOAD(rvb, DRIVING_TUNNEL)

        /* City presets */
        CASE_LOAD(rvb, CITY_STREETS)
        CASE_LOAD(rvb, CITY_SUBWAY)
        CASE_LOAD(rvb, CITY_MUSEUM)
        CASE_LOAD(rvb, CITY_LIBRARY)
        CASE_LOAD(rvb, CITY_UNDERPASS)
        CASE_LOAD(rvb, CITY_ABANDONED)

        /* Misc. presets */
        CASE_LOAD(rvb, DUSTYROOM)
        CASE_LOAD(rvb, CHAPEL)
        CASE_LOAD(rvb, SMALLWATERROOM)
        default:
            return SEAL_BAD_PRESET;
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
