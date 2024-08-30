#include "events/note.h"

void midi_note_reset(midi_note_t *ctx)
{
    ctx->on = false;
    ctx->channel = 0;
    ctx->pitch = 0;
    ctx->velocity = 0;
}

int midi_note_unmarshal(midi_note_t *ctx, midi_cmd_t cmd, uint8_t *data, uint32_t size)
{
    uint32_t iterator = 0;
    ctx->on = cmd.status;
    ctx->channel = cmd.subCmd;
    ctx->pitch = data[iterator++];
    ctx->velocity = data[iterator++];
    return iterator;
}