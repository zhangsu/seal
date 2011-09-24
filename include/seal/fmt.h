/*
 * fmt.h is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 *
 * fmt.h defines all the supported audio file format and provides utilities
 * to deal with them.
 */

#ifndef _SEAL_FMT_H_
#define _SEAL_FMT_H_

enum seal_fmt_t
{
    SEAL_UNKNOWN_FMT,
    SEAL_WAV_FMT,
    SEAL_OV_FMT,
    SEAL_MPG_FMT
};

/* Audio file formats. */
typedef enum seal_fmt_t seal_fmt_t;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Tries to recognize an audio file's format by peeking its header. Use of
 * this function is by no mean an accurate way to determine the exact encoding
 * of the audio file; it only considers the format directly supported by SEAL
 * as if there are only these types of format in the world. Assumes `audio' is
 * at the beginning.
 *
 * @param filename  the filename of the audio
 * @return          the format if recognized or otherwise `SEAL_UNKNOWN_FMT'
 */
seal_fmt_t seal_recognize_fmt(const char* /*filename*/);

/*
 * Ensures an audio format is known.
 *
 * @param filename  the filename of the audio to check format of
 * @param pfmt      if `*pfmt' is not `SEAL_UNKNOWN_FMT' then do nothing;
 *                  otherwise the file format is checked and `pfmt' receives
 *                  the known format
 * @return          nonzero if successful or otherwise 0
 */
int seal_ensure_fmt_known(const char* /*filename*/, seal_fmt_t* /*pfmt*/);

#ifdef __cplusplus
}
#endif

#endif /* _SEAL_FMT_H_ */