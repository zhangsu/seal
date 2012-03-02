/*
 * reader.h is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 *
 * reader.h provides utilities related to file input.
 */

#ifndef _SEAL_READER_H_
#define _SEAL_READER_H_

#include <stddef.h>
#include "pstdint.h"

/* Makes a 32-bit tag in little-endian. No multiple evaluations. */
#define SEAL_MKTAG(a, b, c, d) ((a) | (b) << 8 | (c) << 16 | (d) << 24)

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @param filename  the filename to open
 * @return          the opened file handle
 */
void* _seal_fopen(const char* /*filename*/);

/*
 * @param file      the file handle to close
 */
void _seal_fclose(void* /*file*/);

/*
 * Reads unsigned 16-bit integers in little-endian.
 *
 * @param buf       the array of integers to receive the data
 * @param size      the size of the array
 * @param file      the file input stream
 */
void _seal_read_uint16le(uint16_t* /*buf*/, size_t, void* /*file*/);

/*
 * Reads unsigned 32-bit integers in little-endian.
 *
 * @param buf       the array of integers to receive the data
 * @param size      the size of the array
 * @param file      the file input stream
 */
void _seal_read_uint32le(uint32_t* /*buf*/, size_t, void* /*file*/);

/*
 * Skips `nbytes' bytes (seek forward).
 *
 * @param nbytes    the number of bytes to skip
 * @param file      the file input stream
 */
void _seal_skip(uint32_t /*nbytes*/, void* /*file*/);

#ifdef __cplusplus
}
#endif

#endif /* _SEAL_READER_H_ */
