/*
 * ov.h is part of the Scorched End Audio Library (SEAL) and is licensed under
 * the terms of the GNU Lesser General Public License. See COPYING attached
 * with the library.
 *
 * ov.h provides subroutines to decode ogg vorbis files.
 */

#ifndef _SEAL_OV_H_
#define _SEAL_OV_H_

#include <stddef.h>
#include <seal/raw.h>
#include <seal/stream.h>

seal_err_t _seal_load_ov(seal_raw_t*, const char* /*filename*/);
seal_err_t _seal_init_ov_stream(seal_stream_t*, const char* /*filename*/);
seal_err_t _seal_stream_ov(seal_stream_t*, seal_raw_t*, size_t* /*psize*/);
seal_err_t _seal_rewind_ov_stream(seal_stream_t*);
seal_err_t _seal_close_ov_stream(seal_stream_t*);

#endif /* _SEAL_OV_H_ */
