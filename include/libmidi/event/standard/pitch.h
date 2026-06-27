#ifndef MIDI_EVENT_STANDARD_PITCH_H
#define MIDI_EVENT_STANDARD_PITCH_H

#include "libmidi/protocol.h"

#include <stdint.h>

typedef struct _midi_pitch
{
    uint8_t channel;
    uint16_t value;
} midi_pitch_t;

#ifdef __cplusplus
extern "C"
{
#endif

void midi_pitch_reset(midi_pitch_t* ctx);
int midi_pitch_unmarshal(midi_pitch_t* ctx, const midi_cmd_t cmd, const uint8_t* data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_EVENT_STANDARD_PITCH_H
