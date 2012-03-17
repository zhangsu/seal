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

    SEAL_STREAM_UNOPENED,
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
    SEAL_CANNOT_REWIND_WAV,

    SEAL_CANNOT_OPEN_OV,
    SEAL_CANNOT_GET_OV_INFO,
    SEAL_CANNOT_READ_OV,
    SEAL_CANNOT_REWIND_OV,
    SEAL_CANNOT_CLOSE_OV,

    SEAL_CANNOT_INIT_MPG,
    SEAL_CANNOT_GET_MPG_INFO,
    SEAL_CANNOT_READ_MPG,
    SEAL_CANNOT_REWIND_MPG,
    SEAL_CANNOT_CLOSE_MPG,
};

typedef enum seal_err_t seal_err_t;

#ifdef __cplusplus
extern "C" {
#endif

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

/*
 *****************************************************************************
 * Below are **implementation details**.
 *****************************************************************************
 */

/*
 * @return  the OpenAL error as a SEAL error
 */
seal_err_t _seal_get_openal_err(void);

#endif /* _SEAL_ERR_H_ */
