#include "libmidi/event/system/song_select.h"
#include "libmidi/errors.h"

int midi_song_select_unmarshal(midi_song_select_t* ctx, const uint8_t* data, const uint32_t size)
{
    // F3 vv

    static const uint32_t k_expected_size = 1;

    if (!ctx || !data || size < k_expected_size)
    {
        return MIDI_ERROR_GENERAL;
    }

    // TODO: Use MIDI_DECODE_OR_FAIL

    ctx->value.raw = data[0];
    if (ctx->value.____reserved)
    {
        return MIDI_ERROR_GENERAL;
    }

    return k_expected_size;
}
