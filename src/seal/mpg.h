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

#include <seal/raw.h>
#include <seal/stream.h>

#ifdef __cplusplus
extern "C" {
#endif

int _seal_load_mpg(seal_raw_t*, const char* /*filename*/);
seal_stream_t* _seal_init_mpg_stream(seal_stream_t*,
                                     const char* /*filename*/);
int _seal_stream_mpg(seal_stream_t*, seal_raw_t*);
void _seal_rewind_mpg_stream(seal_stream_t*);
void _seal_close_mpg_stream(seal_stream_t*);

#ifdef __cplusplus
}
#endif

#endif /* _SEAL_MPG_H_ */