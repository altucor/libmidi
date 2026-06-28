#include "libmidi/event/system/mtc_quarter_frame.h"
#include "libmidi/errors.h"

int midi_mtc_quarter_frame_unmarshal(midi_mtc_quarter_frame_t* ctx, const uint8_t* data, const uint32_t size)
{
    // F1 dd

    static const uint32_t k_expected_size = 1;

    if (!ctx || !data || size < k_expected_size)
    {
        return MIDI_ERROR_GENERAL;
    }

    ctx->value.raw = data[0];

    return k_expected_size;
}
