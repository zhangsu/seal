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
 * @return              nonzero if successful or otherwise 0
 */
int seal_startup(const char* /*device_name*/);

/* Uninitializes SEAL and invalidate all SEAL objects. Thread-unsafe. */
void seal_cleanup(void);

/*
 * Gets the maximum number of effects a source can apply concurrently.
 *
 * @return  the version string
 */
int seal_get_neffects_per_src(void);

/*
 * Gets the SEAL version string.
 *
 * @return  the version string
 */
const char* seal_get_verion(void);

#ifdef __cplusplus
}
#endif

/* IMPLEMENTATION DETAILS STARTS FROM HERE. */

void _seal_sleep(unsigned int millisec);

/*
 * OpenAL's error state is not thread-safe and so semaphores are needed unless
 * SEAL_NO_THREAD_SAFETY is defined. Ideally we should lock only before
 * setting the error and unlock after retrieving the error, but that requires
 * a modification to the OpenAL implementation being used...
 */
void _seal_lock_openal(void);
void _seal_unlock_openal(void);

/* Memory operations. */
void* _seal_malloc(size_t);
void* _seal_calloc(size_t /*count*/, size_t /*size*/);
void* _seal_realloc(void* /*mem*/, size_t);
void _seal_free(void* /*mem*/);

/* Common helpers. */
typedef void _seal_openal_allocator_t(int, unsigned int*);
typedef void _seal_openal_deallocator_t(int, const unsigned int*);
typedef char _seal_openal_validator_t(unsigned int);
void* _seal_alloc_obj(size_t, _seal_openal_allocator_t*);
int _seal_free_obj(void* /*obj*/, _seal_openal_deallocator_t*,
                   _seal_openal_validator_t*);

/* OpenAL effect extension functions. */
extern _seal_openal_allocator_t* alGenEffects;
extern _seal_openal_deallocator_t* alDeleteEffects;
extern _seal_openal_validator_t* alIsEffect;
extern void (*alEffectf)(unsigned int, int, float);
extern void (*alEffecti)(unsigned int, int, int);
extern void (*alGetEffectf)(unsigned int, int, float*);
extern void (*alGetEffecti)(unsigned int, int, int*);
extern _seal_openal_allocator_t* alGenAuxiliaryEffectSlots;
extern _seal_openal_deallocator_t* alDeleteAuxiliaryEffectSlots;
extern _seal_openal_validator_t* alIsAuxiliaryEffectSlot;
extern void (*alAuxiliaryEffectSloti)(unsigned int, int, int);
extern void (*alAuxiliaryEffectSlotf)(unsigned int, int, float);
extern void (*alGetAuxiliaryEffectSloti)(unsigned int, int, int*);
extern void (*alGetAuxiliaryEffectSlotf)(unsigned int, int, float*);

#endif /* _SEAL_CORE_H_ */
