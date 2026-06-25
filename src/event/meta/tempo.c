#include "libmidi/event/meta/tempo.h"
#include "libmidi/errors.h"

int midi_tempo_unmarshal(midi_tempo_t* ctx, const uint8_t* data, const uint32_t size)
{
    // FF 51 03 tt tt tt

    static const uint32_t k_expected_size = 3;

    if (!ctx || !data || size < k_expected_size)
    {
        return MIDI_ERROR_GENERAL;
    }

    // TODO: Use MIDI_DECODE_OR_FAIL

    uint32_t iterator = 0;
    ctx->val = data[iterator++] << 16;
    ctx->val |= data[iterator++] << 8;
    ctx->val |= data[iterator++];
    ctx->val = 60000000 / ctx->val;

    return iterator;
}
