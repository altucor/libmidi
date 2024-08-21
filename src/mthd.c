#include "mthd.h"

#include <memory.h>
#include <stdlib.h>

mthd_t *mthd_new()
{
    mthd_t *ctx = calloc(1, sizeof(mthd_t));
    for (uint8_t i = 0; i < MTHD_MARKER_SIZE; i++)
    {
        ctx->mthd[i] = 0;
    }
    ctx->length = 0;
    ctx->format = MTHD_SINGLE_TRACK;
    ctx->ppqn = 0;
    return ctx;
}

void mthd_free(mthd_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    free(ctx);
}

bool mthd_unmarshal(mthd_t *ctx, uint8_t *data, const uint32_t size)
{
    uint32_t iterator = 0;
    for (uint8_t i = 0; i < MTHD_MARKER_SIZE; i++)
    {
        ctx->mthd[i] = data[iterator++];
    }
    if (memcmp(ctx->mthd, mthd_header_reference, MTHD_MARKER_SIZE))
    {
        return false;
    }

    return true;
}