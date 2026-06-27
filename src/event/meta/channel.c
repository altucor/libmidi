#include "libmidi/event/meta/channel.h"
#include "libmidi/errors.h"

int midi_channel_unmarshal(midi_channel_t* ctx, const uint8_t* data, const uint32_t size)
{
    // FF 20 01 dd

    static const uint32_t k_expected_size = 2;

    if (!ctx || !data || size < k_expected_size)
    {
        return MIDI_ERROR_GENERAL;
    }

    // TODO: Use MIDI_DECODE_OR_FAIL

    uint32_t iterator = 0;
    ctx->val = data[iterator++] << 8;
    ctx->val |= data[iterator++];

    return iterator;
}
