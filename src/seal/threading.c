/*
 * threading.c is part of the Scorched End Audio Library (SEAL) and is
 * licensed under the terms of the GNU Lesser General Public License. See
 * COPYING attached with the library.
 */

#include <stdlib.h>
#include <stdint.h>
#include <seal/core.h>
#include "threading.h"

#if defined (SEAL_NO_THREAD_SAFETY)

void* _seal_create_lock(void) { return 0; }
void _seal_destroy_lock(void* placeholder) {}
void _seal_lock(void* placeholder) {}
void _seal_unlock(void* placeholder) {}

#elif defined (__unix__)

#include <pthread.h>

void*
_seal_create_lock(void)
{
    pthread_mutex_t* lock = malloc(sizeof (pthread_mutex_t));
    pthread_mutex_init(lock, 0);

    return lock;
}

void
_seal_destroy_lock(void* lock)
{
    pthread_mutex_destroy(lock);

    free(lock);
}

void
_seal_lock(void* lock)
{
    pthread_mutex_lock(lock);
}

void
_seal_unlock(void* lock)
{
    pthread_mutex_unlock(lock);
}

#elif defined (_WIN32)

#define _WIN32_WINNT 0x0500
#include <Windows.h>

void*
_seal_create_lock(void)
{
    return CreateMutexA(0, 0, 0);
}

void
_seal_destroy_lock(void* lock)
{
    CloseHandle(lock);
}

void
_seal_lock(void* lock)
{
    WaitForSingleObject(lock, INFINITE);
}

void
_seal_unlock(void* lock)
{
    ReleaseMutex(lock);
}

#endif /* SEAL_NO_THREAD_SAFETY, __unix__, _WIN32 */

#if defined (__unix__)

#include <unistd.h>

void*
_seal_create_thread(_seal_routine_t* routine, void* args)
{
    pthread_t thread;

    pthread_create(&thread, 0, routine, args);

    return (void*) thread;
}

void
_seal_join_thread(void* thread)
{
    pthread_join((pthread_t) thread, 0);
}

int
_seal_calling_thread_is(void* thread)
{
    return pthread_self() == (pthread_t) thread;
}

#elif defined (_WIN32)

void*
_seal_create_thread(_seal_routine_t* routine, void* args)
{
    DWORD thread;

    CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE) routine,
                             args, 0, &thread));

    return (void*) thread;
}

void
_seal_join_thread(void* thread)
{
    HANDLE thread_handle = OpenThread(SYNCHRONIZE, 0, (DWORD) thread);
    WaitForSingleObject(thread_handle, INFINITE);
    CloseHandle(thread_handle);
}

int
_seal_calling_thread_is(void* thread)
{
    return GetCurrentThreadId() == (DWORD) thread;
}

#endif /* __unix__, _WIN32 */
