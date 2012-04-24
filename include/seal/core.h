/*
 * core.h is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 *
 * core.h defines interfaces for low-level SEAL operations.
 */

#ifndef _SEAL_CORE_H_
#define _SEAL_CORE_H_

#include <stddef.h>
#include "err.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Initializes SEAL by specifying the device name. This function is not
 * re-entrant nor thread-safe and should be called only once per SEAL session.
 * Match a call to `seal_startup' with a call to `seal_cleanup' and never call
 * `seal_starup' twice in a row.
 *
 * @param device_name   the name of a device; 0 to use the default one
 */
seal_err_t seal_startup(const char* /*device_name*/);

/* Uninitializes SEAL and invalidate all SEAL objects. Thread-unsafe. */
void seal_cleanup(void);

/*
 * @return  the maximum number of effect slots a source can feed concurrently.
 */
int seal_get_per_src_effect_limit(void);

/*
 * Gets the SEAL version string.
 *
 * @return  the version string
 */
const char* seal_get_version(void);

#ifdef __cplusplus
}
#endif

/*
 *****************************************************************************
 * Below are **implementation details**.
 *****************************************************************************
 */

void _seal_sleep(unsigned int millisec);

/* Common types. */
typedef void _seal_openal_initializer_t(int, unsigned int*);
typedef void _seal_openal_destroyer_t(int, const unsigned int*);
typedef char _seal_openal_validator_t(unsigned int);
typedef void _seal_openal_setterf(unsigned int, int, float);
typedef void _seal_openal_getterf(unsigned int, int, float*);
typedef void _seal_openal_setteri(unsigned int, int, int);
typedef void _seal_openal_getteri(unsigned int, int, int*);

/* Common helpers. */
unsigned int _seal_openal_id(void*);
seal_err_t _seal_gen_objs(int, unsigned int*, _seal_openal_initializer_t*);
seal_err_t _seal_delete_objs(int, const unsigned int*,
                             _seal_openal_destroyer_t*);
seal_err_t _seal_init_obj(void*, _seal_openal_initializer_t*);
seal_err_t _seal_destroy_obj(void*, _seal_openal_destroyer_t*,
                             _seal_openal_validator_t*);
seal_err_t _seal_setf(void*, int, float, _seal_openal_setterf*);
seal_err_t _seal_getf(void*, int, float*, _seal_openal_getterf*);
seal_err_t _seal_seti(void*, int, int, _seal_openal_setteri*);
seal_err_t _seal_geti(void*, int, int*, _seal_openal_getteri*);
seal_err_t _seal_getb(void*, int, char*, _seal_openal_getteri*);

/* OpenAL effect extension functions. */
extern _seal_openal_initializer_t* alGenEffects;
extern _seal_openal_destroyer_t* alDeleteEffects;
extern _seal_openal_validator_t* alIsEffect;
extern _seal_openal_setterf* alEffectf;
extern void (*alEffecti)(unsigned int, int, int);
extern _seal_openal_getterf* alGetEffectf;
extern void (*alGetEffecti)(unsigned int, int, int*);
extern _seal_openal_initializer_t* alGenAuxiliaryEffectSlots;
extern _seal_openal_destroyer_t* alDeleteAuxiliaryEffectSlots;
extern _seal_openal_validator_t* alIsAuxiliaryEffectSlot;
extern void (*alAuxiliaryEffectSloti)(unsigned int, int, int);
extern void (*alAuxiliaryEffectSlotf)(unsigned int, int, float);
extern void (*alGetAuxiliaryEffectSloti)(unsigned int, int, int*);
extern void (*alGetAuxiliaryEffectSlotf)(unsigned int, int, float*);

#endif /* _SEAL_CORE_H_ */
