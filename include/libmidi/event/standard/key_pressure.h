#ifndef MIDI_EVENT_STANDARD_KEY_PRESSURE_H
#define MIDI_EVENT_STANDARD_KEY_PRESSURE_H

#include "libmidi/protocol.h"

typedef struct _key_pressure
{
    uint8_t channel;
    uint8_t pitch;
    uint8_t pressure;
} midi_key_pressure_t;

#ifdef __cplusplus
extern "C"
{
#endif

int midi_key_pressure_unmarshal(
    midi_key_pressure_t* ctx,
    const midi_cmd_t cmd,
    const uint8_t* data,
    const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_EVENT_STANDARD_KEY_PRESSURE_H
