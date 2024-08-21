#include "mtrk.h"
#include "util.h"

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

int mtrk_unmarshal(mtrk_t *ctx, uint8_t *data, const uint32_t size)
{
    int res = 0;
    uint32_t iterator = 0;
    for (uint8_t i = 0; i < MTRK_MARKER_SIZE; i++)
    {
        ctx->mtrk[i] = data[iterator++];
    }
    if (memcmp(ctx->mtrk, mtrk_header_reference, MTRK_MARKER_SIZE))
    {
        return -1;
    }

    ctx->size = readu32bswap(data, &iterator);

    while (1)
    {
        if (iterator >= ctx->size)
        {
            break;
        }
        midi_event_t *event = midi_event_new();
        if (res = midi_event_unmarshal(event, data + iterator, size), res < 0)
        {
            return -1;
        }

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

        iterator += ctx->size;
    }

    return iterator;
}
