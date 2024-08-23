#include "buffer.h"

#include <memory.h>

void buffer_reset(buffer_t *ctx)
{
    ctx->size = BUFFER_SIZE;
    ctx->iterator_write = 0;
    ctx->iterator_read = 0;
    memset(&ctx->data, 0x00, BUFFER_SIZE);
}

void buffer_set_size(buffer_t *ctx, const uint32_t size)
{
    ctx->size = size;
}

uint32_t buffer_space_left(buffer_t *ctx)
{
    return ctx->size - ctx->iterator_write;
}

void buffer_append_u8(buffer_t *ctx, const uint8_t b)
{
    if (ctx->iterator_write >= ctx->size)
    {
        return;
    }
    ctx->data[ctx->iterator_write++] = b;
}