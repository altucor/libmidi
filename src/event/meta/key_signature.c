#include "libmidi/event/meta/key_signature.h"
#include "libmidi/errors.h"

int midi_key_signature_unmarshal(midi_key_signature_t* ctx, const uint8_t* data, const uint32_t size)
{
    // FF 59 02 sf mi

    static const uint32_t k_expected_size = 2;

    if (!ctx || !data || size < k_expected_size)
    {
        return MIDI_ERROR_GENERAL;
    }

    // TODO: Use MIDI_DECODE_OR_FAIL

    uint32_t iterator = 0;
    ctx->sharps_flats = *(int8_t*)&data[iterator++];
    ctx->scale = data[iterator++];

    return iterator;
}
