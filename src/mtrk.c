#include "mtrk.h"

#include <memory.h>
#include <stdlib.h>

mtrk_t *mtrk_new()
{
    mtrk_t *ctx = calloc(1, sizeof(mtrk_t));
    for (uint8_t i = 0; i < MTRK_MARKER_SIZE; i++)
    {
        ctx->mtrk[i] = 0;
    }
    ctx->size = 0;
    return ctx;
}

void mtrk_free(mtrk_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    if (ctx->data != NULL)
    {
        free(ctx->data);
    }
    free(ctx);
}

bool mtrk_unmarshal(mtrk_t *ctx, uint8_t *data, const uint32_t size)
{
    uint32_t iterator = 0;
    for (uint8_t i = 0; i < MTRK_MARKER_SIZE; i++)
    {
        ctx->mtrk[i] = data[iterator++];
    }
    if (memcmp(ctx->mtrk, mtrk_header_reference, MTRK_MARKER_SIZE))
    {
        return false;
    }
    for (uint8_t i = 0; i < sizeof(ctx->size); i++)
    {
        ((uint8_t *)&ctx->size)[i] = data[iterator++];
    }
    ctx->data = calloc(ctx->size, sizeof(uint8_t));
    memcpy(ctx->data, data + iterator, ctx->size);

    return true;
}
