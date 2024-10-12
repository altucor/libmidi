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
    if (cmd.status != MIDI_STATUS_PITCH_BEND)
    {
        return -1;
    }

    uint32_t iterator = 0;
    ctx->channel = cmd.subCmd;

    // TODO: Here check 0x7F mask
    // Create some define script to read byte, check with mask and set to value otherwise return -1
    // also here each value should be 7 bit and corresponding bit shift should be applied for (0b0111 1111)  (0b0111 1111)
    ctx->value = data[iterator++];
    ctx->value |= (data[iterator++] << 7);
    return iterator;
}

void midi_pitch_to_str(midi_pitch_t *ctx, char *data, uint32_t size)
{
    snprintf(data, size, "pitch ch: %02d value: %05d", ctx->channel, ctx->value);
}
