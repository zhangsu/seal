#include <al/al.h>
#include <al/efx.h>
#include <seal/effect_slot.h>
#include <seal/core.h>
#include <seal/err.h>
#include <assert.h>

struct seal_effect_slot_t
{
    unsigned int id;
    void*        effect;
};

static int
seti(seal_effect_slot_t* slot, int key, int value)
{
    assert(alIsAuxiliaryEffectSlot(slot->id));

    _seal_lock_openal();
    alAuxiliaryEffectSloti(slot->id, key, value);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return 1;
}

static int
geti(seal_effect_slot_t* slot, int key)
{
    int value;

    assert(alIsAuxiliaryEffectSlot(slot->id));

    _seal_lock_openal();
    alGetAuxiliaryEffectSloti(slot->id, key, &value);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return value;
}

seal_effect_slot_t*
seal_alloc_effect_slot(void)
{
    return _seal_alloc_obj(sizeof (seal_effect_slot_t),
                           alGenAuxiliaryEffectSlots);
}

int
seal_free_effect_slot(seal_effect_slot_t* slot)
{
    return _seal_free_obj(slot, alDeleteAuxiliaryEffectSlots,
                          alIsAuxiliaryEffectSlot);
}

int seal_fill_effect_slot(seal_effect_slot_t* slot, void* effect)
{
    assert(effect != 0);

    if (seti(slot, AL_EFFECTSLOT_EFFECT, *(unsigned int*) slot) == 0)
        return 0;

    slot->effect = effect;

    return 1;
}

int seal_unfill_effect_slot(seal_effect_slot_t* slot)
{
    if (seti(slot, AL_EFFECTSLOT_EFFECT, AL_EFFECT_NULL) == 0)
        return 0;

    slot->effect = 0;

    return 1;
}

int seal_set_effect_slot_gain(seal_effect_slot_t* slot, float gain)
{
    assert(alIsAuxiliaryEffectSlot(slot->id));

    _seal_lock_openal();
    alAuxiliaryEffectSlotf(slot->id, AL_EFFECTSLOT_GAIN, gain);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return 1;
}

int seal_set_effect_slot_auto(seal_effect_slot_t* slot, char automatic)
{
    return seti(slot, AL_EFFECTSLOT_AUXILIARY_SEND_AUTO, automatic);
}

void* seal_get_effect_slot_effect(seal_effect_slot_t* slot)
{
    assert(alIsAuxiliaryEffectSlot(slot->id));

    return slot->effect;
}

float seal_get_effect_slot_gain(seal_effect_slot_t* slot)
{
    float value;

    assert(alIsAuxiliaryEffectSlot(slot->id));

    _seal_lock_openal();
    alGetAuxiliaryEffectSlotf(slot->id, AL_EFFECTSLOT_GAIN, &value);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return value;
}

char seal_is_effect_slot_auto(seal_effect_slot_t* slot)
{
    return geti(slot, AL_EFFECTSLOT_AUXILIARY_SEND_AUTO);
}
