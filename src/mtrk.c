#include "mtrk.h"
#include "util.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

mtrk_t *mtrk_new()
{
    mtrk_t *ctx = calloc(1, sizeof(mtrk_t));
    for (uint8_t i = 0; i < MTRK_MARKER_SIZE; i++)
    {
        ctx->mtrk[i] = 0;
    }
    ctx->size = 0;
    ctx->events_count = 0;
    ctx->events = NULL;
    return ctx;
}

void mtrk_free(mtrk_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    if (ctx->events != NULL)
    {
        free(ctx->events);
    }
    free(ctx);
}

int mtrk_debug(mtrk_t *ctx, char *data, uint32_t size)
{
    const char *mtrk_debug_fmt = "mtrk: %04s\nsize: %04d\nevents: %04d\n";
    return sprintf(data, mtrk_debug_fmt, ctx->mtrk, ctx->size, ctx->events_count);
}

int mtrk_unmarshal(mtrk_t *ctx, uint8_t *data, const uint32_t size)
{
    int res = 0;
    uint32_t iterator = 0;
    (*(uint32_t *)&ctx->mtrk) = readu32(data, &iterator);
    if (memcmp(ctx->mtrk, mtrk_header_reference, MTRK_MARKER_SIZE))
    {
        return -1;
    }
    ctx->size = readu32bswap(data, &iterator);

    if (ctx->size > size)
    {
        return -1;
    }

    while (1)
    {
        if (iterator >= size)
        {
            break;
        }
        midi_event_t *event = midi_event_new();
        if (res = midi_event_unmarshal(event, data + iterator, size), res < 0)
        {
            midi_event_free(event);
            return -1;
        }
        iterator += res;

        if (ctx->events_count == 0)
        {
            ctx->events = calloc(1, sizeof(midi_event_t));
        }
        else
        {
            ctx->events = realloc(ctx->events, sizeof(midi_event_t) * ctx->events_count + 1);
        }

        ctx->events[ctx->events_count] = event;
        ctx->events_count++;
    }

    return iterator;
}
