#include <al/al.h>
#include <al/efx.h>
#include <seal/reverb.h>
#include <seal/core.h>
#include <seal/err.h>
#include <assert.h>

struct seal_reverb_t
{
    unsigned int id;
};

static int setf(seal_reverb_t*, int, float);
static float getf(seal_reverb_t*, int);

seal_reverb_t*
seal_alloc_reverb(void)
{
    return _seal_alloc_obj(sizeof (seal_reverb_t), alGenEffects);
}

int
seal_free_reverb(seal_reverb_t* reverb)
{
    return _seal_free_obj(reverb, alDeleteEffects, alIsEffect);
}

int
seal_set_reverb_density(seal_reverb_t* reverb, float density)
{
    return setf(reverb, AL_REVERB_DENSITY, density);
}

int
seal_set_reverb_diffusion(seal_reverb_t* reverb, float diffusion)
{
    return setf(reverb, AL_REVERB_DIFFUSION, diffusion);
}

int
seal_set_reverb_gain(seal_reverb_t* reverb, float gain)
{
    return setf(reverb, AL_REVERB_GAIN, gain);
}

int
seal_set_reverb_hfgain(seal_reverb_t* reverb, float hfgain)
{
    return setf(reverb, AL_REVERB_GAINHF, hfgain);
}

int
seal_set_reverb_decay_time(seal_reverb_t* reverb, float decay_time)
{
    return setf(reverb, AL_REVERB_DECAY_TIME, decay_time);
}

int
seal_set_reverb_hfdecay_ratio(seal_reverb_t* reverb, float decay_hfratio)
{
    return setf(reverb, AL_REVERB_DECAY_HFRATIO, decay_hfratio);
}

int
seal_set_reverb_reflections_gain(seal_reverb_t* reverb, float gain)
{
    return setf(reverb, AL_REVERB_REFLECTIONS_GAIN, gain);
}

int
seal_set_reverb_reflections_delay(seal_reverb_t* reverb, float delay)
{
    return setf(reverb, AL_REVERB_REFLECTIONS_DELAY, delay);
}

int
seal_set_reverb_late_gain(seal_reverb_t* reverb, float gain)
{
    return setf(reverb, AL_REVERB_LATE_REVERB_GAIN, gain);
}

int
seal_set_reverb_late_delay(seal_reverb_t* reverb, float delay)
{
    return setf(reverb, AL_REVERB_LATE_REVERB_DELAY, delay);
}

int
seal_set_reverb_air_absorbtion_hfgain(seal_reverb_t* reverb, float hfgain)
{
    return setf(reverb, AL_REVERB_AIR_ABSORPTION_GAINHF, hfgain);
}

int
seal_set_reverb_room_rolloff_factor(seal_reverb_t* reverb, float factor)
{
    return setf(reverb, AL_REVERB_ROOM_ROLLOFF_FACTOR, factor);
}

int
seal_set_reverb_hfdecay_limited(seal_reverb_t* reverb, char limited)
{
    assert(alIsEffect(reverb->id));

    _seal_lock_openal();
    alEffecti(reverb->id, AL_REVERB_DECAY_HFLIMIT, limited);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return 0;
}

float
seal_get_reverb_density(seal_reverb_t* reverb)
{
    return getf(reverb, AL_REVERB_DENSITY);
}

float
seal_get_reverb_diffusion(seal_reverb_t* reverb)
{
    return getf(reverb,  AL_REVERB_DIFFUSION);
}

float
seal_get_reverb_gain(seal_reverb_t* reverb)
{
    return getf(reverb,  AL_REVERB_GAIN);
}

float
seal_get_reverb_hfgain(seal_reverb_t* reverb)
{
    return getf(reverb,  AL_REVERB_GAINHF);
}

float
seal_get_reverb_decay_time(seal_reverb_t* reverb)
{
    return getf(reverb,  AL_REVERB_DECAY_TIME);
}

float
seal_get_reverb_hfdecay_ratio(seal_reverb_t* reverb)
{
    return getf(reverb,  AL_REVERB_DECAY_HFRATIO);
}

float
seal_get_reverb_reflections_gain(seal_reverb_t* reverb)
{
    return getf(reverb,  AL_REVERB_REFLECTIONS_GAIN);
}

float
seal_get_reverb_reflections_delay(seal_reverb_t* reverb)
{
    return getf(reverb,  AL_REVERB_REFLECTIONS_DELAY);
}

float
seal_get_reverb_late_gain(seal_reverb_t* reverb)
{
    return getf(reverb,  AL_REVERB_LATE_REVERB_GAIN);
}

float
seal_get_reverb_late_delay(seal_reverb_t* reverb)
{
    return getf(reverb,  AL_REVERB_LATE_REVERB_DELAY);
}

float
seal_get_reverb_air_absorbtion_hfgain(seal_reverb_t* reverb)
{
    return getf(reverb,  AL_REVERB_ROOM_ROLLOFF_FACTOR);
}

float
seal_get_reverb_room_rolloff_factor(seal_reverb_t* reverb)
{
    return getf(reverb,  AL_REVERB_AIR_ABSORPTION_GAINHF);
}

char
seal_is_reverb_hfdecay_limited(seal_reverb_t* reverb)
{
    int attr_value;

    assert(alIsEffect(reverb->id));

    _seal_lock_openal();
    alGetEffecti(reverb->id, AL_REVERB_DECAY_HFLIMIT, &attr_value);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return attr_value;
}

int
setf(seal_reverb_t* reverb, int key, float value)
{
    assert(alIsEffect(reverb->id));

    _seal_lock_openal();
    alEffectf(reverb->id, key, value);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return 1;
}

float
getf(seal_reverb_t* reverb, int key)
{
    float value;

    assert(alIsEffect(reverb->id));

    _seal_lock_openal();
    alGetEffectf(reverb->id, key, &value);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return value;
}
