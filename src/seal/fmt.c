#include <stdio.h>
#include <stdint.h>
#include <seal/fmt.h>
#include <seal/err.h>
#include "reader.h"

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

seal_err_t
seal_recognize_fmt(const char* filename, seal_fmt_t* pfmt)
{
    FILE* audio;
    uint32_t magic_nums[SAMPLE_SIZE];

    audio = _seal_fopen(filename);
    if (audio == 0)
        return SEAL_CANNOT_OPEN_FILE;

    /* Gets the magic numbers in little-endian. */
    _seal_read_uint32le(magic_nums, SAMPLE_SIZE, audio);

    _seal_fclose(audio);

    switch (magic_nums[0]) {
    case RIFF:
        switch (magic_nums[2]) {
        case WAVE:
            *pfmt = SEAL_WAV_FMT;
            return SEAL_OK;
        }
        break;
    case OGGS:
        *pfmt = SEAL_OV_FMT;
        return SEAL_OK;
    case APET:
        switch (magic_nums[1]) {
        case AGEX:
            *pfmt = SEAL_MPG_FMT;
            return SEAL_OK;
        }
        break;
    }

    if ((magic_nums[0] & TAG_) == TAG_ ||
        (magic_nums[0] & ID3_) == ID3_ ||
        (magic_nums[0] & SYNC) == SYNC) {
        *pfmt = SEAL_MPG_FMT;
        return SEAL_OK;
    }

    *pfmt = SEAL_UNKNOWN_FMT;

    return SEAL_BAD_AUDIO;
}

seal_err_t
seal_ensure_fmt_known(const char* filename, seal_fmt_t* pfmt)
{
    if (*pfmt == SEAL_UNKNOWN_FMT)
        return seal_recognize_fmt(filename, pfmt);

    return SEAL_OK;
}