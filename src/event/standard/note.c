#include "libmidi/event/standard/note.h"
#include "libmidi/util.h"

void midi_note_reset(midi_note_t* ctx)
{
    ctx->on = false;
    ctx->channel = 0;
    ctx->pitch = 0;
    ctx->velocity = 0;
}

int midi_note_unmarshal(midi_note_t* ctx, const midi_cmd_t cmd, const uint8_t* data, const uint32_t size)
{
    if (cmd.status != MIDI_STATUS_NOTE_ON & cmd.status != MIDI_STATUS_NOTE_OFF)
    {
        return MIDI_ERROR_STATUS_INVALID;
    }

    uint32_t iterator = 0;
    ctx->on = cmd.status;
    ctx->channel = cmd.channel;

    MIDI_DECODE_OR_FAIL(ctx->pitch, data, iterator, size)
    MIDI_DECODE_OR_FAIL(ctx->velocity, data, iterator, size)

    return iterator;
}

float midi_note_freq(midi_note_t* ctx)
{
    return pitch_to_freq(ctx->pitch);
}

const char* midi_note_name(midi_note_t* ctx)
{
    return pitch_to_name(ctx->pitch);
}
