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

/*
 * Initializes a new effect slot. If the effect slot is no longer needed, call
 * `seal_destroy_effect_slot' to release the resources used by the effect
 * slot.
 *
 * @param effect_slot the effect slot to initialize
 */
seal_err_t seal_init_effect_slot(seal_effect_slot_t*);

/*
 * Destroys an effect slot.
 *
 * @param effect_slot the effect slot to destroy
 */
seal_err_t seal_destroy_effect_slot(seal_effect_slot_t*);

/*
 * Fills an effect slot with an effect object, then the effect Slot will
 * become ready to be mixed with sources.
 *
 * @param effect_slot the effect slot to fill
 * @param effect      the effect to fill the effect slot with
 */
seal_err_t seal_fill_effect_slot(seal_effect_slot_t*, void* /*effect*/);

/*
 * Unfills an effect slot.
 *
 * @param effect_slot the effect slot to unfill
 */
seal_err_t seal_unfill_effect_slot(seal_effect_slot_t*);

/*
 * Sets the output level of an effect Slot in the interval [0.0f, 1.0f]. A
 * value of 0.0 mutes the output.
 *
 * @param effect_slot the effect slot to set the gain of
 * @param gain        the gain
 */
seal_err_t seal_set_effect_slot_gain(seal_effect_slot_t*, float /*gain*/);

/*
 * Sets whether the effect should have automatic adjustments based on the
 * physical positions of the sources and the listener.
 *
 * @param effect_slot the effect slot to set the auto adjust flag of
 * @param automatic   1 to set it auto adjust or otherwise 0
 */
seal_err_t seal_set_effect_slot_auto(seal_effect_slot_t*, char /*automatic*/);

/*
 * Gets the effect object in an effect slot. The default is 0 (null pointer).
 *
 * @param effect_slot the effect slot to get the effect object of
 * @return            the effect object
 */
void* seal_get_effect_slot_effect(seal_effect_slot_t*);

/*
 * Gets the output level of an effect Slot in the interval. The default is
 * 1.0f.
 *
 * @param effect_slot the effect slot to get the gain of
 * @param pgain       the receiver of the gain
 */
seal_err_t seal_get_effect_slot_gain(seal_effect_slot_t*, float* /*pgain*/);

/*
 * Determines if the effect is automatically adjusted. The default is true
 * (nonzero).
 *
 * @param effect_slot the effect slot to determine
 * @param pauto       the receiver of the auto adjust flag
 */
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