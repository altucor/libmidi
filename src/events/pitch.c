#include "events/pitch.h"

#include "protocol.h"

#include <stdio.h>

void midi_pitch_reset(midi_pitch_t *ctx)
{
    ctx->channel = 0;
    ctx->value = 0;
}

int midi_pitch_unmarshal(midi_pitch_t *ctx, midi_cmd_t cmd, uint8_t *data, uint32_t size)
{
    uint32_t iterator = 0;
    ctx->channel = cmd.subCmd;
    ctx->value = data[iterator++];
    ctx->value |= (data[iterator++] << 8);
    return iterator;
}

void midi_pitch_to_str(midi_pitch_t *ctx, char *data, uint32_t size)
{
    snprintf(data, size, "pitch ch: %02d value: %05d", ctx->channel, ctx->value);
}
