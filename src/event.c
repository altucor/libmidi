#include "event.h"

#include <stdlib.h>

midi_event_t *midi_event_new()
{
    midi_event_t *ctx = calloc(1, sizeof(midi_event_t));
    ctx->size = 0;
    ctx->data = NULL;
    return ctx;
}

void midi_event_free(midi_event_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    if (ctx->data != NULL)
    {
        free(ctx->data);
    }
    free(ctx);
}

void midi_event_parse_note(uint8_t *data, const uint32_t size)
{
}

bool midi_event_unmarshal(midi_event_t *ctx, uint8_t *data, const uint32_t size)
{
    uint32_t iterator = 0;
    uint8_t b = data[iterator++];
    ctx->status = (b >> 4) & MIDI_STATUS_MESSAGE_CMD_MASK;
    switch (ctx->status)
    {
    case MIDI_STATUS_NOTE_OFF:
        break;
    case MIDI_STATUS_NOTE_ON:
        break;
    case MIDI_STATUS_KEY_PRESSURE:
        break;
    case MIDI_STATUS_CONTROLLER_CHANGE:
        break;
    case MIDI_STATUS_PROGRAM_CHANGE:
        break;
    case MIDI_STATUS_CHANNEL_PRESSURE:
        break;
    case MIDI_STATUS_PITCH_BEND:
        break;
    case MIDI_STATUS_SYSTEM:
        break;
    default:
        break;
    }
    return false;
}
