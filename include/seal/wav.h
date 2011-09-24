/*
 * wav.h is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 *
 * wav.h provides subroutines to decode WAVE audio files. Currently only
 * uncompressed waveform data with no more than 16-bit depth are supported.
 */

#ifndef _SEAL_WAV_H_
#define _SEAL_WAV_H_

#include "raw.h"
#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif

int _seal_load_wav(seal_raw_t*, const char* /*filename*/);
seal_stream_t* _seal_init_wav_stream(seal_stream_t*,
                                     const char* /*filename*/);
int _seal_stream_wav(seal_stream_t*, seal_raw_t*);
void _seal_rewind_wav_stream(seal_stream_t*);
void _seal_close_wav_stream(seal_stream_t*);

#ifdef __cplusplus
}
#endif

#endif /* _SEAL_WAV_H_ */
