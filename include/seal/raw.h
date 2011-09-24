/*
 * raw.h is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 *
 * raw.h defines the the data structure `seal_raw_t' that represents raw PCM
 * data as well as a series of operations on them.
 */

#ifndef _SEAL_RAW_H_
#define _SEAL_RAW_H_

#include <stddef.h>
#include "pstdint.h"

/* Raw PCM data. */
typedef struct seal_raw_t seal_raw_t;
/* Raw PCM attribute. */
typedef struct seal_raw_attr_t seal_raw_attr_t;

/*
 * bit_depth    bits per sample
 * nchannels    number of channels
 * freq         frequency or sample rate
 */
struct seal_raw_attr_t
{
    int    bit_depth;
    int    nchannels;
    int    freq;
};

/*
 * data     the raw PCM data
 * size     receives the size of the data
 */
struct seal_raw_t
{
    void*           data;
    size_t          size;
    seal_raw_attr_t attr;
};

#define SEAL_RAW_ATTR_INIT_LST { 0, 0, 0 }
#define SEAL_RAW_INIT_LST      { 0, 0, SEAL_RAW_ATTR_INIT_LST }

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Allocates a `seal_raw_t'.
 *
 * @return  the newly allocated `seal_raw_t' if successful or otherwise 0
 */
seal_raw_t* seal_alloc_raw(void);

/*
 * Frees a `seal_raw_t' structure allocated by `seal_alloc_raw'.
 *
 * @param raw   the raw structure to free
 */
void seal_free_raw(seal_raw_t*);

/*
 * Allocates the memory pointed by `raw->data'.
 *
 * @param raw   the raw structure with the `data' field to allocate
 * @param size  the size of the memory to allocate
 * @return      nonzero if successful or otherwise 0
 */
int seal_alloc_raw_data(seal_raw_t*, size_t);

/*
 * Deallocates the memory pointed by `raw->data'.
 *
 * @param raw   the raw structure with the `data' field to deallocate
 */
void seal_free_raw_data(seal_raw_t*);

/*
 * Extends the size of the memory pointed by `raw->data'.
 *
 * @param raw   the raw structure with the `data' field to extend
 * @return      nonzero if successful or otherwise 0
 */
int seal_extend_raw_data(seal_raw_t*);

/*
 * Ensures the size of the memory pointed by `raw->data' is larger than `size'
 * and extends the memory if necessary.
 *
 * @param raw   the raw structure with the `data' field to ensure
 * @param size  the size to ensure `raw->data' to have
 * @return      nonzero if successful or otherwise 0
 */
int seal_ensure_raw_data_size(seal_raw_t*, size_t);

#ifdef __cplusplus
}
#endif

#endif /* _SEAL_RAW_H_ */
