#include "meta.h"

#include "util.h"

#include "memory.h"
#include "stdlib.h"

void midi_event_meta_reset(midi_event_meta_t *ctx)
{
    ctx->type = 0;
    vlv_reset(&ctx->length);
    ctx->data = NULL;
}

midi_event_meta_t *midi_event_meta_new()
{
    midi_event_meta_t *ctx = calloc(1, sizeof(midi_event_meta_t));
    ctx->type = 0;
    vlv_reset(&ctx->length);
    ctx->data = NULL;
    return ctx;
}

void midi_event_meta_free(midi_event_meta_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    if (ctx->data)
    {
        free(ctx->data);
        ctx->data = NULL;
    }
    free(ctx);
}

int midi_event_meta_unmarshal(midi_event_meta_t *ctx, uint8_t *data, uint32_t size)
{
    int res = 0;
    uint32_t iterator = 0;
    (*(uint8_t *)&ctx->type) = data[iterator++];
    if (res = vlv_unmarshal(&ctx->length, data + iterator, size), res < 0)
    {
        return -1;
    }
    iterator += res;
    size -= res;

    if (ctx->length.val > 0)
    {
        // ctx->data = calloc(ctx->length.val, sizeof(uint8_t));
        // memcpy(ctx->data, data + iterator, ctx->length.val);
        // iterator += ctx->length.val;
        alloc_and_copy_from_stream(ctx->data, data, ctx->length.val, &iterator);
        size -= ctx->length.val;
    }

    switch (ctx->type)
    {
    case MIDI_META_EVENT_SEQUENCE_NUMBER:
        break;
    case MIDI_META_EVENT_TEXT:
        break;
    case MIDI_META_EVENT_COPYRIGHT:
        break;
    case MIDI_META_EVENT_TRACK_NAME:
        break;
    case MIDI_META_EVENT_INSTRUMENT_NAME:
        break;
    case MIDI_META_EVENT_LYRIC_TEXT:
        break;
    case MIDI_META_EVENT_TEXT_MARKER:
        break;
    case MIDI_META_EVENT_CUE_POINT:
        break;
    case MIDI_META_EVENT_PROGRAM_PATCH_NAME:
        break;
    case MIDI_META_EVENT_DEVICE_PORT_NAME:
        break;
    case MIDI_META_EVENT_MIDI_CHANNEL:
        break;
    case MIDI_META_EVENT_MIDI_PORT:
        break;
    case MIDI_META_EVENT_TRACK_END:
        break;
    case MIDI_META_EVENT_TEMPO:
        break;
    case MIDI_META_EVENT_SMPTE_OFFSET:
        break;
    case MIDI_META_EVENT_TIME_SIGNATURE:
        break;
    case MIDI_META_EVENT_KEY_SIGNATURE:
        break;
    case MIDI_META_EVENT_PROPRIETARY_EVENT:
        break;
    default:
        break;
    }
    return iterator;
}

bool midi_event_meta_is_track_end(midi_event_meta_t *ctx)
{
    return ctx->type == MIDI_META_EVENT_TRACK_END;
}
