#include "libmidi/event.h"

#include "libmidi/util.h"

#include <memory.h>
#include <stdlib.h>

void midi_event_reset(midi_event_t* ctx)
{
    memset(ctx, 0x00, sizeof(midi_event_t));
}

midi_event_t* midi_event_new()
{
    midi_event_t* ctx = calloc(1, sizeof(midi_event_t));
    if (!ctx)
    {
        return NULL;
    }

    ctx->predelay = 0;
    return ctx;
}

void midi_event_copy(midi_event_t* dst, const midi_event_t* src)
{
    if (!dst || !src)
    {
        return;
    }

    memcpy(dst, src, sizeof(midi_event_t));

    if (src->message.status != MIDI_STATUS_SYSTEM)
    {
        return;
    }

    switch (src->message_meta)
    {
        case MIDI_META_EVENT_SEQUENCE_NUMBER:
        {
            break;
        }
        case MIDI_META_EVENT_TEXT:
        case MIDI_META_EVENT_COPYRIGHT:
        case MIDI_META_EVENT_TRACK_NAME:
        case MIDI_META_EVENT_INSTRUMENT_NAME:
        case MIDI_META_EVENT_LYRIC_TEXT:
        case MIDI_META_EVENT_TEXT_MARKER:
        case MIDI_META_EVENT_CUE_POINT:
        case MIDI_META_EVENT_PROGRAM_PATCH_NAME:
        case MIDI_META_EVENT_DEVICE_PORT_NAME:
        {
            dst->meta.text = midi_text_event_new_from(&src->meta.text);
            break;
        }
        case MIDI_META_EVENT_MIDI_CHANNEL_PREFIX:
        {
            break;
        }
        case MIDI_META_EVENT_MIDI_PORT:
        {
            break;
        }
        case MIDI_META_EVENT_TRACK_END:
        {
            break;
        }
        case MIDI_META_EVENT_M_LIVE_TAG:
        {
            break;
        }
        case MIDI_META_EVENT_TEMPO:
        {
            // no need - no dynamic data
            break;
        }
        case MIDI_META_EVENT_SMPTE_OFFSET:
        {
            break;
        }
        case MIDI_META_EVENT_TIME_SIGNATURE:
        {
            // no need - no dynamic data
            break;
        }
        case MIDI_META_EVENT_KEY_SIGNATURE:
        {
            break;
        }
        case MIDI_META_EVENT_PROPRIETARY_EVENT:
        {
            break;
        }

        default: break;
    }
}

void midi_event_cleanup(midi_event_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    if (ctx->message.status != MIDI_STATUS_SYSTEM)
    {
        return;
    }

    switch (ctx->message_meta)
    {
        case MIDI_META_EVENT_SEQUENCE_NUMBER:
        {
            break;
        }
        case MIDI_META_EVENT_TEXT:
        case MIDI_META_EVENT_COPYRIGHT:
        case MIDI_META_EVENT_TRACK_NAME:
        case MIDI_META_EVENT_INSTRUMENT_NAME:
        case MIDI_META_EVENT_LYRIC_TEXT:
        case MIDI_META_EVENT_TEXT_MARKER:
        case MIDI_META_EVENT_CUE_POINT:
        case MIDI_META_EVENT_PROGRAM_PATCH_NAME:
        case MIDI_META_EVENT_DEVICE_PORT_NAME:
        {
            midi_text_event_cleanup(&ctx->meta.text);
            break;
        }
        case MIDI_META_EVENT_MIDI_CHANNEL_PREFIX:
        {
            break;
        }
        case MIDI_META_EVENT_MIDI_PORT:
        {
            break;
        }
        case MIDI_META_EVENT_TRACK_END:
        {
            break;
        }
        case MIDI_META_EVENT_M_LIVE_TAG:
        {
            break;
        }
        case MIDI_META_EVENT_TEMPO:
        {
            // no need - no dynamic data
            break;
        }
        case MIDI_META_EVENT_SMPTE_OFFSET:
        {
            break;
        }
        case MIDI_META_EVENT_TIME_SIGNATURE:
        {
            // no need - no dynamic data
            break;
        }
        case MIDI_META_EVENT_KEY_SIGNATURE:
        {
            break;
        }
        case MIDI_META_EVENT_PROPRIETARY_EVENT:
        {
            break;
        }
        default:
        {
            break;
        }
    }

    // 1. determine type
    // 2. cast or access correct field of union
    // 3. cleanup directly field internals

    // free only internal stuff without "free(ctx);"
}

void midi_event_free(midi_event_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    midi_event_cleanup(ctx);

    free(ctx);
}
