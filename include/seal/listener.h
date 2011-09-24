/*
 * listener.h is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 *
 * listener.h wraps up the listener singleton object, which is the abstract
 * representation of the sole listener who hears the sound.
 */

#ifndef _SEAL_LISTENER_H_
#define _SEAL_LISTENER_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Sets the master scalar amplitude multiplier of the listener which applies
 * to all the sources. 1.0f means that the sound is unattenuated; 0.5f means
 * an attenuation of 6 dB; 0.0f means silence.
 *
 * @param gain  the scalar amplitude multiplier in the interval [0.0f, +inf.)
 * @return      nonzero if successful or otherwise 0
 */
int seal_set_listener_gain(float /*gain*/);

/*
 * Sets the position of the listener in a right-handed Cartesian coordinate
 * system. Use of NaN and infinity is undefined.
 *
 * @param x     the x position to set
 * @param y     the y position to set
 * @param z     the z position to set
 */
void seal_set_listener_pos(float /*x*/, float /*y*/, float /*z*/);

/*
 * Sets the velocity of the listener in a right-handed Cartesian coordinate
 * system. The velocity of the listener does not affect its position but is a
 * factor used during the Doppler effect emulation.
 *
 * @param x     the x velocity to set
 * @param y     the y velocity to set
 * @param z     the z velocity to set
 */
void seal_set_listener_vel(float /*x*/, float /*y*/, float /*z*/);

/*
 * Sets the orientation of the listener.
 *
 * @param orien a pair of 3-tuple consisting of an 'at' vector and an 'up'
 *              vector, where the 'at' vector represents the 'forward'
 *              direction of the listener and the 'up' vector represents the
 *              'up' direction for the listener. These two vectors must be
 *              linearly independent, must not be NaN and must not be
 *              normalized. Otherwise, the operation is undefined
 * @return      nonzero if successful or otherwise 0
 */
void seal_set_listener_orien(float* /*orien*/);

/*
 * Gets the gain of the listener. The default is 1.0f.
 *
 * @see         seal_set_listener_gain
 * @return      the gain
 */
float seal_get_listener_gain(void);

/*
 * Gets the position of the listener. The default is ( 0.0f, 0.0f, 0.0f ).
 *
 * @see         seal_set_listener_pos
 * @param x     receives the x position
 * @param y     receives the y position
 * @param z     receives the z position
 */
void seal_get_listener_pos(float* /*x*/, float* /*y*/, float* /*z*/);

/*
 * Gets the velocity of the listener. The default is ( 0.0f, 0.0f, 0.0f ).
 *
 * @see         seal_set_listener_vel
 * @param x     receives the x velocity
 * @param y     receives the y velocity
 * @param z     receives the z velocity
 */
void seal_get_listener_vel(float* /*x*/, float* /*y*/, float* /*z*/);

/*
 * Gets the orientation of the listener. The default is
 * ( 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f ).
 *
 * @see seal_set_listener_orien
 * @param orien the array of a pair of 3-tuple that receives the 'at' vector
 *              and the 'up' vector
 * @return      nonzero if successful or otherwise 0
 */
void seal_get_listener_orien(float* /*orien*/);

#ifdef __cplusplus
}
#endif

#endif /* _SEAL_LISTENER_H_ */