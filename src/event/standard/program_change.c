#include "libmidi/event/standard/program_change.h"
#include "libmidi/protocol.h"

int midi_program_change_unmarshal(
    midi_program_change_t* ctx,
    const midi_cmd_t cmd,
    const uint8_t* data,
    const uint32_t size)
{
    if (cmd.status != MIDI_STATUS_PROGRAM_CHANGE)
    {
        return MIDI_ERROR_STATUS_INVALID;
    }

    uint32_t iterator = 0;
    ctx->channel = cmd.channel;
    MIDI_DECODE_OR_FAIL(ctx->program, data, iterator, size)

    return iterator;
}

int midi_program_change_marshal(const midi_program_change_t* ctx, uint8_t* data, const uint32_t size)
{
    static const int k_program_change_size = 2;
    if (!ctx || size < k_program_change_size)
    {
        return -1;
    }

    midi_cmd_t cmd = {};

    cmd.new_msg = true;
    cmd.status = MIDI_STATUS_PROGRAM_CHANGE;
    cmd.channel = ctx->channel;

    data[0] = cmd.raw;
    data[1] = ctx->program;

    return k_program_change_size;
}
