#include "libmidi/event/standard/control.h"

#include "libmidi/protocol.h"

void midi_control_reset(midi_control_t* ctx)
{
    ctx->channel = 0;
    ctx->control = 0;
    ctx->value = 0;
}

int midi_control_unmarshal(midi_control_t* ctx, const midi_cmd_t cmd, const uint8_t* data, const uint32_t size)
{
    if (cmd.status != MIDI_STATUS_CONTROLLER_CHANGE)
    {
        return MIDI_ERROR_STATUS_INVALID;
    }

    uint32_t iterator = 0;
    ctx->channel = cmd.channel;

    MIDI_DECODE_OR_FAIL(ctx->control, data, iterator, size)
    MIDI_DECODE_OR_FAIL(ctx->value, data, iterator, size)

    return iterator;
}
