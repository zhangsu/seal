/*
 * threading.h is part of the Scorched End Audio Library (SEAL) and is
 * licensed under the terms of the GNU Lesser General Public License.
 * See COPYING attached with the library.
 *
 * threading.h is an abstraction layer for thread-related operations, which
 * ensures SEAL's thread-safety.
 */

#ifndef _SEAL_THREADING_H_
#define _SEAL_THREADING_H_

typedef void* _seal_routine_t(void*);

/*
 * Define this to improve performance for single-threaded application or for
 * language bindings like CRuby 1.8 and 1.9 where no real concurrency can
 * exist due to the nature of green threads or the global interpreter lock.
 */
//#define SEAL_NO_THREAD_SAFETY

/* Mutex. */
void* _seal_create_lock(void);
void _seal_destroy_lock(void* /*lock*/);
void _seal_lock(void* /*lock*/);
void _seal_unlock(void* /*lock*/);

/* Thread manipulations. */
void* _seal_create_thread(_seal_routine_t*, void* /*args*/);
void _seal_join_thread(void* /*thread*/);
int _seal_calling_thread_is(void* /*thread*/);

#endif /* _SEAL_THREADING_H_ */