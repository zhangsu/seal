/*
 * raw.c is part of the Scorched End Audio Library (SEAL) and is licensed
 * under the terms of the GNU Lesser General Public License. See COPYING
 * attached with the library.
 */

#include <stddef.h>
#include <seal/raw.h>
#include <seal/core.h>
#include <seal/err.h>
#include <assert.h>

/* Reallocates `raw->data' to size `size'. */
static int realloc_raw_data(seal_raw_t*, size_t /*size*/);

seal_raw_t*
seal_alloc_raw(void)
{
    seal_raw_t* raw;

    raw = _seal_calloc(1, sizeof (seal_raw_t));

    return raw;
}

void
seal_free_raw(seal_raw_t* raw)
{
    if (raw->data != 0)
        _seal_free(raw->data);
    _seal_free(raw);
}

int
seal_alloc_raw_data(seal_raw_t* raw, size_t size)
{
    assert(size > 0);

    raw->size = size;
    raw->data = _seal_malloc(size);
    if (raw->data == 0)
        return 0;

    return 1;
}

void
seal_free_raw_data(seal_raw_t* raw)
{
    _seal_free(raw->data);
}

int
seal_extend_raw_data(seal_raw_t* raw)
{
    return realloc_raw_data(raw, raw->size * 2);
}

int
seal_ensure_raw_data_size(seal_raw_t* raw, size_t size)
{
    /* If the buffer is not large enough... */
    if (size >= raw->size)
        return seal_extend_raw_data(raw);

    return 1;
}

int
realloc_raw_data(seal_raw_t* raw, size_t size)
{
    void* buf;

    buf = _seal_realloc(raw->data, size);
    SEAL_CHK(buf != 0, SEAL_MEM_ALLOC_FAILED, 0);
    raw->data = buf;
    raw->size = size;

    return 1;
}
