#include "libmidi/event/standard/channel_pressure.h"
#include "libmidi/protocol.h"

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

int midi_channel_pressure_marshal(const midi_channel_pressure_t* ctx, uint8_t* data, const uint32_t size)
{
    static const int k_channel_pressure_size = 2;
    if (!ctx || size < k_channel_pressure_size)
    {
        return -1;
    }

    midi_cmd_t cmd = {};

    cmd.new_msg = true;
    cmd.status = MIDI_STATUS_CHANNEL_PRESSURE;
    cmd.channel = ctx->channel;

    data[0] = cmd.raw;
    data[1] = ctx->pressure;

    return k_channel_pressure_size;
}
