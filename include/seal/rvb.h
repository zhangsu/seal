/*
 * Interfaces for manipulating reverberation effect objects which can be loaded
 * into effect slots. The reverberation parameters can be customized to
 * emulate reverberations in different environment or can be loaded from
 * presets. The preset constants suggest the reverberation environment, for
 * example, `SEAL_ICEPALACE_LONGPASSAGE_REVERB` emulates the reverberation in
 * a long passage of an ice palace.
 *
 * For more infomation about reverberations, check out the OpenAL effect
 * extension guide at: http://zhang.su/seal/EffectsExtensionGuide.pdf
 */

#ifndef _SEAL_RVB_H_
#define _SEAL_RVB_H_

#include "err.h"

enum seal_rvb_preset_t
{
    /* Default presets. */
    SEAL_GENERIC_REVERB,
    SEAL_PADDEDCELL_REVERB,
    SEAL_ROOM_REVERB,
    SEAL_BATHROOM_REVERB,
    SEAL_LIVINGROOM_REVERB,
    SEAL_STONEROOM_REVERB,
    SEAL_AUDITORIUM_REVERB,
    SEAL_CONCERTHALL_REVERB,
    SEAL_CAVE_REVERB,
    SEAL_ARENA_REVERB,
    SEAL_HANGAR_REVERB,
    SEAL_CARPETEDHALLWAY_REVERB,
    SEAL_HALLWAY_REVERB,
    SEAL_STONECORRIDOR_REVERB,
    SEAL_ALLEY_REVERB,
    SEAL_FOREST_REVERB,
    SEAL_CITY_REVERB,
    SEAL_MOUNTAINS_REVERB,
    SEAL_QUARRY_REVERB,
    SEAL_PLAIN_REVERB,
    SEAL_PARKINGLOT_REVERB,
    SEAL_SEWERPIPE_REVERB,
    SEAL_UNDERWATER_REVERB,
    SEAL_DRUGGED_REVERB,
    SEAL_DIZZY_REVERB,
    SEAL_PSYCHOTIC_REVERB,

    /* Castle presets. */
    SEAL_CASTLE_SMALLROOM_REVERB,
    SEAL_CASTLE_SHORTPASSAGE_REVERB,
    SEAL_CASTLE_MEDIUMROOM_REVERB,
    SEAL_CASTLE_LARGEROOM_REVERB,
    SEAL_CASTLE_LONGPASSAGE_REVERB,
    SEAL_CASTLE_HALL_REVERB,
    SEAL_CASTLE_CUPBOARD_REVERB,
    SEAL_CASTLE_COURTYARD_REVERB,
    SEAL_CASTLE_ALCOVE_REVERB,

    /* Factory presets. */
    SEAL_FACTORY_SMALLROOM_REVERB,
    SEAL_FACTORY_SHORTPASSAGE_REVERB,
    SEAL_FACTORY_MEDIUMROOM_REVERB,
    SEAL_FACTORY_LARGEROOM_REVERB,
    SEAL_FACTORY_LONGPASSAGE_REVERB,
    SEAL_FACTORY_HALL_REVERB,
    SEAL_FACTORY_CUPBOARD_REVERB,
    SEAL_FACTORY_COURTYARD_REVERB,
    SEAL_FACTORY_ALCOVE_REVERB,

    /* Ice palace presets. */
    SEAL_ICEPALACE_SMALLROOM_REVERB,
    SEAL_ICEPALACE_SHORTPASSAGE_REVERB,
    SEAL_ICEPALACE_MEDIUMROOM_REVERB,
    SEAL_ICEPALACE_LARGEROOM_REVERB,
    SEAL_ICEPALACE_LONGPASSAGE_REVERB,
    SEAL_ICEPALACE_HALL_REVERB,
    SEAL_ICEPALACE_CUPBOARD_REVERB,
    SEAL_ICEPALACE_COURTYARD_REVERB,
    SEAL_ICEPALACE_ALCOVE_REVERB,

    /* Space station presets. */
    SEAL_SPACESTATION_SMALLROOM_REVERB,
    SEAL_SPACESTATION_SHORTPASSAGE_REVERB,
    SEAL_SPACESTATION_MEDIUMROOM_REVERB,
    SEAL_SPACESTATION_LARGEROOM_REVERB,
    SEAL_SPACESTATION_LONGPASSAGE_REVERB,
    SEAL_SPACESTATION_HALL_REVERB,
    SEAL_SPACESTATION_CUPBOARD_REVERB,
    SEAL_SPACESTATION_ALCOVE_REVERB,

    /* Wooden galleon presets. */
    SEAL_WOODEN_SMALLROOM_REVERB,
    SEAL_WOODEN_SHORTPASSAGE_REVERB,
    SEAL_WOODEN_MEDIUMROOM_REVERB,
    SEAL_WOODEN_LARGEROOM_REVERB,
    SEAL_WOODEN_LONGPASSAGE_REVERB,
    SEAL_WOODEN_HALL_REVERB,
    SEAL_WOODEN_CUPBOARD_REVERB,
    SEAL_WOODEN_COURTYARD_REVERB,
    SEAL_WOODEN_ALCOVE_REVERB,

    /* Sports presets. */
    SEAL_SPORT_EMPTYSTADIUM_REVERB,
    SEAL_SPORT_SQUASHCOURT_REVERB,
    SEAL_SPORT_SMALLSWIMMINGPOOL_REVERB,
    SEAL_SPORT_LARGESWIMMINGPOOL_REVERB,
    SEAL_SPORT_GYMNASIUM_REVERB,
    SEAL_SPORT_FULLSTADIUM_REVERB,
    SEAL_SPORT_STADIUMTANNOY_REVERB,

    /* Prefab presets. */
    SEAL_PREFAB_WORKSHOP_REVERB,
    SEAL_PREFAB_SCHOOLROOM_REVERB,
    SEAL_PREFAB_PRACTISEROOM_REVERB,
    SEAL_PREFAB_OUTHOUSE_REVERB,
    SEAL_PREFAB_CARAVAN_REVERB,

    /* Dome presets. */
    SEAL_DOME_TOMB_REVERB,
    SEAL_DOME_SAINTPAULS_REVERB,

    /* Pipe presets. */
    SEAL_PIPE_SMALL_REVERB,
    SEAL_PIPE_LONGTHIN_REVERB,
    SEAL_PIPE_LARGE_REVERB,
    SEAL_PIPE_RESONANT_REVERB,

    /* Outdoors presets. */
    SEAL_OUTDOORS_BACKYARD_REVERB,
    SEAL_OUTDOORS_ROLLINGPLAINS_REVERB,
    SEAL_OUTDOORS_DEEPCANYON_REVERB,
    SEAL_OUTDOORS_CREEK_REVERB,
    SEAL_OUTDOORS_VALLEY_REVERB,

    /* Mood presets. */
    SEAL_MOOD_HEAVEN_REVERB,
    SEAL_MOOD_HELL_REVERB,
    SEAL_MOOD_MEMORY_REVERB,

    /* Driving presets. */
    SEAL_DRIVING_COMMENTATOR_REVERB,
    SEAL_DRIVING_PITGARAGE_REVERB,
    SEAL_DRIVING_INCAR_RACER_REVERB,
    SEAL_DRIVING_INCAR_SPORTS_REVERB,
    SEAL_DRIVING_INCAR_LUXURY_REVERB,
    SEAL_DRIVING_FULLGRANDSTAND_REVERB,
    SEAL_DRIVING_EMPTYGRANDSTAND_REVERB,
    SEAL_DRIVING_TUNNEL_REVERB,

    /* City presets. */
    SEAL_CITY_STREETS_REVERB,
    SEAL_CITY_SUBWAY_REVERB,
    SEAL_CITY_MUSEUM_REVERB,
    SEAL_CITY_LIBRARY_REVERB,
    SEAL_CITY_UNDERPASS_REVERB,
    SEAL_CITY_ABANDONED_REVERB,

    /* Misc. presets. */
    SEAL_DUSTYROOM_REVERB,
    SEAL_CHAPEL_REVERB,
    SEAL_SMALLWATERROOM_REVERB
};

typedef struct seal_rvb_t seal_rvb_t;
typedef enum seal_rvb_preset_t seal_rvb_preset_t;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Initializes a new reverb effect. If the reverb is no longer needed, call
 * `seal_destroy_rvb' to release the resources used by the reverb.
 *
 * There is a limit on the number of allocated reverbs. This function returns
 * an error if it is exceeding the limit.
 *
 * @param reverb    the reverb object to initialize
 */
seal_err_t seal_init_rvb(seal_rvb_t*);

/*
 * Destroys a reverb effect.
 *
 * @param reverb    the reverb to destroy
 */
seal_err_t seal_destroy_rvb(seal_rvb_t*);

/*
 * Loads the specified reverb paramter preset into a reverb object.
 *
 * @param reverb    the reverb to load the preset into
 * @param preset    the preset to load
 */
seal_err_t seal_load_rvb(seal_rvb_t*, seal_rvb_preset_t);

/*
 * Sets the modal density of a reverb in the interval [0.0f, 1.0f]. The
 * density controls the coloration of the late reverb. The Lower the value,
 * the more coloration.
 *
 * @param reverb    the reverb to set the density of
 * @param density   the density
 */
seal_err_t seal_set_rvb_density(seal_rvb_t*, float /*density*/);

/*
 * Sets the diffusion of a reverb in the interval [0.0f, 1.0f]. The diffusion
 * controls the echo density in the reverberation decay. Reducing diffusion
 * gives the reverberation a more "grainy" character that is especially
 * noticeable with percussive sound sources. If you set a diffusion value of
 * 0.0f, the later reverberation sounds like a succession of distinct echoes.
 *
 * @param reverb    the reverb to set the diffusion of
 * @param diffusion the diffusion
 */
seal_err_t seal_set_rvb_diffusion(seal_rvb_t*, float /*diffusion*/);

/*
 * Sets the gain of a reverb in the interval [0.0f, 1.0f], or from -100 dB (no
 * reflected sound at all) to 0 dB (the maximum amount). The gain is the
 * master volume control for the reflected sound (both early reflections and
 * reverberation) that the reverb effect adds to all sources. It sets the
 * maximum amount of reflections and reverberation added to the final sound
 * mix.
 *
 * @param reverb    the reverb to set the gain of
 * @param gain      the gain
 */
seal_err_t seal_set_rvb_gain(seal_rvb_t*, float /*gain*/);

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
 */
seal_err_t seal_set_rvb_hfgain(seal_rvb_t*, float /*hfgain*/);

/*
 * Sets the decay time of a reverb in the interval [0.1f, 20.0f], typically
 * from a small room with very dead surfaces to a large room with very live
 * surfaces.
 *
 * @param reverb    the reverb to set the decay time of
 * @param time      the decay time
 */
seal_err_t seal_set_rvb_decay_time(seal_rvb_t*, float /*time*/);

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
 * @param ratio         the decay high-frequency ratio
 */
seal_err_t seal_set_rvb_hfdecay_ratio(seal_rvb_t*, float /*ratio*/);

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
 */
seal_err_t seal_set_rvb_reflections_gain(seal_rvb_t*, float /*gain*/);

/*
 * Sets the reflections delay of a reverb in the interval [0.0f, 0.3f] (in
 * seconds). It is the amount of delay between the arrival time of the direct
 * path from the source to the first reflection from the source. You can
 * reduce or increase this delay to simulate closer or more distant reflective
 * surfaces and therefore control the perceived size of the room.
 *
 * @param reverb    the reverb to set the reflections delay of
 * @param delay     the reflections delay
 */
seal_err_t seal_set_rvb_reflections_delay(seal_rvb_t*, float /*delay*/);

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
 */
seal_err_t seal_set_rvb_late_gain(seal_rvb_t*, float /*gain*/);

/*
 * Sets the late delay of a reverb in the interval [0.0f, 0.1f] (in second)
 * It defines the begin time of the late reverberation relative to the time of
 * the initial reflection (the first of the early reflections). Reducing or
 * increasing late delay is useful for simulating a smaller or larger room.
 *
 * @param reverb    the reverb to set the late delay of
 * @param delay     the late delay
 */
seal_err_t seal_set_rvb_late_delay(seal_rvb_t*, float /*delay*/);

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
 */
seal_err_t seal_set_rvb_air_absorbtion_hfgain(seal_rvb_t*, float /*hfgain*/);

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
 */
seal_err_t seal_set_rvb_room_rolloff_factor(seal_rvb_t*, float /*factor*/);

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
 */
seal_err_t seal_set_rvb_hfdecay_limited(seal_rvb_t*, char /*limited*/);

/*
 * Gets the density of a reverb. The default is 1.0f.
 *
 * @see             seal_set_rvb_density
 * @param reverb    the source to get the density of
 * @param pdensity  the receiver of the density
 */
seal_err_t seal_get_rvb_density(seal_rvb_t*, float* /*pdensity*/);

/*
 * Gets the diffusion of a reverb. The default is 1.0f.
 *
 * @see                 seal_set_rvb_diffusion
 * @param reverb        the source to get the diffusion of
 * @param pdiffusion    the receiver of diffusion
 */
seal_err_t seal_get_rvb_diffusion(seal_rvb_t*, float* /*pdiffusion*/);

/*
 * Gets the gain of a reverb. The default is 0.32f.
 *
 * @see             seal_set_rvb_gain
 * @param reverb    the source to get the gain of
 * @param pgain     the receiver of the gain
 */
seal_err_t seal_get_rvb_gain(seal_rvb_t*, float* /*pgain*/);

/*
 * Gets the high-frequency gain of a reverb. The default is 0.89f.
 *
 * @see             seal_set_rvb_hfgain
 * @param reverb    the source to get the high-frequency gain of
 * @param phfgain   the receiver of the high-frequency gain
 */
seal_err_t seal_get_rvb_hfgain(seal_rvb_t*, float* /*phfgain*/);

/*
 * Gets the decay time of a reverb. The default is 1.49f.
 *
 * @see             seal_set_rvb_decay_time
 * @param reverb    the source to get the decay time of
 * @param ptime     the receiver of the decay time
 */
seal_err_t seal_get_rvb_decay_time(seal_rvb_t*, float* /*ptime*/);

/*
 * Gets the high-frequency decay ratio of a reverb. The default is 0.83f.
 *
 * @see             seal_set_rvb_hfdecay_ratio
 * @param reverb    the source to get the high-frequency decay time of
 * @param pratio    the receiver of the high-frequency decay ratio
 */
seal_err_t seal_get_rvb_hfdecay_ratio(seal_rvb_t*, float* /*pratio*/);

/*
 * Gets the reflections gain of a reverb. The default is 0.05f.
 *
 * @see             seal_set_rvb_reflections_gain
 * @param reverb    the source to get the reflections gain of
 * @param pgain     the receiver of the reflections gain
 */
seal_err_t seal_get_rvb_reflections_gain(seal_rvb_t*, float* /*pgain*/);

/*
 * Gets the reflections delay of a reverb. The default is 0.007f.
 *
 * @see             seal_set_rvb_reflections_delay
 * @param reverb    the source to get the reflections delay of
 * @param pdelay    the receiver of the reflections delay
 */
seal_err_t seal_get_rvb_reflections_delay(seal_rvb_t*, float* /*pdelay*/);

/*
 * Gets the late gain of a reverb. The default is 1.26f.
 *
 * @see             seal_set_rvb_late_gain
 * @param reverb    the source to get the late gain of
 * @param pgain     the receiver of the late gain
 */
seal_err_t seal_get_rvb_late_gain(seal_rvb_t*, float* /*pgain*/);

/*
 * Gets the late delay of a reverb. The default is 0.011f.
 *
 * @see             seal_set_rvb_late_delay
 * @param reverb    the source to get the late delay of
 * @param pdelay    the receiver of the late delay
 */
seal_err_t seal_get_rvb_late_delay(seal_rvb_t*, float* /*pdelay*/);

/*
 * Gets the air absorbtion high-frequency gain of a reverb. The default is
 * 0.994f.
 *
 * @see             seal_set_rvb_air_absorbtion_hfgain
 * @param reverb    the source to get the gain of
 * @param phfgain   the receiver of the air absorbtion high-frequency gain
 */
seal_err_t seal_get_rvb_air_absorbtion_hfgain(seal_rvb_t*,
                                                 float* /*phfgain*/);

/*
 * Gets the room rolloff factor of a reverb. The default is 0.0f.
 *
 * @see             seal_set_rvb_room_rolloff_factor
 * @param reverb    the source to get the factor of
 * @param pfactor   the receiver of the room rolloff factor
 */
seal_err_t seal_get_rvb_room_rolloff_factor(seal_rvb_t*,
                                               float* /*pfactor*/);

/*
 * Determines if the high-frequency decay of a reverb is limited. The default
 * is true (1).
 *
 * @see             seal_set_rvb_hfdecay_limited
 * @param reverb    the source to get the limited flag of
 * @param plimited  the receiver of the limited flag
 */
seal_err_t seal_is_rvb_hfdecay_limited(seal_rvb_t*, char* /*plimited*/);

#ifdef __cplusplus
}
#endif

/*
 *****************************************************************************
 * Below are **implementation details**.
 *****************************************************************************
 */

struct seal_rvb_t
{
    unsigned int id;
};

#endif /* _SEAL_RVB_H_ */