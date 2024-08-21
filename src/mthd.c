#include "mthd.h"
#include "util.h"

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
    ctx->track_count = 0;
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

int mthd_unmarshal(mthd_t *ctx, uint8_t *data, const uint32_t size)
{
    uint32_t iterator = 0;
    for (uint8_t i = 0; i < MTHD_MARKER_SIZE; i++)
    {
        ctx->mthd[i] = data[iterator++];
    }
    if (memcmp(ctx->mthd, mthd_header_reference, MTHD_MARKER_SIZE))
    {
        return -1;
    }

    ctx->length = readu32bswap(data, &iterator);
    ctx->format = readu16bswap(data, &iterator);
    ctx->track_count = readu16bswap(data, &iterator);
    ctx->ppqn = readu16bswap(data, &iterator);

    return iterator;
}
