#include "libmidi/events/control.h"

#include "libmidi/protocol.h"

void midi_control_reset(midi_control_t* ctx)
{
    ctx->channel = 0;
    ctx->control = 0;
    ctx->value = 0;
}

int midi_control_unmarshal(midi_control_t* ctx, midi_cmd_t cmd, uint8_t* data, uint32_t size)
{
    if (cmd.status != MIDI_STATUS_CONTROLLER_CHANGE)
    {
        return MIDI_ERROR_STATUS_INVALID;
    }

    uint32_t iterator = 0;
    ctx->channel = cmd.channel;
    MIDI_CHECK_DATA_OR_FAIL(data[iterator], ctx->control);
    iterator++;
    MIDI_CHECK_DATA_OR_FAIL(data[iterator], ctx->value);
    iterator++;
    return iterator;
}
