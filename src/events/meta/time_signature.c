#include "events/meta/time_signature.h"

int midi_time_signature_unmarshal(midi_time_signature_t *ctx, uint8_t *data, uint32_t size)
{
    // FF 58 04 nn dd cc bb
    uint32_t iterator = 0;
    ctx->numerator = data[iterator];
    iterator++;
    ctx->denominator = data[iterator];
    iterator++;
    ctx->ticks_per_click = data[iterator];
    iterator++;
    ctx->thirty_second_notes_per_crotchet = data[iterator];
    iterator++;

    return iterator;
}