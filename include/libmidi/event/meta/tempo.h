#ifndef MIDI_TEMPO_H
#define MIDI_TEMPO_H

#include <stdint.h>

typedef struct tempo
{
    uint32_t val;
} midi_tempo_t;

#ifdef __cplusplus
extern "C"
{
#endif

int midi_tempo_unmarshal(midi_tempo_t* ctx, const uint8_t* data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_TEMPO_H
