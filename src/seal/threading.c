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

#elif defined (_POSIX_VERSION)

#include <pthread.h>

_seal_lock_t
_seal_create_lock(void)
{
    // TODO
    return 0;
}

void
_seal_destroy_lock(_seal_lock_t lock)
{
    // TODO
}

void
_seal_lock(_seal_lock_t lock)
{
    // TODO
}

void
_seal_unlock(_seal_lock_t lock)
{
    // TODO
}

_seal_tls_t
_seal_alloc_tls()
{
    // TODO
    return 0;
}

void
_seal_free_tls(_seal_tls_t tls)
{
    // TODO
}

void
_seal_set_tls(_seal_tls_t tls, void* value)
{
    // TODO
}

void*
_seal_get_tls(_seal_tls_t tls)
{
    // TODO
    return 0;
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
    return (void*) TlsAlloc();
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

#endif /* SEAL_NO_THREAD_SAFETY, _POSIX_VERSION, _WIN32 */

#if defined (_POSIX_VERSION)

void
_seal_sleep(unsigned int millisec)
{
    // TODO
}

#elif defined (_WIN32)

void
_seal_sleep(unsigned int millisec)
{
    Sleep(millisec);
}

#endif /* _POSIX_VERSION, _WIN32 */