#ifndef MIDI_KEY_PRESSURE_H
#define MIDI_KEY_PRESSURE_H

#include "protocol.h"

typedef struct _key_pressure
{
    uint8_t channel;
    uint8_t pitch;
    uint8_t pressure;
} midi_key_pressure_t;

#ifdef __cplusplus
extern "C" {
#endif

int midi_key_pressure_unmarshal(midi_key_pressure_t *ctx, midi_cmd_t cmd, uint8_t *data, uint32_t size);
void midi_key_pressure_to_str(midi_key_pressure_t *ctx, char *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_KEY_PRESSURE_H
