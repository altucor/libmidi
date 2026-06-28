#include "libmidi/event/system/song_position.h"
#include "libmidi/errors.h"

int midi_song_position_unmarshal(midi_song_position_t* ctx, const uint8_t* data, const uint32_t size)
{
    // F2 vv vv

    static const uint32_t k_expected_size = 2;

    if (!ctx || !data || size < k_expected_size)
    {
        return MIDI_ERROR_GENERAL;
    }

    // TODO: Use MIDI_DECODE_OR_FAIL

    uint32_t iterator = 0;
    ctx->value = 0;
    ctx->value |= data[iterator++];
    ctx->value |= (data[iterator++] << 7);

    return iterator;
}
