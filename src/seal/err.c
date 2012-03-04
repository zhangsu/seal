/*
 * err.c is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 */

#ifndef NDEBUG
# include <stdio.h>
#endif
#include <al/al.h>
#include <seal/err.h>
#include <seal/core.h>
#include "threading.h"

/* Thread-local error state. Initialized and uninitialized in core.c. */
_seal_tls_t _seal_err;

seal_err_t
seal_get_err(void)
{
    seal_err_t err;

    err = seal_peek_err();
    _seal_set_tls(_seal_err, (void*) SEAL_OK);

    return err;
}

seal_err_t
seal_peek_err(void)
{
    return (seal_err_t) _seal_get_tls(_seal_err);
}

const char*
seal_get_err_msg(seal_err_t err)
{
    switch (err) {
    case SEAL_OK:
        return 0;

    case SEAL_BAD_OBJ:
        return "Invalid object";
    case SEAL_BAD_ENUM:
        return "Invalid token";
    case SEAL_BAD_VAL:
        return "Invalid parameter value";
    case SEAL_BAD_OP:
        return "Invalid operation";

    case SEAL_CANNOT_OPEN_DEVICE:
        return "Cannot open the specified device";
    case SEAL_NO_EFX:
        return "The effect module is not found";
    case SEAL_NO_EXT_FUNC:
        return "The required extension functions are not found";
    case SEAL_BAD_DEVICE:
        return "Invalid device";
    case SEAL_CANNOT_CREATE_CONTEXT:
        return "Cannot create an additional context for the device";

    case SEAL_STREAM_UNOPENED:
        return "Cannot use the uninitialized stream";
    case SEAL_STREAM_ALREADY_OPENED:
        return "Cannot re-initialize the already-opened stream";
    case SEAL_STREAM_INUSE:
        return "The operation cannot be done as the stream is still in use";
    case SEAL_MIXING_STREAM_FMT:
        return "Cannot attach a stream with a different audio format than "
               "that of the currently attached stream";

    case SEAL_MIXING_SRC_TYPE:
        return "Cannot attach a stream to a static source or "
               "attach a buffer to a streaming source";

    case SEAL_CANNOT_OPEN_FILE:
        return "Cannot open the specified file";
    case SEAL_CANNOT_ALLOC_MEM:
        return "Cannot allocate additional memory";

    case SEAL_BAD_AUDIO:
        return "The specified audio file format is unsupported";
    case SEAL_BAD_WAV_SUBTYPE:
        return "The specified WAVE file has an unsupported subtype";
    case SEAL_BAD_WAV_CHUNK:
        return "The specified WAVE file has an invalid chunk";
    case SEAL_BAD_WAV_CHUNK_SIZE:
        return "The specified WAVE file has an invalid chunk size";
    case SEAL_ABSENT_WAV_CHUNK:
        return "The specified WAVE file is missing a required chunk";
    case SEAL_BAD_WAV_BPS:
        return "The specified WAVE file has an unsupported bit depth";
    case SEAL_BAD_WAV_NCHANNELS:
        return "The specified WAVE file has an invalid number of channels";
    case FILE_BAD_WAV_FREQ:
        return "The specified WAVE file has an invalid sample rate";

    case SEAL_CANNOT_OPEN_OV:
        return "Failed openning the specified Ogg Vorbis file";
    case SEAL_CANNOT_GET_OV_INFO:
        return "Failed getting info from the specified Ogg Vorbis file";
    case SEAL_CANNOT_READ_OV:
        return "Failed reading the specified Ogg Vorbis file";

    case SEAL_CANNOT_INIT_MPG:
        return "Failed initializing MPEG decoder";
    case SEAL_CANNOT_GET_MPG_INFO:
        return "Failed getting info from the specified MPEG file";
    case SEAL_CANNOT_READ_MPG:
        return "Failed reading the specified MPEG file";

    case SEAL_CANNOT_OPEN_MID:
        return "Failed openning the specified MIDI file "
               "or initializing the MIDI device";
    case SEAL_CANNOT_PLAY_MID:
        return "Failed playing the specified MIDI file";

    default:
        return "Unkown error";
    }
}

void
_seal_set_err(seal_err_t err)
{
    _seal_set_tls(_seal_err, (void*) err);
#ifndef NDEBUG
    if (err != SEAL_OK)
        fprintf(stderr, "[SEAL][DEBUG] %s\n", seal_get_err_msg(err));
#endif
}

/*
 * Always call `_seal_lock_openal' before calling an OpenAL function that could
 * possibly raise an OpenAL error, and then call this function to get the
 * error which will automatically release the lock.
 */
int
_seal_chk_openal_err(void)
{
    seal_err_t err;

    switch (alGetError()) {
    case AL_INVALID_NAME:
        err = SEAL_BAD_OBJ;
        break;
    case AL_INVALID_ENUM:
        err = SEAL_BAD_ENUM;
        break;
    case AL_INVALID_VALUE:
        err = SEAL_BAD_VAL;
        break;
    case AL_INVALID_OPERATION:
        err = SEAL_BAD_OP;
        break;
    case AL_OUT_OF_MEMORY:
        err = SEAL_CANNOT_ALLOC_MEM;
        break;
    default:
        err = SEAL_OK;
    }
    _seal_unlock_openal();

    if (err != SEAL_OK) {
        _seal_set_err(err);

        return 0;
    }

    return 1;
}
