/*
 * threading.c is part of the Scorched End Audio Library (SEAL) and is
 * licensed under the terms of the GNU Lesser General Public License. See
 * COPYING attached with the library.
 */

#include <stdlib.h>
#include <stdint.h>
#include "threading.h"

#if defined (__unix__) || defined (__APPLE_CC__)
# include <pthread.h>
# include <unistd.h>

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
# define _WIN32_WINNT 0x0500
# include <Windows.h>

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
