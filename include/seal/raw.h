/*
 * Interfaces for manipulating the data structure `seal_raw_t' which contains
 * raw PCM data.
 */

#ifndef _SEAL_RAW_H_
#define _SEAL_RAW_H_

#include <stddef.h>
#include "err.h"

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
 * Allocates the memory pointed by `raw->data'.
 *
 * @param raw   the raw structure with the `data' field to allocate
 * @param size  the size of the memory to allocate
 */
seal_err_t seal_alloc_raw_data(seal_raw_t*, size_t);

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
 */
seal_err_t seal_extend_raw_data(seal_raw_t*);

/*
 * Ensures the size of the memory pointed by `raw->data' is larger than `size'
 * and extends the memory if necessary.
 *
 * @param raw   the raw structure with the `data' field to ensure
 * @param size  the size to ensure `raw->data' to have
 */
seal_err_t seal_ensure_raw_data_size(seal_raw_t*, size_t);

#ifdef __cplusplus
}
#endif

#endif /* _SEAL_RAW_H_ */
