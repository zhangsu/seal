/*
 * reader.c is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 */

#ifdef _WIN32
# include <Windows.h>
#endif
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <seal/err.h>
#include "reader.h"

enum
{
    JUNK_BUF_SIZE = 512
};

/* Helpers that convert raw bytes to little-endian 16- and 32-bit integers. */
static uint16_t
raw2le16(uint8_t* bytes)
{
    return bytes[1] << 8 | bytes[0];
}

static uint32_t
raw2le32(uint8_t* bytes)
{
    return bytes[3] << 24 | bytes[2] << 16 | raw2le16(bytes);
}

FILE*
_seal_fopen(const char* filename)
{
    FILE* file;

#ifdef _WIN32
    wchar_t wfilename[260];
    MultiByteToWideChar(CP_UTF8, 0, filename, -1, wfilename, 260);
    file = _wfopen(wfilename, L"rb");
#else
    file = fopen(filename, "rb");
#endif

    return file;
}

void
_seal_fclose(FILE* file)
{
    fclose(file);
}

/*
 * A template for the following two functions. All arguments except `FILE'
 * will have multiple evaluations in one call.
 */
#define READ_UINT_LE(nbits, buf, size, file) do                             \
{                                                                           \
    size_t _i_;                                                             \
                                                                            \
    assert((buf) != 0 && (size) > 0);                                       \
                                                                            \
    fread((buf), sizeof (uint##nbits##_t), (size), (file));                 \
    for (_i_ = 0; _i_ < (size); ++_i_) {                                    \
        *(buf) = raw2le##nbits((uint8_t*) (buf));                           \
        ++(buf);                                                            \
    }                                                                       \
} while (0)

void
_seal_read_uint16le(uint16_t* buf, size_t size, void* file)
{
    READ_UINT_LE(16, buf, size, file);
}

void
_seal_read_uint32le(uint32_t* buf, size_t size, void* file)
{
    READ_UINT_LE(32, buf, size, file);
}

/*
 * Use of static variable avoids the overhead brought by dynamic memory
 * allocations and deallocations. Microsoft's implementation of fseek flushes
 * the buffer, which is evil and why fread is used here.
 */
void
_seal_skip(uint32_t nbytes, void* file)
{
    uint32_t i;
    static uint8_t junk[JUNK_BUF_SIZE];

    assert(file != 0);

    for (i = JUNK_BUF_SIZE; i <= nbytes; i += JUNK_BUF_SIZE)
        fread(junk, 1, JUNK_BUF_SIZE, file);
    nbytes %= JUNK_BUF_SIZE;
    if (nbytes > 0)
        fread(junk, 1, nbytes, file);
}
