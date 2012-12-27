/*
 * Interfaces for manipulating buffers. Buffers are essentially abstract
 * representations of (raw) audio data and are used by sources. Buffers are
 * most suitable for small-sized sound effect which can be efficiently loaded
 * to memory at once. Streams, on the other hand, are more suitable for massive
 * audio such as background music.
 *
 * In order to have 3D sound effect on the buffer, the audio file needs to have
 * mono-channel. If the audio file has multi-channel, the sound will not be
 * positioned in a 3D space. Multi-channel audio (a.k.a. stereo) is already
 * designed to have illusion of directionality and audible perspective. Most
 * sound effect should be monophonic.
 */

#ifndef _SEAL_BUF_H_
#define _SEAL_BUF_H_

#include <stddef.h>
#include "raw.h"
#include "fmt.h"
#include "err.h"

typedef struct seal_buf_t seal_buf_t;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Initializes a new buffer. If the buffer is no longer needed, call
 * `seal_destroy_buf' to release the resources used by the buffer.
 *
 * There is a limit on the number of allocated buffer. This function returns an
 * error if it is exceeding the limit.
 *
 * @param buf   the buffer to initialize
 */
seal_err_t seal_init_buf(seal_buf_t*);

/*
 * Destroys a buffer that is not currently used by any source.
 *
 * @param buf   the buffer to destroy
 */
seal_err_t seal_destroy_buf(seal_buf_t*);

/*
 * Loads audio from a file to a buffer that is not currently used by any
 * source. Sets all the attributes appropriately.
 *
 * @param buf       the buffer to receive the loaded data
 * @param filename  the filename of the audio
 * @param fmt       the format of the audio file; automatic recognition of the
 *                  audio format will be attempted if `fmt' is
 *                  `SEAL_UNKNOWN_FMT'
 */
seal_err_t seal_load2buf(seal_buf_t*, const char* /*filename*/, seal_fmt_t);

/*
 * Copies raw PCM data to a buffer that is not currently used by any source.
 * Sets all the attributes appropriately.
 *
 * @param buf   the destination buffer to receive the copied data
 * @param raw   the source raw data to copy
 */
seal_err_t seal_raw2buf(seal_buf_t*, seal_raw_t*);

/*
 * Gets the size, in bytes, of a buffer. The default is 0.
 *
 * @param buf   the buffer to retrive the size of
 * @param psize the receiver of the size
 */
seal_err_t seal_get_buf_size(seal_buf_t*, int* /*psize*/);

/*
 * Gets the frequency (sample rate) of the audio contained in a buffer. The
 * default is 0.
 *
 * @param buf   the buffer to retrive the frequency of
 * @param pfreq the receiver of the frequency
 */
seal_err_t seal_get_buf_freq(seal_buf_t*, int* /*pfreq*/);

/*
 * Gets the bit depth (bits per sample) of the audio contained in a buffer.
 * The default is 16.
 *
 * @param buf   the buffer to retrive the bit depth of
 * @param pbps  the receiver of the bit depth
 */
seal_err_t seal_get_buf_bps(seal_buf_t*, int* /*pbps*/);

/*
 * Gets the number of channels of the audio contained in a buffer. The default
 * is 1.
 *
 * @param buf           the buffer to retrive the number of channels of
 * @param pnchannels    the receiver of the number of channels
 */
seal_err_t seal_get_buf_nchannels(seal_buf_t*, int* /*pnchannels*/);

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
 */
seal_err_t seal_load(seal_raw_t*, const char* /*filename*/, seal_fmt_t);

#ifdef __cplusplus
}
#endif

/*
 *****************************************************************************
 * Below are **implementation details**.
 *****************************************************************************
 */

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
 */
int _seal_get_buf_fmt(int /*nchannels*/, int /*bit_depth*/);

seal_err_t _seal_raw2buf(unsigned int /*buf*/, seal_raw_t*);

struct seal_buf_t
{
    unsigned int id;
};

#endif /* _SEAL_BUF_H_ */
