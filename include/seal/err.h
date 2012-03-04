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

    SEAL_OPEN_DEVICE_FAILED,
    SEAL_NO_EFX,
    SEAL_NO_EXT_FUNC,
    SEAL_BAD_DEVICE,
    SEAL_CREATE_CONTEXT_FAILED,

    SEAL_ALLOC_BUF_FAILED,
    SEAL_FREE_BUF_FAILED,
    SEAL_BUF_INUSE,

    SEAL_ALLOC_STREAM_FAILED,
    SEAL_STREAM_ALREADY_OPENED,
    SEAL_STREAM_UNOPENED,
    SEAL_STREAM_INUSE,
    SEAL_MIXING_STREAM_FMT,

    SEAL_ALLOC_SRC_FAILED,
    SEAL_BAD_SRC_ATTR_VAL,
    SEAL_BAD_SRC_OP,
    SEAL_MIXING_SRC_TYPE,

    SEAL_BAD_LISTENER_ATTR_VAL,

    SEAL_ALLOC_EFEFCT_FAILED,
    SEAL_FREE_EFFECT_FAILED,
    SEAL_BAD_EFFECT,
    SEAL_BAD_EFFECT_ATTR_VAL,
    SEAL_BAD_EFFECT_OP,

    SEAL_OPEN_FILE_FAILED,
    SEAL_BAD_AUDIO,

    SEAL_MEM_ALLOC_FAILED,

    SEAL_BAD_WAV_SUBTYPE,
    SEAL_BAD_WAV_CHUNK,
    SEAL_BAD_WAV_CHUNK_SIZE,
    SEAL_ABSENT_WAV_CHUNK,
    SEAL_BAD_WAV_BPS,
    SEAL_BAD_WAV_NCHANNELS,
    FILE_BAD_WAV_FREQ,

    SEAL_OPEN_OV_FAILED,
    SEAL_GET_OV_INFO_FAILED,
    SEAL_READ_OV_FAILED,

    SEAL_INIT_MPG_FAILED,
    SEAL_GET_MPG_INFO_FAILED,
    SEAL_READ_MPG_FAILED,

    SEAL_OPEN_MID_FAILED,
    SEAL_PLAY_MID_FAILED
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
 * Retrieves the last OpenAL error.
 *
 * @return  the error
 */
int _seal_get_al_err(void);

#ifdef __cplusplus
}
#endif

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

#define SEAL_CHK_AL(alerr, err, ret)                                        \
    SEAL_CHK((_seal_get_al_err() != (alerr)), err, ret)

#define SEAL_CHK_AL_S(alerr, err, label)                                    \
    SEAL_CHK_S(_seal_get_al_err() != (alerr), err, label)

/* Checks and maps 2 OpenAL errors to 2 SEAL errors. */
#define SEAL_CHK_AL2(alerr1, err1, alerr2, err2, ret) do                    \
{                                                                           \
    switch (_seal_get_al_err()) {                                           \
    case (alerr1):                                                          \
        SEAL_ABORT(err1, ret);                                              \
    case (alerr2):                                                          \
        SEAL_ABORT(err2, ret);                                              \
    }                                                                       \
} while (0)

/* Checks and maps 2 OpenAL errors to 2 SEAL errors. */
#define SEAL_CHK_AL2_S(alerr1, err1, alerr2, err2, label) do                \
{                                                                           \
    switch (_seal_get_al_err()) {                                           \
    case (alerr1):                                                          \
        SEAL_ABORT_S(err1, label);                                          \
    case (alerr2):                                                          \
        SEAL_ABORT_S(err2, label);                                          \
    }                                                                       \
} while (0)

/* Checks and maps 3 OpenAL errors to 3 SEAL errors. */
#define SEAL_CHK_AL3(alerr1, err1, alerr2, err2, alerr3, err3, ret) do      \
{                                                                           \
    switch (_seal_get_al_err()) {                                           \
    case (alerr1):                                                          \
        SEAL_ABORT(err1, ret);                                              \
    case (alerr2):                                                          \
        SEAL_ABORT(err2, ret);                                              \
    case (alerr3):                                                          \
        SEAL_ABORT(err3, ret);                                              \
    }                                                                       \
} while (0)

/* Checks and maps 3 OpenAL errors to 3 SEAL errors. */
#define SEAL_CHK_AL3_S(alerr1, err1, alerr2, err2, alerr3, err3, label) do  \
{                                                                           \
    switch (_seal_get_al_err()) {                                           \
    case (alerr1):                                                          \
        SEAL_ABORT_S(err1, label);                                          \
    case (alerr2):                                                          \
        SEAL_ABORT_S(err2, label);                                          \
    case (alerr3):                                                          \
        SEAL_ABORT_S(err3, label);                                          \
    }                                                                       \
} while (0)

#endif /* _SEAL_ERR_H_ */
