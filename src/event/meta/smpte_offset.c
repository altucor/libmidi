#include "libmidi/event/meta/smpte_offset.h"
#include "libmidi/errors.h"

int midi_smpte_offset_unmarshal(midi_smpte_offset_t* ctx, const uint8_t* data, const uint32_t size)
{
    // 0xFF 0x54 0x05 hh mm ss ff aa

    static const uint32_t k_expected_size = 5;

    if (!ctx || !data || size < k_expected_size)
    {
        return MIDI_ERROR_GENERAL;
    }

    // TODO: Use MIDI_DECODE_OR_FAIL

    uint32_t iterator = 0;
    ctx->hours.raw = data[iterator++];
    ctx->minutes = data[iterator++];
    ctx->seconds = data[iterator++];
    ctx->frames = data[iterator++];
    ctx->subframes = data[iterator++];

    return iterator;
}
