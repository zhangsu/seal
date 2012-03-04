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

int
seal_set_listener_gain(float gain)
{
    _seal_lock_openal();
    alListenerf(AL_GAIN, gain);
    if (_seal_chk_openal_err() == 0)
        return 0;

    return 1;
}

void
seal_set_listener_pos(float x, float y, float z)
{
    alListener3f(AL_POSITION, x, y, z);
}

void
seal_set_listener_vel(float x, float y, float z)
{
    alListener3f(AL_VELOCITY, x, y, z);
}

void
seal_set_listener_orien(float* orien)
{
    assert(orien != 0);

    alListenerfv(AL_ORIENTATION, orien);
}

float
seal_get_listener_gain(void)
{
    float gain;

    alGetListenerf(AL_GAIN, &gain);

    return gain;
}

void
seal_get_listener_pos(float* x, float* y, float* z)
{
    assert(x != 0 && y != 0 && z != 0);

    alGetListener3f(AL_POSITION, x, y, z);
}

void
seal_get_listener_vel(float* x, float* y, float* z)
{
    assert(x != 0 && y != 0 && z != 0);

    alGetListener3f(AL_VELOCITY, x, y, z);
}

void
seal_get_listener_orien(float* orien)
{
    assert(orien != 0);

    alGetListenerfv(AL_ORIENTATION, orien);
}