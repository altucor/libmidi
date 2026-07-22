#include "libmidi/event/standard/key_pressure.h"
#include "libmidi/protocol.h"

int midi_key_pressure_unmarshal(
    midi_key_pressure_t* ctx,
    const midi_cmd_t cmd,
    const uint8_t* data,
    const uint32_t size)
{
    if (cmd.status != MIDI_STATUS_KEY_PRESSURE)
    {
        return MIDI_ERROR_STATUS_INVALID;
    }

    uint32_t iterator = 0;
    ctx->channel = cmd.channel;

    MIDI_DECODE_OR_FAIL(ctx->pitch, data, iterator, size)
    MIDI_DECODE_OR_FAIL(ctx->pressure, data, iterator, size)

    return iterator;
}

int midi_key_pressure_marshal(const midi_key_pressure_t* ctx, uint8_t* data, const uint32_t size)
{
    static const int k_pressure_size = 3;

    if (!ctx || size < k_pressure_size)
    {
        return -1;
    }

    midi_cmd_t cmd = {};

    cmd.new_msg = true;
    cmd.status = MIDI_STATUS_KEY_PRESSURE;
    cmd.channel = ctx->channel;

    data[0] = cmd.raw;
    data[1] = ctx->pitch;
    data[2] = ctx->pressure;

    return k_pressure_size;
}
