/*
 * ov.h is part of the Scorched End Audio Library (SEAL) and is licensed under
 * the terms of the GNU Lesser General Public License. See COPYING attached
 * with the library.
 *
 * ov.h provides subroutines to decode ogg vorbis files.
 */

#ifndef _SEAL_OV_H_
#define _SEAL_OV_H_

#include "raw.h"
#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif

int _seal_load_ov(seal_raw_t*, const char* /*filename*/);
seal_stream_t* _seal_init_ov_stream(seal_stream_t*, const char* /*filename*/);
int _seal_stream_ov(seal_stream_t*, seal_raw_t*);
void _seal_rewind_ov_stream(seal_stream_t*);
void _seal_close_ov_stream(seal_stream_t*);

#ifdef __cplusplus
}
#endif

#endif /* _SEAL_OV_H_ */
