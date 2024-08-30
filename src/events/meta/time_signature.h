#ifndef MIDI_TIME_SIGNATURE_H
#define MIDI_TIME_SIGNATURE_H

#include <stdint.h>

typedef struct time_signature
{
    uint8_t numerator;
    uint8_t denominator;
    uint8_t ticks_per_click;
    uint8_t thirty_second_notes_per_crotchet;
} midi_time_signature_t;

#ifdef __cplusplus
extern "C" {
#endif

int midi_time_signature_unmarshal(midi_time_signature_t *ctx, uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_TIME_SIGNATURE_H
