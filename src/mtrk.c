#include "mtrk.h"
#include "util.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

mtrk_t *mtrk_new(input_state_machine_t *state_machine)
{
    mtrk_t *ctx = calloc(1, sizeof(mtrk_t));
    ctx->state_machine = state_machine;
    input_state_machine_reset(ctx->state_machine);
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
        for (uint32_t i = 0; i < ctx->events_count; i++)
        {
            // midi_event_free(ctx->events[i]);
        }
        free(ctx->events);
        ctx->events = NULL;
        ctx->events_count = 0;
    }
    free(ctx);
}

int mtrk_debug(mtrk_t *ctx, char *data, uint32_t size)
{
    const char *mtrk_debug_fmt = "mtrk: %04s\nsize: %04d\nevents: %04d\n";
    return sprintf(data, mtrk_debug_fmt, ctx->mtrk, ctx->size, ctx->events_count);
}

int mtrk_unmarshal(mtrk_t *ctx, uint8_t *data, uint32_t size)
{
    if (size < sizeof(ctx->mtrk) + sizeof(ctx->size))
    {
        return -1;
    }
    int res = 0;
    uint32_t iterator = 0;
    (*(uint32_t *)&ctx->mtrk) = readu32(data, &iterator);
    size -= sizeof(uint32_t);
    if (memcmp(ctx->mtrk, mtrk_header_reference, MTRK_MARKER_SIZE))
    {
        return -1;
    }
    ctx->size = readu32bswap(data, &iterator);
    size -= sizeof(uint32_t);

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
        input_state_machine_feed(ctx->state_machine, data[iterator]);
        iterator++;
        size--;
        // midi_event_t *event = midi_event_new();
        // if (res = midi_event_unmarshal(event, data + iterator, size), res < 0)
        // {
        //     midi_event_free(event);
        //     return -1;
        // }
        // iterator += res;
        // size -= res;

        // if (ctx->events_count == 0)
        // {
        //     ctx->events = calloc(1, sizeof(midi_event_t));
        // }
        // else
        // {
        //     ctx->events = realloc(ctx->events, sizeof(midi_event_t) * ctx->events_count + 1);
        // }

        // ctx->events[ctx->events_count] = event;
        // ctx->events_count++;

        // if (midi_event_is_system(event) && midi_event_meta_is_track_end(&event->meta))
        // {
        //     break;
        // }
    }

    return iterator;
}
