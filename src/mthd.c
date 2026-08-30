#include "libmidi/mthd.h"
#include "libmidi/util.h"

#include <memory.h>
#include <stdlib.h>

mthd_t* mthd_new()
{
    mthd_t* ctx = calloc(1, sizeof(mthd_t));
    if (!ctx)
    {
        return NULL;
    }

    for (uint8_t i = 0; i < MTHD_MARKER_SIZE; i++)
    {
        ctx->mthd[i] = 0;
    }

    ctx->length = 0;
    ctx->format = MTHD_FORMAT_SINGLE_TRACK;
    ctx->track_count = 0;
    ctx->ppqn = 0;

    return ctx;
}

void mthd_free(mthd_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    free(ctx);
}

int mthd_unmarshal(mthd_t* ctx, const uint8_t* data, const uint32_t size)
{
    if (!ctx || !data)
    {
        return MIDI_ERROR_INVALID_POINTER;
    }

    if (size < sizeof(mthd_t))
    {
        return MIDI_ERROR_NOT_ENOUGH_DATA;
    }

    uint32_t iterator = 0;
    (*(uint32_t*)&ctx->mthd) = readu32(data, &iterator);
    if (memcmp(ctx->mthd, k_mthd_header_reference, MTHD_MARKER_SIZE))
    {
        return MIDI_ERROR_MTHD_INVALID_MARKER;
    }

    ctx->length = readu32bswap(data, &iterator);
    ctx->format = readu16bswap(data, &iterator);

    if (ctx->format >= MTHD_FORMAT_COUNT)
    {
        return MIDI_ERROR_MTHD_INVALID_FORMAT;
    }

    ctx->track_count = readu16bswap(data, &iterator);
    ctx->ppqn = readu16bswap(data, &iterator);

    return iterator;
}
