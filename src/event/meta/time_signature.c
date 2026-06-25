#include "libmidi/event/meta/time_signature.h"
#include "libmidi/errors.h"

int midi_time_signature_unmarshal(midi_time_signature_t* ctx, const uint8_t* data, const uint32_t size)
{
    // FF 58 04 nn dd cc bb

    static const uint32_t k_expected_size = 4;

    if (!ctx || !data || size < k_expected_size)
    {
        return MIDI_ERROR_GENERAL;
    }

    uint32_t iterator = 0;
    ctx->numerator = data[iterator++];
    ctx->denominator = data[iterator++];
    ctx->ticks_per_click = data[iterator++];
    ctx->thirty_second_notes_per_crotchet = data[iterator++];

    return iterator;
}
