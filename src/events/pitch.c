#include "events/pitch.h"

#include "protocol.h"

void midi_pitch_reset(midi_pitch_t *ctx)
{
    ctx->channel = 0;
    ctx->value = 0;
}

int midi_pitch_unmarshal(midi_pitch_t *ctx, uint8_t *data, uint32_t size)
{
    uint32_t iterator = 0;
    ctx->channel = (data[iterator++] & MIDI_STATUS_MESSAGE_SUBCMD_MASK);
    ctx->value = data[iterator++];
    ctx->value |= (data[iterator++] << 8);
    return iterator;
}