/*
 * buf.h is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 *
 * buf.h wraps up the abstract data type `seal_buf_t'. Buffers are essentially
 * abstract representations of the (raw) audio data and are used by sources.
 * Buffers are most suitable for small-sized sound effect which can be
 * efficiently loaded to memory at once. Streams, on the other hand, are more
 * suitable for long audio such as background music.
 */

#ifndef _SEAL_BUF_H_
#define _SEAL_BUF_H_

#include <stddef.h>
#include "raw.h"
#include "fmt.h"

typedef struct seal_buf_t seal_buf_t;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Allocates a new buffer.
 *
 * @return  the newly allocated buffer if successful or otherwise 0
 */
seal_buf_t* seal_alloc_buf(void);

/*
 * Initializes an already-allocated buffer by loading audio from a file. Will
 * not try to free the buffer upon failure.
 *
 * @param buf       the buffer to initialize
 * @param filename  the audio resource filename; extension is trivial
 * @param fmt       the format of the audio file; automatic recognition of the
 *                  audio format will be tried if the format is
 *                  `SEAL_UNKNOWN_FMT'
 * @return          the initialized buffer if successful or otherwise 0
 */
seal_buf_t* seal_init_buf(seal_buf_t*, const char* /*filename*/, seal_fmt_t);

/*
 * Allocates and initializes a buffer by loading audio from a file.
 *
 * @param filename  the audio resource filename; extension is trivial
 * @param fmt       the format of the audio file; automatic recognition of the
 *                  audio format will be tried if the format is
 *                  `SEAL_UNKNOWN_FMT'
 * @return          the newly-initialized buffer if successful or otherwise 0
 */
seal_buf_t* seal_new_buf(const char* /*filename*/, seal_fmt_t);

/*
 * Deallocates a buffer that is not currently used by any source.
 *
 * @param buf   the buffer to deallocate
 * @return      nonzero if successful or otherwise 0
 */
int seal_free_buf(seal_buf_t*);

/*
 * Loads audio from a file to a buffer that is not currently used by any
 * source. Sets all the attributes appropriately.
 *
 * @param buf       the buffer to receive the loaded data
 * @param filename  the filename of the audio
 * @param fmt       the format of the audio file; automatic recognition of the
 *                  audio format will be attempted if `format' is
 *                  `SEAL_UNKNOWN_FMT'
 * @return          nonzero if successful or otherwise 0
 */
int seal_load2buf(seal_buf_t*, const char* /*filename*/, seal_fmt_t);

/*
 * Copies raw PCM data to a buffer that is not currently used by any source.
 * Sets all the attributes appropriately.
 *
 * @param buf   the destination buffer to receive the copied data
 * @param raw   the source raw data to copy
 * @return      nonzero if successful or otherwise 0
 */
int seal_raw2buf(seal_buf_t*, seal_raw_t*);

/*
 * Gets the size, in bytes, of a buffer. The default is 0.
 *
 * @param buf   the buffer to retrive the size of
 * @return      the size
 */
size_t seal_get_buf_size(seal_buf_t*);

/*
 * Gets the frequency (sample rate) of the audio contained in a buffer. The
 * default is 0.
 *
 * @param buf   the buffer to retrive the frequency of
 * @return      the frequency
 */
int seal_get_buf_freq(seal_buf_t*);

/*
 * Gets the bit depth (bits per sample) of the audio contained in a buffer.
 * The default is 16.
 *
 * @param buf   the buffer to retrive the bit depth of
 * @return      the bit depth
 */
int seal_get_buf_bps(seal_buf_t*);

/*
 * Gets the number of channels of the audio contained in a buffer. The default
 * is 1.
 *
 * @param buf   the buffer to retrive the number of channels of
 * @return      the number of channels
 */
int seal_get_buf_nchannels(seal_buf_t*);

/*
 * Loads an audio file and extracts the whole PCM data to memory. This is the
 * front-end for loading various audio formats.
 *
 * @param raw       the receiver of the loaded PCM data; `raw->data' will be
 *                  dynamically allocated so the caller is responsible for
 *                  deallocating it
 * @param filename  the filename of the audio
 * @param fmt       the format of the audio file; automatic recognition of the
 *                  audio format will be attempted if the passed-in `fmt' is
 *                  `SEAL_UNKNOWN_FMT'
 * @return          nonzero if successful or otherwise 0
 */
int seal_load(seal_raw_t*, const char* /*filename*/, seal_fmt_t);

#ifdef __cplusplus
}
#endif

/*
 * Gets the OpenAL buffer format based on the number of channels and bit
 * depth.
 *
 * @param nchannels number of channels
 * @param bit_depth bits per sample
 * @return          one of:
 *                  AL_FORMAT_MONO8
 *                  AL_FORMAT_MONO16
 *                  AL_FORMAT_STEREO8
 *                  AL_FORMAT_STEREO16
 *                  if successful or otherwise 0
 */
int _seal_get_buf_fmt(int /*nchannels*/, int /*bit_depth*/);

#endif /* _SEAL_BUF_H_ */
