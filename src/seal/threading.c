/*
 * threading.c is part of the Scorched End Audio Library (SEAL) and is
 * licensed under the terms of the GNU Lesser General Public License. See
 * COPYING attached with the library.
 */

#include <seal/threading.h>
#include <seal/core.h>
#include <seal/pstdint.h>

#if defined (SEAL_NO_THREAD_SAFETY)

_seal_lock_t _seal_create_lock(void) { return 0; }
void _seal_destroy_lock(_seal_lock_t placeholder) {}
void _seal_lock(_seal_lock_t placeholder) {}
void _seal_unlock(_seal_lock_t placeholder) {}

_seal_tls_t
_seal_alloc_tls(void)
{
    return _seal_malloc(sizeof (uintptr_t));
}

void
_seal_free_tls(_seal_tls_t tls)
{
    _seal_free(tls);
}

void
_seal_set_tls(_seal_tls_t tls, void* value)
{
    *((uintptr_t*) tls) = (uintptr_t) value;
}

void*
_seal_get_tls(_seal_tls_t tls)
{
    return (void*) *(uintptr_t*) tls;
}

#elif defined (__unix__)

#include <pthread.h>

_seal_lock_t
_seal_create_lock(void)
{
    pthread_mutex_t* lock = _seal_malloc(sizeof (pthread_mutex_t));
    pthread_mutex_init(lock, 0);

    return lock;
}

void
_seal_destroy_lock(_seal_lock_t lock)
{
    pthread_mutex_destroy((pthread_mutex_t*) lock);
}

void
_seal_lock(_seal_lock_t lock)
{
    pthread_mutex_lock((pthread_mutex_t*) lock);
}

void
_seal_unlock(_seal_lock_t lock)
{
    pthread_mutex_unlock((pthread_mutex_t*) lock);
}

_seal_tls_t
_seal_alloc_tls()
{
    pthread_key_t* tls = _seal_malloc(sizeof (pthread_key_t));
    pthread_key_create(tls, 0);

    return tls;
}

void
_seal_free_tls(_seal_tls_t tls)
{
    pthread_key_delete(*(pthread_key_t*) tls);
    _seal_free(tls);
}

void
_seal_set_tls(_seal_tls_t tls, void* value)
{
    pthread_setspecific(*(pthread_key_t*) tls, value);
}

void*
_seal_get_tls(_seal_tls_t tls)
{
    return pthread_getspecific(*(pthread_key_t*) tls);
}

#elif defined (_WIN32)

#include <Windows.h>

_seal_lock_t
_seal_create_lock(void)
{
    return CreateMutexA(0, 0, 0);
}

void
_seal_destroy_lock(_seal_lock_t lock)
{
    CloseHandle(lock);
}

void
_seal_lock(_seal_lock_t lock)
{
    WaitForSingleObject(lock, INFINITE);
}

void
_seal_unlock(_seal_lock_t lock)
{
    ReleaseMutex(lock);
}

_seal_tls_t
_seal_alloc_tls()
{
    return (_seal_tls_t) TlsAlloc();
}

void
_seal_free_tls(_seal_tls_t tls)
{
    TlsFree((DWORD) tls);
}

void
_seal_set_tls(_seal_tls_t tls, void* value)
{
    TlsSetValue((DWORD) tls, value);
}

void*
_seal_get_tls(_seal_tls_t tls)
{
    return TlsGetValue((DWORD) tls);
}

#endif /* SEAL_NO_THREAD_SAFETY, __unix__, _WIN32 */

#if defined (__unix__)

#include <unistd.h>

void
_seal_sleep(unsigned int millisec)
{
    usleep(millisec * 1000);
}

#elif defined (_WIN32)

void
_seal_sleep(unsigned int millisec)
{
    Sleep(millisec);
}

#endif /* __unix__, _WIN32 */
