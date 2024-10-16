#include "events/key_pressure.h"

#include <stdio.h>

int midi_key_pressure_unmarshal(midi_key_pressure_t *ctx, midi_cmd_t cmd, uint8_t *data, uint32_t size)
{
    if (cmd.status != MIDI_STATUS_KEY_PRESSURE)
    {
        return MIDI_ERROR_STATUS_INVALID;
    }

    uint32_t iterator = 0;
    ctx->channel = cmd.channel;
    MIDI_CHECK_DATA_OR_FAIL(data[iterator], ctx->pitch);
    iterator++;
    MIDI_CHECK_DATA_OR_FAIL(data[iterator], ctx->pressure);
    iterator++;
    return iterator;
}

void midi_key_pressure_to_str(midi_key_pressure_t *ctx, char *data, uint32_t size)
{
    snprintf(data, size, "key press ch: %02d pitch: %03d pressure: %03d", ctx->channel, ctx->pitch, ctx->pressure);
}
