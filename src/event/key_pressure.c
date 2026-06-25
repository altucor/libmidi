#include "libmidi/event/key_pressure.h"

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
