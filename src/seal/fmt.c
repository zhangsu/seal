/*
 * fmt.c is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 */

#include <stdio.h>
#include <seal/fmt.h>
#include <seal/err.h>
#include <assert.h>
#include "reader.h"
#include "pstdint.h"

enum
{
    RIFF = SEAL_MKTAG('R', 'I', 'F', 'F'),
    WAVE = SEAL_MKTAG('W', 'A', 'V', 'E'),
    OGGS = SEAL_MKTAG('O', 'g', 'g', 'S'),
    /* APE tag identifier (splitted). */
    APET = SEAL_MKTAG('A', 'P', 'E', 'T'),
    AGEX = SEAL_MKTAG('A', 'G', 'E', 'X'),
    /* Number of magic numbers needed to recognize the format. */
    SAMPLE_SIZE = 3
};

/* ID3v1 tag identifier. */
static const unsigned int TAG_ = SEAL_MKTAG('T', 'A', 'G', 0);
/* ID3v2 tag identifier. */
static const unsigned int ID3_ = SEAL_MKTAG('I', 'D', '3', 0);
/* MP3 Sync identifier. */
static const unsigned int SYNC = 0xf0ff;

seal_fmt_t
seal_recognize_fmt(const char* filename)
{
    FILE* audio;
    uint32_t magic_nums[SAMPLE_SIZE];

    audio = _seal_fopen(filename);
    if (audio == 0)
        return SEAL_UNKNOWN_FMT;

    /* Gets the magic numbers in little-endian. */
    _seal_read_uint32le(magic_nums, SAMPLE_SIZE, audio);

    _seal_fclose(audio);

    switch (magic_nums[0]) {
    case RIFF:
        switch (magic_nums[2]) {
        case WAVE:
            return SEAL_WAV_FMT;
        }
        break;
    case OGGS:
        return SEAL_OV_FMT;
    case APET:
        switch (magic_nums[1]) {
        case AGEX:
            return SEAL_MPG_FMT;
        }
        break;
    }

    if ((magic_nums[0] & TAG_) == TAG_ ||
        (magic_nums[0] & ID3_) == ID3_ ||
        (magic_nums[0] & SYNC) == SYNC)
        return SEAL_MPG_FMT;

    SEAL_ABORT(SEAL_BAD_AUDIO, SEAL_UNKNOWN_FMT);
}

int
seal_ensure_fmt_known(const char* filename, seal_fmt_t* pfmt)
{
    if (*pfmt == SEAL_UNKNOWN_FMT)
        *pfmt = seal_recognize_fmt(filename);

    return *pfmt != SEAL_UNKNOWN_FMT;
}