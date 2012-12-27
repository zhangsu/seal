#include <al/al.h>
#include <al/efx.h>
#include <seal/efs.h>
#include <seal/core.h>
#include <seal/err.h>

seal_err_t
seal_init_efs(seal_efs_t* slot)
{
    return _seal_init_obj(slot, alGenAuxiliaryEffectSlots);
}

seal_err_t
seal_destroy_efs(seal_efs_t* slot)
{
    return _seal_destroy_obj(slot, alDeleteAuxiliaryEffectSlots,
                             alIsAuxiliaryEffectSlot);
}

seal_err_t
seal_set_efs_effect(seal_efs_t* slot, void* effect)
{
    seal_err_t err;

    err = _seal_seti(slot, AL_EFFECTSLOT_EFFECT,
                     effect == 0 ? AL_EFFECT_NULL : _seal_openal_id(effect),
                     alAuxiliaryEffectSloti);
    if (err == SEAL_OK)
        slot->effect = effect;

    return err;
}

seal_err_t
seal_set_efs_gain(seal_efs_t* slot, float gain)
{
    return _seal_setf(slot, AL_EFFECTSLOT_GAIN, gain, alAuxiliaryEffectSlotf);
}

seal_err_t
seal_set_efs_auto(seal_efs_t* slot, char automatic)
{
    return _seal_seti(slot, AL_EFFECTSLOT_AUXILIARY_SEND_AUTO, automatic,
                      alAuxiliaryEffectSloti);
}

void*
seal_get_efs_effect(seal_efs_t* slot)
{
    return slot->effect;
}

seal_err_t
seal_get_efs_gain(seal_efs_t* slot, float* pgain)
{
    return _seal_getf(slot, AL_EFFECTSLOT_GAIN, pgain,
                      alGetAuxiliaryEffectSlotf);
}

seal_err_t
seal_is_efs_auto(seal_efs_t* slot, char* pauto)
{
    return _seal_getb(slot, AL_EFFECTSLOT_AUXILIARY_SEND_AUTO, pauto,
                      alGetAuxiliaryEffectSloti);
}
