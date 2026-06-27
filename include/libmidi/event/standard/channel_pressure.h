#ifndef MIDI_EVENT_STANDARD_CHANNEL_PRESSURE_H
#define MIDI_EVENT_STANDARD_CHANNEL_PRESSURE_H

#include "libmidi/protocol.h"

typedef struct _channel_pressure
{
    uint8_t channel;
    uint8_t pressure;
} midi_channel_pressure_t;

#ifdef __cplusplus
extern "C"
{
#endif

int midi_channel_pressure_unmarshal(
    midi_channel_pressure_t* ctx,
    const midi_cmd_t cmd,
    const uint8_t* data,
    const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_EVENT_STANDARD_CHANNEL_PRESSURE_H
