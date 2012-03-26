#include <assert.h>
#include <al/al.h>
#include <al/efx.h>
#include <seal/effect_slot.h>
#include <seal/core.h>
#include <seal/err.h>

static seal_err_t
seti(seal_effect_slot_t* slot, int key, int value)
{
    assert(alIsAuxiliaryEffectSlot(slot->id));

    _seal_lock_openal();
    alAuxiliaryEffectSloti(slot->id, key, value);

    return _seal_get_openal_err();
}

static seal_err_t
geti(seal_effect_slot_t* slot, int key, int* pvalue)
{
    assert(alIsAuxiliaryEffectSlot(slot->id));

    _seal_lock_openal();
    alGetAuxiliaryEffectSloti(slot->id, key, pvalue);

    return _seal_get_openal_err();
}

seal_err_t
seal_init_effect_slot(seal_effect_slot_t* slot)
{
    return _seal_init_obj(slot, alGenAuxiliaryEffectSlots);
}

seal_err_t
seal_destroy_effect_slot(seal_effect_slot_t* slot)
{
    if (alIsAuxiliaryEffectSlot == 0)
        return SEAL_OK;

    return _seal_destroy_obj(slot, alDeleteAuxiliaryEffectSlots,
                             alIsAuxiliaryEffectSlot);
}

seal_err_t
seal_set_effect_slot_effect(seal_effect_slot_t* slot, void* effect)
{
    seal_err_t err;

    assert(effect != 0);

    /* Hack: assuming the effect slot id is always at offset 0. */
    err = seti(slot, AL_EFFECTSLOT_EFFECT,
               effect == 0 ? AL_EFFECT_NULL : *(unsigned int*) effect);
    if (err == SEAL_OK)
        slot->effect = effect;

    return err;
}

seal_err_t
seal_set_effect_slot_gain(seal_effect_slot_t* slot, float gain)
{
    assert(alIsAuxiliaryEffectSlot(slot->id));

    _seal_lock_openal();
    alAuxiliaryEffectSlotf(slot->id, AL_EFFECTSLOT_GAIN, gain);

    return _seal_get_openal_err();
}

seal_err_t
seal_set_effect_slot_auto(seal_effect_slot_t* slot, char automatic)
{
    return seti(slot, AL_EFFECTSLOT_AUXILIARY_SEND_AUTO, automatic);
}

void*
seal_get_effect_slot_effect(seal_effect_slot_t* slot)
{
    assert(alIsAuxiliaryEffectSlot(slot->id));

    return slot->effect;
}

seal_err_t
seal_get_effect_slot_gain(seal_effect_slot_t* slot, float* pgain)
{
    assert(alIsAuxiliaryEffectSlot(slot->id));

    _seal_lock_openal();
    alGetAuxiliaryEffectSlotf(slot->id, AL_EFFECTSLOT_GAIN, pgain);

    return _seal_get_openal_err();
}

seal_err_t
seal_is_effect_slot_auto(seal_effect_slot_t* slot, char* pauto)
{
    int automatic;
    seal_err_t err;

    err = geti(slot, AL_EFFECTSLOT_AUXILIARY_SEND_AUTO, &automatic);
    if (err == SEAL_OK)
        *pauto = automatic;

    return err;
}
