#include "libmidi/event/standard/pitch.h"

#include "libmidi/protocol.h"

void midi_pitch_reset(midi_pitch_t* ctx)
{
    ctx->channel = 0;
    ctx->value = 0;
}

int midi_pitch_unmarshal(midi_pitch_t* ctx, const midi_cmd_t cmd, const uint8_t* data, const uint32_t size)
{
    if (cmd.status != MIDI_STATUS_PITCH_BEND)
    {
        return MIDI_ERROR_STATUS_INVALID;
    }

    uint32_t iterator = 0;
    ctx->channel = cmd.channel;

    uint8_t t1, t2 = 0;
    MIDI_DECODE_OR_FAIL(t1, data, iterator, size)
    MIDI_DECODE_OR_FAIL(t2, data, iterator, size)

    ctx->value = t1 | (t2 << 7);

    return iterator;
}

int midi_pitch_marshal(const midi_pitch_t* ctx, uint8_t* data, const uint32_t size)
{
    static const int k_pitch_size = 3;
    if (!ctx || size < k_pitch_size)
    {
        return -1;
    }

    midi_cmd_t cmd = {};

    cmd.new_msg = true;
    cmd.status = MIDI_STATUS_PITCH_BEND;
    cmd.channel = ctx->channel;

    data[0] = cmd.raw;
    data[1] = (ctx->value & 0x7F);
    data[2] = ((ctx->value >> 7) & 0x7F);

    return k_pitch_size;
}
