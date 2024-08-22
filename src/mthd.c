#include "mthd.h"
#include "util.h"

#include <memory.h>
#include <stdio.h>
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

int mthd_debug(mthd_t *ctx, char *data, uint32_t size)
{
    const char *mthd_debug_fmt = "mthd: %04s\nlength: %04d\nformat: %04d\ntracks: %04d\nppqn: %04d\n";
    return sprintf(data, mthd_debug_fmt, ctx->mthd, ctx->length, ctx->format, ctx->track_count, ctx->ppqn);
}

int mthd_unmarshal(mthd_t *ctx, uint8_t *data, const uint32_t size)
{
    if (size < sizeof(mthd_t))
    {
        return -1;
    }

    uint32_t iterator = 0;
    (*(uint32_t *)&ctx->mthd) = readu32(data, &iterator);
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
