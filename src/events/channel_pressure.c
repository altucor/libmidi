#include "events/channel_pressure.h"

int midi_chennal_pressure_unmarshal(midi_channel_pressure_t *ctx, midi_cmd_t cmd, uint8_t *data, uint32_t size)
{
    uint32_t iterator = 0;
    ctx->channel = cmd.subCmd;
    ctx->pressure = data[iterator++];
    return iterator;
}
