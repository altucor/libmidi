#ifndef MIDI_CHANNEL_PRESSURE_H
#define MIDI_CHANNEL_PRESSURE_H

#include "protocol.h"

typedef struct channel_pressure
{
    uint8_t channel;
    uint8_t pressure;
} midi_channel_pressure_t;

#ifdef __cplusplus
extern "C" {
#endif

int midi_channel_pressure_unmarshal(midi_channel_pressure_t *ctx, midi_cmd_t cmd, uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_CHANNEL_PRESSURE_H
