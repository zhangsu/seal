/*
 * mid.c is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 */

#include <seal/err.h>
#include <seal/mid.h>

int
seal_midi_startup(void)
{
    return 1;
}

int
seal_play_midi(const char* filename)
{
    filename = filename;

    return 1;
}

int
seal_pause_midi(void)
{
    return 1;
}

int
seal_resume_midi(void)
{
    return 1;
}


int
seal_stop_midi(void)
{
    return 1;
}

int
seal_rewind_midi(void)
{
    return 1;
}

void
seal_midi_cleanup(void)
{
}
