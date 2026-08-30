#include "libmidi/event/meta/channel_prefix.h"
#include "libmidi/errors.h"

int midi_channel_prefix_unmarshal(midi_channel_prefix_t* ctx, const uint8_t* data, const uint32_t size)
{
    // FF 20 01 dd

    static const uint32_t k_expected_size = 1;

    if (!ctx || !data || size < k_expected_size)
    {
        return MIDI_ERROR_GENERAL;
    }

    // TODO: Use MIDI_DECODE_OR_FAIL

    ctx->val = data[0];

    return k_expected_size;
}
