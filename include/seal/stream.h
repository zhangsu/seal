/*
 * stream.h is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 *
 * stream.h wraps up the `seal_stream_t' data type used by streaming sources.
 * It is the front end for various decoders.
 */

#ifndef _SEAL_STREAM_H_
#define _SEAL_STREAM_H_

#include "raw.h"
#include "fmt.h"

/* Audio stream data. */
typedef struct seal_stream_t seal_stream_t;

struct seal_stream_t
{
    /* Tagged union of identifiers used by different decoder libraries. */
    void*           id;
    seal_fmt_t      fmt;
    seal_raw_attr_t attr;
    /* Reserved for internal use; users should not access this field. */
    unsigned int    in_use;
};

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Allocates an uninitialized stream.
 *
 * @return  the allocated stream
 */
seal_stream_t* seal_alloc_stream(void);

/*
 * Initializes an already-allocated stream from the passed-in file.
 *
 * @param stream    the already-allocated stream
 * @param filename  the filename of the audio
 * @param fmt       the format of the audio file; automatic recognition of the
 *                  audio format will be attempted if the passed-in `fmt' is
 *                  `SEAL_UNKNOWN_FMT'
 * @return          the initialized stream if successful or otherwise 0
 */
seal_stream_t* seal_init_stream(seal_stream_t*, const char* /*filename*/,
                                seal_fmt_t);

/*
 * Opens a stream from the passed-in file.
 *
 * @param filename  the filename of the audio
 * @param fmt       the format of the audio file; automatic recognition of the
 *                  audio format will be attempted if the passed-in `fmt' is
 *                  `SEAL_UNKNOWN_FMT'
 * @return          the stream if successful or otherwise 0
 */
seal_stream_t* seal_open_stream(const char* /*filename*/, seal_fmt_t);

/*
 * Streams from an opened stream.
 *
 * @param stream    the opened stream to stream from; will be left untouched
 *                  if an error occurs
 * @param raw       the receiver of the streamed PCM data; if the size of the
 *                  streamed data is not zero, `raw->data' will be dynamically
 *                  allocated so the caller is responsible for deallocating
 *                  it; `raw->size' should be set to the desired size, in
 *                  bytes, of PCM data to stream prior to this call and will
 *                  be adjusted to the actual size, if not zero, of streamed
 *                  data; will be left untouched if an error occurs
 * @return          actual size, in bytes, of streamed data if successful or
 *                  a negative integer
 */
int seal_stream(seal_stream_t*, seal_raw_t*);

/*
 * Rewinds a stream to the beginning.
 *
 * @param stream    the stream to rewind
 */
void seal_rewind_stream(seal_stream_t*);

/*
 * Closes a stream opened by `seal_init_stream' or `seal_open_stream' that
 * is not used by any source.
 *
 * @param stream    the stream to close;  will be left untouched if an error
 *                  occurs
 * @return          nonzero if successful or otherwise 0
 */
int seal_close_stream(seal_stream_t*);

/*
 * Deallocates a stream allocated by `seal_alloc_stream' or `seal_open_stream'
 * that is not used by any source. Automatically calls `seal_close_stream'.
 *
 * @param stream    the stream to deallocate;  will be left untouched if an
 *                  error occurs
 * @return          nonzero if successful or otherwise 0
 */
int seal_free_stream(seal_stream_t*);

#ifdef __cplusplus
}
#endif

#endif /* _SEAL_STREAM_H_ */