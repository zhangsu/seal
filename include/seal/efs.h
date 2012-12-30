/*
 * Interfaces for manipulating effect slots, which are containers for effect
 * objects. Effect slots can attach effect objects (such as reverb objects) and
 * then be fed with a mix of audio from different sources, essentially
 * filtering the rendering of the sound sources and output to the mixer based
 * on the attached effect object. For example, if a reverb object is attached
 * to an effect slot and one source is feeding the slot, the sound of that
 * source will have the reverberation effect defined by the reverb object.

 * Multiple sources can feed the same effect slot, but conversely there is a
 * limit on the number of effect slots a source can feed concurrently. See the
 * documentation for EffectSlot#feed for more details.
 *
 * For more infomation about effect slots, check out the OpenAL effect
 * extension guide at: http://zhang.su/seal/EffectsExtensionGuide.pdf
 */

#ifndef _SEAL_EFS_H_
#define _SEAL_EFS_H_

#include "err.h"

typedef struct seal_efs_t seal_efs_t;

/*
 * Initializes a new effect slot. If the effect slot is no longer needed, call
 * `seal_destroy_efs' to release the resources used by the effect slot.
 *
 * There is a limit on the number of allocated effect slots. This function
 * returns an error if it is exceeding the limit.
 *
 * @param efs   the effect slot to initialize
 */
seal_err_t SEAL_API seal_init_efs(seal_efs_t*);

/*
 * Destroys an effect slot.
 *
 * @param efs   the effect slot to destroy
 */
seal_err_t SEAL_API seal_destroy_efs(seal_efs_t*);

/*
 * Fills an effect slot with an effect object, then the effect Slot will
 * become ready to be fed by sources. Pass 0 to unfill the slot.
 *
 * Changing the parameters of the effect object after it is attached to the
 * slot will not change the sound effect provided by the slot. To update the
 * sound effect, the effect object must be re-attached to the slot.
 *
 * @param efs       the effect slot to fill
 * @param effect    the effect to fill the effect slot with
 */
seal_err_t SEAL_API seal_set_efs_effect(seal_efs_t*, void* /*effect*/);

/*
 * Sets the output level of an effect Slot in the interval [0.0f, 1.0f]. A
 * value of 0.0 mutes the output.
 *
 * @param efs   the effect slot to set the gain of
 * @param gain  the gain
 */
seal_err_t SEAL_API seal_set_efs_gain(seal_efs_t*, float /*gain*/);

/*
 * Sets whether the effect should have automatic adjustments based on the
 * physical positions of the sources and the listener.
 *
 * @param efs       the effect slot to set the auto adjust flag of
 * @param automatic 1 to set it auto adjust or otherwise 0
 */
seal_err_t SEAL_API seal_set_efs_auto(seal_efs_t*, char /*automatic*/);

/*
 * Gets the effect object in an effect slot. The default is 0 (null pointer).
 *
 * @param efs   the effect slot to get the effect object of
 * @return      the effect object
 */
void* SEAL_API seal_get_efs_effect(seal_efs_t*);

/*
 * Gets the output level of an effect Slot in the interval. The default is
 * 1.0f.
 *
 * @param efs   the effect slot to get the gain of
 * @param pgain the receiver of the gain
 */
seal_err_t SEAL_API seal_get_efs_gain(seal_efs_t*, float* /*pgain*/);

/*
 * Determines if the effect is automatically adjusted. The default is true
 * (nonzero).
 *
 * @param efs   the effect slot to determine
 * @param pauto the receiver of the auto adjust flag
 */
seal_err_t SEAL_API seal_is_efs_auto(seal_efs_t*, char* /*pauto*/);

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
