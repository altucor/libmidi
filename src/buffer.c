#include "buffer.h"

#include <memory.h>
#include <stdlib.h>

void buffer_reset(buffer_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    ctx->size = 0;
    ctx->iterator_write = 0;
    ctx->iterator_read = 0;
    if (ctx->data != NULL)
    {
        free(ctx->data);
        ctx->data = NULL;
    }
}

buffer_t *buffer_new()
{
    buffer_t *ctx = calloc(1, sizeof(buffer_t));
    buffer_reset(ctx);
    return ctx;
}

void buffer_free(buffer_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    if (ctx->data != NULL)
    {
        free(ctx->data);
        ctx->data = NULL;
    }
    free(ctx);
}

void buffer_realloc(buffer_t *ctx, const uint32_t size)
{
    buffer_reset(ctx);
    ctx->size = size;
    ctx->data = malloc(size);
}

uint32_t buffer_space_left(buffer_t *ctx)
{
    if (ctx == NULL)
    {
        return 0;
    }
    return ctx->size - ctx->iterator_write;
}

void buffer_append_u8(buffer_t *ctx, const uint8_t b)
{
    if (ctx == NULL)
    {
        return;
    }
    if (ctx->iterator_write >= ctx->size)
    {
        return;
    }
    ctx->data[ctx->iterator_write++] = b;
}