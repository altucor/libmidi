#include "libmidi/mtrk.h"
#include "libmidi/util.h"

#include <memory.h>
#include <stdlib.h>

void mtrk_handle_error(mtrk_t* ctx, const midi_error_e error)
{
    if (!ctx)
    {
        return;
    }

    ctx->error = error;
}

void mtrk_handle_event(mtrk_t* ctx, const midi_event_t* event)
{
    if (!ctx)
    {
        return;
    }

    if (ctx->events_count == 0)
    {
        ctx->events = calloc(ctx->events_count + 1, sizeof(midi_event_t));
    }
    else
    {
        ctx->events = realloc(ctx->events, sizeof(midi_event_t) * (ctx->events_count + 1));
    }

    midi_event_t* event_copy = midi_event_new();
    if (!event_copy)
    {
        return;
    }

    midi_event_copy(event_copy, event);

    ctx->events[ctx->events_count] = event_copy;
    ctx->events_count++;
}

mtrk_t* mtrk_new(midi_input_device_t* device)
{
    if (!device)
    {
        return NULL;
    }

    mtrk_t* ctx = calloc(1, sizeof(mtrk_t));
    if (!ctx)
    {
        return NULL;
    }

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
    ctx->cb.event = (midi_cb_event_f*)&mtrk_handle_event;
    midi_input_device_set_listener(ctx->device, &ctx->cb);
    return ctx;
}

void mtrk_free(mtrk_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    if (ctx->events)
    {
        for (uint32_t i = 0; i < ctx->events_count; i++)
        {
            midi_event_free(ctx->events[i]);
        }

        free(ctx->events);
        ctx->events = NULL;
        ctx->events_count = 0;
    }

    free(ctx);
}

static bool is_track_end_event(const midi_event_t* event)
{
    return event->message.status == MIDI_STATUS_SYSTEM && event->message.system == MIDI_STATUS_SYSTEM_COMMON_META &&
           event->message_meta == MIDI_META_EVENT_TRACK_END;
    // return false;
}

int mtrk_unmarshal(mtrk_t* ctx, const uint8_t* data, const uint32_t size)
{
    if (size < sizeof(ctx->mtrk) + sizeof(ctx->size))
    {
        return MIDI_ERROR_NOT_ENOUGH_DATA;
    }

    int res = 0;
    uint32_t iterator = 0;

    (*(uint32_t*)&ctx->mtrk) = readu32(data, &iterator);
    if (memcmp(ctx->mtrk, k_mtrk_header_reference, MTRK_MARKER_SIZE))
    {
        return MIDI_ERROR_MTRK_INVALID_MARKER;
    }

    ctx->size = readu32bswap(data, &iterator);

    if (ctx->size > size - iterator)
    {
        return MIDI_ERROR_NOT_ENOUGH_DATA;
    }

    while (1)
    {
        if (iterator >= size)
        {
            break;
        }

        if (ctx->events_count > 0 && is_track_end_event(ctx->events[ctx->events_count - 1]))
        {
            break;
        }

        midi_input_device_feed(ctx->device, data[iterator]);
        iterator++;
    }

    return iterator;
}

uint32_t mtrk_get_events_count(mtrk_t* ctx)
{
    if (!ctx)
    {
        return 0;
    }

    return ctx->events_count;
}

midi_event_t* mtrk_get_event(mtrk_t* ctx, const uint32_t index)
{
    if (!ctx || index >= ctx->events_count)
    {
        return NULL;
    }

    return ctx->events[index];
}

int32_t mtrk_find_event_index(mtrk_t* ctx, const uint32_t start_index, const midi_cmd_t cmd, const uint8_t message_meta)
{
    if (!ctx || start_index > ctx->events_count)
    {
        return -1;
    }
    for (uint32_t i = start_index; i < ctx->events_count; i++)
    {
        midi_event_t* event = ctx->events[i];
        if (event->message.status != cmd.status || event->message.system != cmd.system ||
            event->message_meta != message_meta)
        {
            continue;
        }
        return i;
    }

    return -1;
}

uint64_t mtrk_get_duration(mtrk_t* ctx)
{
    if (!ctx)
    {
        return 0;
    }

    uint64_t total = 0;

    for (uint32_t i = 0; i < ctx->events_count; i++)
    {
        total += ctx->events[i]->predelay;
    }

    return total;
}
