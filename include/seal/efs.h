/*
 * efs.h is part of the Scorched End Audio Library (SEAL) and is
 * licensed under the terms of the GNU Lesser General Public License. See
 * COPYING attached with the library.
 *
 * efs.h defines the effect slot type, which is the container type for
 * effects. A source can mix an effect in an effect slot to filter the output
 * sound.
 */

#ifndef _SEAL_EFS_H_
#define _SEAL_EFS_H_

#include "src.h"
#include "err.h"

typedef struct seal_efs_t seal_efs_t;

/*
 * Initializes a new effect slot. If the effect slot is no longer needed, call
 * `seal_destroy_efs' to release the resources used by the effect
 * slot.
 *
 * @param efs   the effect slot to initialize
 */
seal_err_t seal_init_efs(seal_efs_t*);

/*
 * Destroys an effect slot.
 *
 * @param efs   the effect slot to destroy
 */
seal_err_t seal_destroy_efs(seal_efs_t*);

/*
 * Fills an effect slot with an effect object, then the effect Slot will
 * become ready to be mixed with sources. Pass 0 to unfill the slot.
 *
 * @param efs       the effect slot to fill
 * @param effect    the effect to fill the effect slot with
 */
seal_err_t seal_set_efs_effect(seal_efs_t*, void* /*effect*/);

/*
 * Mixes an sound effect loaded into an effect slot with a source's output.
 * Later calls to this function with a different effect slot and the same
 * index will override the old effect slot association.
 *
 * @see             seal_get_neffects_per_src
 * @param efs       the slot that contains the effect to mix
 * @param index     the zero-based index of the effect
 * @param src       the source that feeds the effect slot
 */
seal_err_t seal_feed_efs(seal_efs_t*, int /*index*/, seal_src_t*);

/*
 * Sets the output level of an effect Slot in the interval [0.0f, 1.0f]. A
 * value of 0.0 mutes the output.
 *
 * @param efs   the effect slot to set the gain of
 * @param gain  the gain
 */
seal_err_t seal_set_efs_gain(seal_efs_t*, float /*gain*/);

/*
 * Sets whether the effect should have automatic adjustments based on the
 * physical positions of the sources and the listener.
 *
 * @param efs       the effect slot to set the auto adjust flag of
 * @param automatic 1 to set it auto adjust or otherwise 0
 */
seal_err_t seal_set_efs_auto(seal_efs_t*, char /*automatic*/);

/*
 * Gets the effect object in an effect slot. The default is 0 (null pointer).
 *
 * @param efs   the effect slot to get the effect object of
 * @return      the effect object
 */
void* seal_get_efs_effect(seal_efs_t*);

/*
 * Gets the output level of an effect Slot in the interval. The default is
 * 1.0f.
 *
 * @param efs   the effect slot to get the gain of
 * @param pgain the receiver of the gain
 */
seal_err_t seal_get_efs_gain(seal_efs_t*, float* /*pgain*/);

/*
 * Determines if the effect is automatically adjusted. The default is true
 * (nonzero).
 *
 * @param efs   the effect slot to determine
 * @param pauto the receiver of the auto adjust flag
 */
seal_err_t seal_is_efs_auto(seal_efs_t*, char* /*pauto*/);

/*
 *****************************************************************************
 * Below are **implementation details**.
 *****************************************************************************
 */

struct seal_efs_t
{
    unsigned int id;
    void*        effect;
};

#endif /* _SEAL_EFS_H_ */
