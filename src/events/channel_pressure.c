#include "events/channel_pressure.h"

int midi_channel_pressure_unmarshal(midi_channel_pressure_t *ctx, midi_cmd_t cmd, uint8_t *data, uint32_t size)
{
    if (cmd.status != MIDI_STATUS_CHANNEL_PRESSURE)
    {
        return MIDI_ERROR_STATUS_INVALID;
    }

    uint32_t iterator = 0;
    ctx->channel = cmd.subCmd;
    MIDI_CHECK_DATA_OR_FAIL(data[iterator], ctx->pressure);
    iterator++;
    return iterator;
}
