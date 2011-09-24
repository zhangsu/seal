/*
 * mid.h is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 *
 * mid.h provides interfaces for MIDI handling. MIDI playback will not have
 * 3D spatialization; most of the time, like multi-channel digital PCM audio,
 * MIDI serves as background music and need not 3D spatialization anyway.
 */

#ifndef _SEAL_MID_H_
#define _SEAL_MID_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Initializes MIDI component.
 *
 * @return          nonzero if successful or otherwise 0
 */
int seal_midi_startup(void);

/*
 * Plays a MIDI file.
 *
 * @param filename  the filename of the MIDI file.
 * @return          nonzero if successful or otherwise 0
 */
int seal_play_midi(const char* /*filename*/);

/*
 * Pauses the playback of the currently playing MIDI file (if any).
 *
 * @return          nonzero if successful or otherwise 0
 */
int seal_pause_midi(void);

/*
 * Resumes the paused playback of the currently playing MIDI file (if any).
 *
 * @return          nonzero if successful or otherwise 0
 */
int seal_resume_midi(void);

/*
 * Stops the playback of the currently playing MIDI file (if any).
 *
 * @return          nonzero if successful or otherwise 0
 */
int seal_stop_midi(void);

/*
 * Rewinds the playback of the currently playing MIDI file (if any).
 *
 * @return          nonzero if successful or otherwise 0
 */
int seal_rewind_midi(void);

/* Uninitialize MIDI component. */
void seal_midi_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif /* _SEAL_MID_H_ */