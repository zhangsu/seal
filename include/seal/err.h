/*
 * err.h is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 *
 * err.h defines SEAL errors and provides subroutines for clearing and
 * retriving errors. SEAL functions will set a thread-local error flag when
 * error strikes, so if the last SEAL function call returns 0, the exact error
 * and detailed error message can be retrieved using functions in this module.
 */

#ifndef _SEAL_ERR_H_
#define _SEAL_ERR_H_

#include <al/al.h>

enum seal_err_t
{
    SEAL_OK,

    SEAL_BAD_OBJ,
    SEAL_BAD_ENUM,
    SEAL_BAD_VAL,
    SEAL_BAD_OP,

    SEAL_CANNOT_OPEN_DEVICE,
    SEAL_NO_EFX,
    SEAL_NO_EXT_FUNC,
    SEAL_BAD_DEVICE,
    SEAL_CANNOT_CREATE_CONTEXT,

    SEAL_STREAM_ALREADY_OPENED,
    SEAL_STREAM_UNOPENED,
    SEAL_STREAM_INUSE,
    SEAL_MIXING_STREAM_FMT,

    SEAL_MIXING_SRC_TYPE,

    SEAL_CANNOT_OPEN_FILE,
    SEAL_BAD_AUDIO,

    SEAL_CANNOT_ALLOC_MEM,

    SEAL_BAD_WAV_SUBTYPE,
    SEAL_BAD_WAV_CHUNK,
    SEAL_BAD_WAV_CHUNK_SIZE,
    SEAL_ABSENT_WAV_CHUNK,
    SEAL_BAD_WAV_BPS,
    SEAL_BAD_WAV_NCHANNELS,
    FILE_BAD_WAV_FREQ,

    SEAL_CANNOT_OPEN_OV,
    SEAL_CANNOT_GET_OV_INFO,
    SEAL_CANNOT_READ_OV,

    SEAL_CANNOT_INIT_MPG,
    SEAL_CANNOT_GET_MPG_INFO,
    SEAL_CANNOT_READ_MPG,

    SEAL_CANNOT_OPEN_MID,
    SEAL_CANNOT_PLAY_MID
};

typedef enum seal_err_t seal_err_t;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Gets the thread-local SEAL error state and clears it to SEAL_OK. If a seal
 * function returns with an error, the later error will override the old one.
 *
 * @return  the error state
 */
seal_err_t seal_get_err(void);

/*
 * Peeks the thread-local SEAL error state without clearing it to SEAL_OK.
 *
 * @return  the error state
 */
seal_err_t seal_peek_err(void);

/*
 * Gets a SEAL error message.
 *
 * @param err   the error to get the message associated with
 * @return      the error message if there is any or otherwise 0
 */
const char* seal_get_err_msg(seal_err_t);

#ifdef __cplusplus
}
#endif

/* IMPLEMENTATION DETAILS STARTS FROM HERE. */

/*
 * Sets the thread-local SEAL error state.
 *
 * @param err the error to set
 */
void _seal_set_err(seal_err_t);

/*
 * Checks if OpenAL is in an error state and sets the corresponding SEAL error
 * if it is.
 *
 * @return  1 if OpenAL is not in an error state or otherwise 0
 */
int _seal_chk_openal_err(void);

/*
 * Some syntactic sugars for SEAL error checking. No multiple evaluation.
 * SEAL_CHK_AL_* asserts there is no OpenAL error so far.
 * SEAL_CHK_* (without the "AL") asserts an expression evaluates to true.
 * Macros with suffix `_S' also does clean-up by jumping to a local label that
 * starts the clean-up process.
 */

/* Sets an error and returns with a specified value. */
#define SEAL_ABORT(err, ret) do                                             \
{                                                                           \
    _seal_set_err(err);                                                     \
    return ret;                                                             \
} while (0)

/* Sets an error and jumps to a specified label. */
#define SEAL_ABORT_S(err, label) do                                         \
{                                                                           \
    _seal_set_err(err);                                                     \
    goto label;                                                             \
} while (0)

/* Asserts `expr' evaluates to true and returns. */
#define SEAL_CHK(expr, err, ret) if (!(expr)) SEAL_ABORT(err, ret)

/* Asserts `expr' evaluates to true and jumps. */
#define SEAL_CHK_S(expr, err, label) if (!(expr)) SEAL_ABORT_S(err, label)

#endif /* _SEAL_ERR_H_ */
