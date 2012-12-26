#include <stdlib.h>
#include <stddef.h>
#include <seal/raw.h>
#include <seal/err.h>

/* Reallocates `raw->data' to size `size'. */
static
seal_err_t
realloc_raw_data(seal_raw_t* raw, size_t size)
{
    void* buf;

    buf = realloc(raw->data, size);
    if (buf == 0)
        return SEAL_CANNOT_ALLOC_MEM;
    raw->data = buf;
    raw->size = size;

    return SEAL_OK;
}

seal_err_t
seal_alloc_raw_data(seal_raw_t* raw, size_t size)
{
    raw->size = size;
    raw->data = malloc(size);
    if (raw->data == 0)
        return SEAL_CANNOT_ALLOC_MEM;

    return SEAL_OK;
}

void
seal_free_raw_data(seal_raw_t* raw)
{
    free(raw->data);
}

seal_err_t
seal_extend_raw_data(seal_raw_t* raw)
{
    return realloc_raw_data(raw, raw->size * 2);
}

seal_err_t
seal_ensure_raw_data_size(seal_raw_t* raw, size_t size)
{
    /* If the buffer is not large enough... */
    if (size >= raw->size)
        return seal_extend_raw_data(raw);

    return SEAL_OK;
}
