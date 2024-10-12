#include "events/key_pressure.h"

#include <stdio.h>

int midi_key_pressure_unmarshal(midi_key_pressure_t *ctx, midi_cmd_t cmd, uint8_t *data, uint32_t size)
{
    if (cmd.status != MIDI_STATUS_KEY_PRESSURE)
    {
        return -1;
    }

    uint32_t iterator = 0;
    ctx->channel = cmd.subCmd;
    ctx->pitch = data[iterator++];
    ctx->pressure = data[iterator++];
    return iterator;
}

void midi_key_pressure_to_str(midi_key_pressure_t *ctx, char *data, uint32_t size)
{
    snprintf(data, size, "key press ch: %02d pitch: %03d pressure: %03d", ctx->channel, ctx->pitch, ctx->pressure);
}
