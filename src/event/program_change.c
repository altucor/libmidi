#include "libmidi/event/program_change.h"

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
