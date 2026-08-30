#ifndef MIDI_SEQUENCE_NUMBER_H
#define MIDI_SEQUENCE_NUMBER_H

#include <stdint.h>

typedef struct _midi_sequence_number
{
    uint16_t val;
} midi_sequence_number_t;

#ifdef __cplusplus
extern "C"
{
#endif

int midi_sequence_number_unmarshal(midi_sequence_number_t* ctx, const uint8_t* data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_SEQUENCE_NUMBER_H
