/*
 * threading.h is an abstraction layer for thread-related operations, which
 * ensures Seal's thread-safety.
 */

#ifndef _SEAL_THREADING_H_
#define _SEAL_THREADING_H_

typedef void* _seal_routine_t(void*);

/* Thread manipulations. */
void* _seal_create_thread(_seal_routine_t*, void* /*args*/);
void _seal_join_thread(void* /*thread*/);
int _seal_calling_thread_is(void* /*thread*/);

#endif /* _SEAL_THREADING_H_ */