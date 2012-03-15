/*
 * effect_slot.h is part of the Scorched End Audio Library (SEAL) and is
 * licensed under the terms of the GNU Lesser General Public License. See
 * COPYING attached with the library.
 *
 * effect_slot.h defines the effect slot type, which is the container type for
 * effects. A source can mix an effect in an effect slot to filter the output
 * sound.
 */

#ifndef _SEAL_EFFECT_SLOT_H_
#define _SEAL_EFFECT_SLOT_H_

#include "err.h"

typedef struct seal_effect_slot_t seal_effect_slot_t;

seal_err_t seal_init_effect_slot(seal_effect_slot_t*);

seal_err_t seal_destroy_effect_slot(seal_effect_slot_t*);

seal_err_t seal_fill_effect_slot(seal_effect_slot_t*, void* /*effect*/);

seal_err_t seal_unfill_effect_slot(seal_effect_slot_t*);

seal_err_t seal_set_effect_slot_gain(seal_effect_slot_t*, float /*gain*/);

seal_err_t seal_set_effect_slot_auto(seal_effect_slot_t*, char /*automatic*/);

seal_err_t seal_get_effect_slot_effect(seal_effect_slot_t*, void* /*effect*/);

seal_err_t seal_get_effect_slot_gain(seal_effect_slot_t*, float* /*pgain*/);

seal_err_t seal_is_effect_slot_auto(seal_effect_slot_t*, char* /*pauto*/);

/*
 *****************************************************************************
 * Below are **implementation details**.
 *****************************************************************************
 */

struct seal_effect_slot_t
{
    unsigned int id;
    void*        effect;
};

#endif /* _SEAL_EFFECT_SLOT_H_ */