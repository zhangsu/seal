/*
 * reverb.h is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 *
 * reverb.h defines the reverberation effect type and its parameters. Effect
 * objects can be put into an effect slot for sources to use.
 */

#ifndef _SEAL_REVERB_H_
#define _SEAL_REVERB_H_

typedef struct seal_reverb_t seal_reverb_t;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Allocates a new reverb effect.
 *
 * @return  the newly allocated reverb if successful or otherwise 0
 */
seal_reverb_t* seal_alloc_reverb(void);

/*
 * Deallocates a reverb effect.
 *
 * @param buf   the reverb to deallocate
 * @return      nonzero if successful or otherwise 0
 */
int seal_free_reverb(seal_reverb_t*);

/*
 * Sets the modal density of a reverb in the interval [0.0f, 1.0f]. The
 * density controls the coloration of the late reverb. The Lower the value,
 * the more coloration.
 *
 * @param reverb    the reverb to set the density of
 * @param density   the density
 * @return          nonzero if successful or otherwise 0
 */
int seal_set_reverb_density(seal_reverb_t*, float /*density*/);

/*
 * Sets the diffusion of a reverb in the interval [0.0f, 1.0f]. The diffusion
 * controls the echo density in the reverberation decay. Reducing diffusion
 * gives the reverberation a more "grainy" character that is especially
 * noticeable with percussive sound sources. If you set a diffusion value of
 * 0.0f, the later reverberation sounds like a succession of distinct echoes.
 *
 * @param reverb    the reverb to set the diffusion of
 * @param diffusion the diffusion
 * @return          nonzero if successful or otherwise 0
 */
int seal_set_reverb_diffusion(seal_reverb_t*, float /*diffusion*/);

/*
 * Sets the gain of a reverb in the interval [0.0f, 1.0f], or from -100 dB (no
 * reflected sound at all) to 0 dB (the maximum amount). The gain is the master
 * volume control for the reflected sound (both early reflections and
 * reverberation) that the reverb effect adds to all sources. It sets the
 * maximum amount of reflections and reverberation added to the final sound
 * mix.
 *
 * @param reverb    the reverb to set the gain of
 * @param gain      the gain
 * @return          nonzero if successful or otherwise 0
 */
int seal_set_reverb_gain(seal_reverb_t*, float /*gain*/);

/*
 * Sets the high-frequency gain of a reverb in the interval [0.0f, 1.0f], or
 * from -100 dB (virtually no reflected sound) to 0 dB (no filter). The high-
 * frequency gain further tweaks reflected sound by attenuating it at high
 * frequencies. It controls a low-pass filter that applies globally to the
 * reflected sound of all sound sources feeding the particular instance of the
 * reverb effect.
 *
 * @param reverb    the reverb to set the high-frequency gain of
 * @param hfgain    the high-frequency gain
 * @return          nonzero if successful or otherwise 0
 */
int seal_set_reverb_hfgain(seal_reverb_t*, float /*hfgain*/);

/*
 * Sets the decay time of a reverb in the interval [0.1f, 20.0f], typically
 * from a small room with very dead surfaces to a large room with very live
 * surfaces.
 *
 * @param reverb        the reverb to set the decay time of
 * @param decay_time    the decay time
 * @return              nonzero if successful or otherwise 0
 */
int seal_set_reverb_decay_time(seal_reverb_t*, float /*decay_time*/);

/*
 * Sets the high-frequency decay ratio, or the spectral quality of the decay
 * time of a reverb in the interval [0.1, 20.0]. It is the ratio of high-
 * frequency decay time relative to the time set by decay Time. 1.0 means
 * neutral: the decay time is equal for all frequencies. As this value
 * increases above 1.0, the high-frequency decay time increases so it's longer
 * than the decay time at low frequencies. You hear a more brilliant
 * reverberation with a longer decay at high frequencies. As this value
 * decreases below 1.0, the high-frequency decay time decreases so it's
 * shorter than the decay time of the low frequencies. You hear a more natural
 * reverberation.
 *
 * @param reverb        the reverb to set the decay high-frequency ratio of
 * @param decay_hfratio the decay high-frequency ratio
 * @return              nonzero if successful or otherwise 0
 */
int seal_set_reverb_hfdecay_ratio(seal_reverb_t*, float /*decay_hfratio*/);

/*
 * Sets the reflections gain, or the overall amount of initial reflections
 * relative to the gain of a reverb in the interval [0.0f, 3.16f], or from
 * -100 dB (no initial reflections at all) to 10 dB. The reflections gain is
 * corrected by the value of the gain property and does not affect the
 * subsequent reverberation decay.
 *
 * You can increase the amount of initial reflections to simulate a more
 * narrow space or closer walls, especially effective if you associate the
 * initial reflections increase with a reduction in reflections delays by
 * lowering the value of the reflection delay property. To simulate open or
 * semi-open environments, you can maintain the amount of early reflections
 * while reducing the value of the late gain property, which controls later
 * reflections.
 *
 * @param reverb    the reverb to set the reflections gain of
 * @param gain      the reflections gain
 * @return          nonzero if successful or otherwise 0
 */
int seal_set_reverb_reflections_gain(seal_reverb_t*, float /*gain*/);

/*
 * Sets the reflections delay of a reverb in the interval [0.0f, 0.3f] (in
 * seconds). It is the amount of delay between the arrival time of the direct
 * path from the source to the first reflection from the source. You can
 * reduce or increase this delay to simulate closer or more distant reflective
 * surfaces and therefore control the perceived size of the room.
 *
 * @param reverb    the reverb to set the reflections delay of
 * @param delay     the reflections delay
 * @return          nonzero if successful or otherwise 0
 */
int seal_set_reverb_reflections_delay(seal_reverb_t*, float /*delay*/);

/*
 * Sets the late gain, or the overall amount of later reverberation relative
 * to the gain of a reverb in the interval [0.0f, 10.0f], or from -100 dB (no
 * late reverberation at all) to 20 dB.
 *
 * Note that late gain and decay time are independent properties: if you
 * adjust decay time without changing late gain, the total intensity (the
 * averaged square of the amplitude) of the late reverberation remains
 * constant.
 *
 * @param reverb    the reverb to set the late gain of
 * @param gain      the late gain
 * @return          nonzero if successful or otherwise 0
 */
int seal_set_reverb_late_gain(seal_reverb_t*, float /*gain*/);

/*
 * Sets the late delay of a reverb in the interval [0.0f, 0.1f] (in second)
 * It defines the begin time of the late reverberation relative to the time of
 * the initial reflection (the first of the early reflections). Reducing or
 * increasing late delay is useful for simulating a smaller or larger room.
 *
 * @param reverb    the reverb to set the late delay of
 * @param delay     the late delay
 * @return          nonzero if successful or otherwise 0
 */
int seal_set_reverb_late_delay(seal_reverb_t*, float /*delay*/);

/*
 * Sets the air absorption high-frequency gain of a reverb in the interval
 * [0.892f, 1.0f]. It controls the distance-dependent attenuation at high
 * frequencies caused by the propagation medium. It applies to reflected sound
 * only. You can use this value to simulate sound transmission through foggy
 * air, dry air, smoky atmosphere, and so on. The default value 0.994
 * (-0.05 dB) per meter, which roughly corresponds to typical condition of
 * atmospheric humidity, temperature, and so on. Lowering the value simulates
 * a more absorbent medium (more humidity in the air, for example); raising
 * the value simulates a less absorbent medium (dry desert air, for example).
 *
 * @param reverb    the reverb to set the air absorption gain of
 * @param gain      the air absorption high-frequency gain
 * @return          nonzero if successful or otherwise 0
 */
int seal_set_reverb_air_absorbtion_hfgain(seal_reverb_t*, float /*hfgain*/);

/*
 * Sets the room rolloff factor of a reverb in the interval [0.0f, 10.0f]. It
 * is one of two methods available to attenuate the reflected sound
 * (containing both reflections and reverberation) according to source-
 * listener distance. It is defined the same way as the global rolloff factor,
 * but operates on reverb sound instead of direct-path sound. Setting the room
 * rolloff factor value to 1.0 specifies that the reflected sound will decay
 * by 6 dB every time the distance doubles. Any value other than 1.0 is
 * equivalent to a scaling factor applied to the quantity specified by
 * ((Source listener distance) - (Reference Distance)). Reference Distance is
 * an OpenAL source parameter that specifies the inner border for distance
 * rolloff effects: if the source comes closer to the listener than the
 * reference distance, the direct-path sound isn't increased as the source
 * comes closer to the listener, and neither is the reflected sound.
 *
 * The default value of Room Rolloff Factor is 0.0 because, by default, the
 * reverb effect naturally manages the reflected sound level automatically for
 * each sound source to simulate the natural rolloff of reflected sound vs.
 * distance in typical rooms. (Note that this isn't the case if the source
 * property @TODO is set to false.) You can use this value as an option to
 * automatic control so you can exaggerate or replace the default
 * automatically-controlled rolloff.
 *
 * @param reverb    the reverb to set the room rolloff factor of
 * @param factor    the room rolloff factor
 * @return          nonzero if successful or otherwise 0
 */
int seal_set_reverb_room_rolloff_factor(seal_reverb_t*, float /*factor*/);

/*
 * Sets whether the high-frequency decay time automatically stays below a
 * limit value that's derived from the setting of the air absorption high-
 * frequency gain. This limit applies regardless of the setting of the
 * decay high-frequency ratio, and the limit doesn't affect the value of decay
 * high-frequency ratio. This limit, when on, maintains a natural sounding
 * reverberation decay by allowing you to increase the value of decay time
 * without the risk of getting an unnaturally long decay time at high
 * frequencies. If this flag is set to false, high-frequency decay time isn't
 * automatically limited.
 *
 * @param reverb    the reverb to set the high-frequency decay limit flag of
 * @param limited   1 to set it limited or otherwise 0
 * @return          nonzero if successful or otherwise 0
 */
int seal_set_reverb_hfdecay_limited(seal_reverb_t*, char /*limited*/);

/*
 * Gets the density of a reverb. The default is 1.0f.
 *
 * @see             seal_set_reverb_density
 * @param reverb    the source to get the density of
 * @return          the density if successful or otherwise 0
 */
float seal_get_reverb_density(seal_reverb_t*);

/*
 * Gets the diffusion of a reverb. The default is 1.0f.
 *
 * @see             seal_set_reverb_diffusion
 * @param reverb    the source to get the diffusion of
 * @return          the diffusion if successful or otherwise 0
 */
float seal_get_reverb_diffusion(seal_reverb_t*);

/*
 * Gets the gain of a reverb. The default is 0.32f.
 *
 * @see             seal_set_reverb_gain
 * @param reverb    the source to get the gain of
 * @return          the gain if successful or otherwise 0
 */
float seal_get_reverb_gain(seal_reverb_t*);

/*
 * Gets the high-frequency gain of a reverb. The default is 0.89f.
 *
 * @see             seal_set_reverb_hfgain
 * @param reverb    the source to get the high-frequency gain of
 * @return          the high-frequency gain if successful or otherwise 0
 */
float seal_get_reverb_hfgain(seal_reverb_t*);

/*
 * Gets the decay time of a reverb. The default is 1.49f.
 *
 * @see             seal_set_reverb_decay_time
 * @param reverb    the source to get the decay time of
 * @return          the decay time if successful or otherwise 0
 */
float seal_get_reverb_decay_time(seal_reverb_t*);

/*
 * Gets the high-frequency decay ratio of a reverb. The default is 0.83f.
 *
 * @see             seal_set_reverb_hfdecay_ratio
 * @param reverb    the source to get the high-frequency decay time of
 * @return          the high-frequency decay ratio if successful or
 *                  otherwise 0
 */
float seal_get_reverb_hfdecay_ratio(seal_reverb_t*);

/*
 * Gets the reflections gain of a reverb. The default is 0.05f.
 *
 * @see             seal_set_reverb_reflections_gain
 * @param reverb    the source to get the reflections gain of
 * @return          the reflections gain if successful or otherwise 0
 */
float seal_get_reverb_reflections_gain(seal_reverb_t*);

/*
 * Gets the reflections delay of a reverb. The default is 0.007f.
 *
 * @see             seal_set_reverb_reflections_delay
 * @param reverb    the source to get the reflections delay of
 * @return          the reflections delay if successful or otherwise 0
 */
float seal_get_reverb_reflections_delay(seal_reverb_t*);

/*
 * Gets the late gain of a reverb. The default is 1.26f.
 *
 * @see             seal_set_reverb_late_gain
 * @param reverb    the source to get the late gain of
 * @return          the late gain if successful or otherwise 0
 */
float seal_get_reverb_late_gain(seal_reverb_t*);

/*
 * Gets the late delay of a reverb. The default is 0.011f.
 *
 * @see             seal_set_reverb_late_delay
 * @param reverb    the source to get the late delay of
 * @return          the late delay if successful or otherwise 0
 */
float seal_get_reverb_late_delay(seal_reverb_t*);

/*
 * Gets the air absorbtion high-frequency gain of a reverb. The default is
 * 0.994f.
 *
 * @see             seal_set_reverb_air_absorbtion_hfgain
 * @param reverb    the source to get the gain of
 * @return          the air absorbtion high-frequency gain if successful or
 *                  otherwise 0
 */
float seal_get_reverb_air_absorbtion_hfgain(seal_reverb_t*);

/*
 * Gets the room rolloff factor of a reverb. The default is 0.0f.
 *
 * @see             seal_set_reverb_room_rolloff_factor
 * @param reverb    the source to get the factor of
 * @return          the room rolloff factor if successful or otherwise 0
 */
float seal_get_reverb_room_rolloff_factor(seal_reverb_t*);

/*
 * Determines if the high-frequency decay of a reverb is limited. The default
 * is true (return 1).
 *
 * @see             seal_set_reverb_hfdecay_limited
 * @param reverb    the source to get the limited flag of
 * @return          nonzero if it is limited and successful or otherwise 0
 */
char seal_is_reverb_hfdecay_limited(seal_reverb_t*);

#ifdef __cplusplus
}
#endif

#endif /* _SEAL_REVERB_H_ */