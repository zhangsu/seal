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

#include <stddef.h>
#include "raw.h"
#include "fmt.h"

/* Audio stream data. */
typedef struct seal_stream_t seal_stream_t;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Opens a stream from the passed-in file. If the stream is no longer needed,
 * call `seal_close_stream' to release the resources used by the stream.
 *
 * @param stream    the stream to open
 * @param filename  the filename of the audio
 * @param fmt       the format of the audio file; automatic recognition of the
 *                  audio format will be attempted if the passed-in `fmt' is
 *                  `SEAL_UNKNOWN_FMT'
 */
seal_err_t seal_open_stream(seal_stream_t*, const char* /*filename*/,
                            seal_fmt_t);

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
 * @param psize     the receiver of the actual size, in bytes, of streamed
 *                  data if successful or otherwise a negative integer
 */
seal_err_t seal_stream(seal_stream_t*, seal_raw_t*, size_t* /*psize*/);

/*
 * Rewinds a stream to the beginning.
 *
 * @param stream    the stream to rewind
 */
seal_err_t seal_rewind_stream(seal_stream_t*);

/*
 * Closes a stream opened by `seal_open_stream' that is not used by any
 * source.
 *
 * @param stream    the stream to close;  will be left untouched if an error
 *                  occurs
 */
seal_err_t seal_close_stream(seal_stream_t*);

#ifdef __cplusplus
}
#endif

struct seal_stream_t
{
    /* Tagged union of identifiers used by different decoder libraries. */
    void*           id;
    seal_fmt_t      fmt;
    seal_raw_attr_t attr;
};

#endif /* _SEAL_STREAM_H_ */