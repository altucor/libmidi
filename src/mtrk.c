#include "mtrk.h"
#include "util.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

void mtrk_handle_event(mtrk_t *ctx, midi_cmd_t msg, uint8_t message_meta, midi_event_t event)
{
    if (ctx->events_count == 0)
    {
        ctx->events = calloc(ctx->events_count + 1, sizeof(midi_event_smf_t));
    }
    else
    {
        ctx->events = realloc(ctx->events, sizeof(midi_event_smf_t) * (ctx->events_count + 1));
    }

    midi_event_smf_t *smf_event = midi_event_smf_new();

    smf_event->predelay.val = midi_input_device_get_predelay(ctx->device);
    smf_event->message = msg;
    smf_event->message_meta = message_meta;
    smf_event->event = event;
    ctx->events[ctx->events_count] = smf_event;
    ctx->events_count++;
}

mtrk_t *mtrk_new(midi_input_device_t *device)
{
    mtrk_t *ctx = calloc(1, sizeof(mtrk_t));
    ctx->device = device;
    midi_input_device_reset(ctx->device);
    for (uint8_t i = 0; i < MTRK_MARKER_SIZE; i++)
    {
        ctx->mtrk[i] = 0;
    }
    ctx->size = 0;
    ctx->events_count = 0;
    ctx->events = NULL;

    ctx->cb.handle = ctx;
    ctx->cb.event = (midi_cb_event_f *)&mtrk_handle_event;
    midi_input_device_set_listener(ctx->device, &ctx->cb);
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
            midi_event_smf_free(ctx->events[i]);
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
        if (ctx->events_count > 0 && ctx->events[ctx->events_count - 1]->message.status == MIDI_STATUS_SYSTEM &&
            ctx->events[ctx->events_count - 1]->message.subCmd == MIDI_STATUS_SYSTEM_RESET_OR_META &&
            ctx->events[ctx->events_count - 1]->message_meta == MIDI_META_EVENT_TRACK_END)
        {
            break;
        }
        if (iterator >= size)
        {
            break;
        }
        if (iterator == 47)
        {
            int a = 1;
            a = a;
        }
        midi_input_device_feed(ctx->device, data[iterator]);
        iterator++;
    }

    return iterator;
}
