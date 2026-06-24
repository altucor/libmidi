#include "libmidi/events/key_pressure.h"

int midi_key_pressure_unmarshal(midi_key_pressure_t* ctx, midi_cmd_t cmd, uint8_t* data, uint32_t size)
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
