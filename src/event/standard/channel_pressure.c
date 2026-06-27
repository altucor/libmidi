#include "libmidi/event/standard/channel_pressure.h"

int midi_channel_pressure_unmarshal(
    midi_channel_pressure_t* ctx,
    const midi_cmd_t cmd,
    const uint8_t* data,
    const uint32_t size)
{
    if (cmd.status != MIDI_STATUS_CHANNEL_PRESSURE)
    {
        return MIDI_ERROR_STATUS_INVALID;
    }

    uint32_t iterator = 0;
    ctx->channel = cmd.channel;
    MIDI_DECODE_OR_FAIL(ctx->pressure, data, iterator, size)

    return iterator;
}
