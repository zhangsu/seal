/*
 * listener.c is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 */

#include <al/al.h>
#include <seal/listener.h>
#include <seal/core.h>
#include <seal/err.h>
#include <assert.h>

static int
setf(int key, float value)
{
    _seal_lock_openal();
    alListenerf(key, value);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return 1;
}

static int
set3f(int key, float x, float y, float z)
{
    _seal_lock_openal();
    alListener3f(key, x, y, z);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return 1;
}

static int
setfv(int key, float* vector)
{
    assert(vector != 0);

    _seal_lock_openal();
    alListenerfv(key, vector);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return 1;
}

static float
getf(int key)
{
    float value;

    _seal_lock_openal();
    alGetListenerf(key, &value);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return value;
}

static int
get3f(int key, float* px, float* py, float* pz)
{
    assert(px != 0 && py != 0 && pz != 0);

    _seal_lock_openal();
    alGetListener3f(key, px, py, pz);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return 1;
}

static int
getfv(int key, float* vector)
{
    assert(vector != 0);

    _seal_lock_openal();
    alGetListenerfv(key, vector);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return 1;
}

int
seal_set_listener_gain(float gain)
{
    return setf(AL_GAIN, gain);
}

int
seal_set_listener_pos(float x, float y, float z)
{
    return set3f(AL_POSITION, x, y, z);
}

int
seal_set_listener_vel(float x, float y, float z)
{
    return set3f(AL_VELOCITY, x, y, z);
}

int
seal_set_listener_orien(float* orien)
{
    return setfv(AL_ORIENTATION, orien);
}

float
seal_get_listener_gain(void)
{
    return getf(AL_GAIN);
}

int
seal_get_listener_pos(float* x, float* y, float* z)
{
    return get3f(AL_POSITION, x, y, z);
}

int
seal_get_listener_vel(float* x, float* y, float* z)
{
    return get3f(AL_VELOCITY, x, y, z);
}

int
seal_get_listener_orien(float* orien)
{
    return getfv(AL_ORIENTATION, orien);
}