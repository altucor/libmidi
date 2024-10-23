#include "buffer.h"

#include <memory.h>
#include <stdlib.h>

void buffer_reset(buffer_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    ctx->expected_size = 0;
    ctx->iterator_write = 0;
}

void buffer_set_expected_size(buffer_t *ctx, const uint32_t size)
{
    if (ctx == NULL || size > BUFFER_SIZE)
    {
        return;
    }
    buffer_reset(ctx);
    ctx->expected_size = size;
    // TODO: Maybe overwrite with zero whole buffer
}

uint32_t buffer_space_left(buffer_t *ctx)
{
    if (ctx == NULL)
    {
        return 0;
    }
    return ctx->expected_size - ctx->iterator_write;
}

void buffer_append_u8(buffer_t *ctx, const uint8_t b)
{
    if (ctx == NULL || ctx->iterator_write >= ctx->expected_size)
    {
        return;
    }
    ctx->data[ctx->iterator_write++] = b;
}
