/*
 * Interfaces for manipulating the listener singleton object. The listener
 * object abstractly represents the main object in a sound application which
 * "hears" all the sound. For example, the listener object can be used to
 * represent the main character moving around on the map in a role-playing
 * game. The properties of the listener (position, velocity, etc.) combined
 * with those of the existing sources determine how the sound should be
 * rendered.
 */

#ifndef _SEAL_LISTENER_H_
#define _SEAL_LISTENER_H_

#include "err.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Sets the master scalar amplitude multiplier of the listener which applies
 * to all the sources. 1.0f means that the sound is unattenuated; 0.5f means
 * an attenuation of 6 dB; 0.0f means silence.
 *
 * @param gain  the scalar amplitude multiplier in the interval [0.0f, +inf.)
 */
seal_err_t SEAL_API seal_set_listener_gain(float /*gain*/);

/*
 * Sets the position of the listener in a right-handed Cartesian coordinate
 * system. Use of NaN and infinity is undefined.
 *
 * @param x     the x position to set
 * @param y     the y position to set
 * @param z     the z position to set
 */
seal_err_t SEAL_API seal_set_listener_pos(
    float /*x*/,
    float /*y*/,
    float /*z*/
);

/*
 * Sets the velocity of the listener in a right-handed Cartesian coordinate
 * system. The velocity of the listener does not affect its position but is a
 * factor used during the Doppler effect emulation.
 *
 * @param x     the x velocity to set
 * @param y     the y velocity to set
 * @param z     the z velocity to set
 */
seal_err_t SEAL_API seal_set_listener_vel(
    float /*x*/,
    float /*y*/,
    float /*z*/
);

/*
 * Sets the orientation of the listener.
 *
 * @param orien a pair of 3-tuple consisting of an 'at' vector and an 'up'
 *              vector, where the 'at' vector represents the 'forward'
 *              direction of the listener and the 'up' vector represents the
 *              'up' direction for the listener. These two vectors must be
 *              linearly independent, must not be NaN and must not be
 *              normalized. Otherwise, the operation is undefined
 */
seal_err_t SEAL_API seal_set_listener_orien(float* /*orien*/);

/*
 * Gets the gain of the listener. The default is 1.0f.
 *
 * @see         seal_set_listener_gain
 * @param pgain the receiver of the gain
 */
seal_err_t SEAL_API seal_get_listener_gain(float* /*pgain*/);

/*
 * Gets the position of the listener. The default is ( 0.0f, 0.0f, 0.0f ).
 *
 * @see         seal_set_listener_pos
 * @param px    the receiver of the x position
 * @param py    the receiver of the y position
 * @param pz    the receiver of the z position
 */
seal_err_t SEAL_API seal_get_listener_pos(
    float* /*px*/,
    float* /*py*/,
    float* /*pz*/
);

/*
 * Gets the velocity of the listener. The default is ( 0.0f, 0.0f, 0.0f ).
 *
 * @see         seal_set_listener_vel
 * @param px    the receiver of the x velocity
 * @param py    the receiver of the y velocity
 * @param pz    the receiver of the z velocity
 */
seal_err_t SEAL_API seal_get_listener_vel(
    float* /*px*/,
    float* /*py*/,
    float* /*pz*/
);

/*
 * Gets the orientation of the listener. The default is
 * ( 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f ).
 *
 * @see seal_set_listener_orien
 * @param orien the array of a pair of 3-tuple that receives the 'at' vector
 *              and the 'up' vector
 */
seal_err_t SEAL_API seal_get_listener_orien(float* /*orien*/);

#ifdef __cplusplus
}
#endif

#endif /* _SEAL_LISTENER_H_ */