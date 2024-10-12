#include "events/note.h"
#include "util.h"

#include <stdio.h>

void midi_note_reset(midi_note_t *ctx)
{
    ctx->on = false;
    ctx->channel = 0;
    ctx->pitch = 0;
    ctx->velocity = 0;
}

int midi_note_unmarshal(midi_note_t *ctx, midi_cmd_t cmd, uint8_t *data, uint32_t size)
{
    if (cmd.status != MIDI_STATUS_NOTE_ON & cmd.status != MIDI_STATUS_NOTE_OFF)
    {
        return -1;
    }

    uint32_t iterator = 0;
    ctx->on = cmd.status;
    ctx->channel = cmd.subCmd;
    ctx->pitch = data[iterator++];
    ctx->velocity = data[iterator++];
    return iterator;
}

void midi_note_to_str(midi_note_t *ctx, char *data, uint32_t size)
{
    snprintf(data, size, "note %s ch: %02d pitch: %03d (%s %.2f) velocity: %03d", ctx->on ? "On " : "Off", ctx->channel, ctx->pitch, kNotesStr[ctx->pitch],
             kNotesFreq[ctx->pitch], ctx->channel);
}

float midi_note_freq(midi_note_t *ctx)
{
    return pitch_to_freq(ctx->pitch);
}

const char *midi_note_name(midi_note_t *ctx)
{
    return pitch_to_name(ctx->pitch);
}
