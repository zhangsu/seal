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

#include <stddef.h>
#include <seal/raw.h>
#include <seal/stream.h>

seal_err_t _seal_load_wav(seal_raw_t*, const char* /*filename*/);
seal_err_t _seal_init_wav_stream(seal_stream_t*, const char* /*filename*/);
seal_err_t _seal_stream_wav(seal_stream_t*, seal_raw_t*, size_t* /*psize*/);
seal_err_t _seal_rewind_wav_stream(seal_stream_t*);
seal_err_t _seal_close_wav_stream(seal_stream_t*);

#endif /* _SEAL_WAV_H_ */
