/*
 * threading.h is part of the Scorched End Audio Library (SEAL) and is
 * licensed under the terms of the GNU Lesser General Public License.
 * See COPYING attached with the library.
 *
 * threading.h is an abstraction layer for thread-related operations, which
 * ensures SEAL's thread-safety and is intended for internal use only (so
 * treat all these as implementation details).
 */

#ifndef _SEAL_THREADING_H_
#define _SEAL_THREADING_H_

typedef void* _seal_lock_t;
typedef void* _seal_tls_t;
typedef void* _seal_thread_t;
typedef void* _seal_routine(void*);

/*
 * Define this to improve performance for single-threaded application or for
 * language bindings like CRuby 1.8 and 1.9 where no real concurrency can
 * exist due to the nature of green threads or the global interpreter lock.
 */
//#define SEAL_NO_THREAD_SAFETY

#ifdef __cplusplus
extern "C" {
#endif

/* Mutex. */
_seal_lock_t _seal_create_lock(void);
void _seal_destroy_lock(_seal_lock_t);
void _seal_lock(_seal_lock_t);
void _seal_unlock(_seal_lock_t);

/* Thread-local storage. */
_seal_tls_t _seal_alloc_tls(void);
void _seal_free_tls(_seal_tls_t);
void _seal_set_tls(_seal_tls_t, void* /*value*/);
void* _seal_get_tls(_seal_tls_t);

/* Thread manipulations. */
_seal_thread_t _seal_create_thread(_seal_routine*, void* /*args*/);
void _seal_join_thread(_seal_thread_t);
int _seal_calling_thread_is(_seal_thread_t);

#ifdef __cplusplus
}
#endif

#endif /* _SEAL_THREADING_H_ */