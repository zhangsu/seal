/*
 * mpg.h is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 *
 * mpg.h provides subroutines to decode MPEG 1.0, 2.0 and 2.5 audio for layers
 * 1, 2 and 3.
 */

#ifndef _SEAL_MPG_H_
#define _SEAL_MPG_H_

#include <stddef.h>
#include <seal/raw.h>
#include <seal/stream.h>
#include <seal/err.h>

seal_err_t _seal_load_mpg(seal_raw_t*, const char* /*filename*/);
seal_err_t _seal_init_mpg_stream(seal_stream_t*, const char* /*filename*/);
seal_err_t _seal_stream_mpg(seal_stream_t*, seal_raw_t*, size_t* /*psize*/);
seal_err_t _seal_rewind_mpg_stream(seal_stream_t*);
seal_err_t _seal_close_mpg_stream(seal_stream_t*);

#endif /* _SEAL_MPG_H_ */