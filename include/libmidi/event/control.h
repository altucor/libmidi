#ifndef MIDI_CONTROL_H
#define MIDI_CONTROL_H

#include "libmidi/protocol.h"

#include <stdint.h>

typedef struct _midi_control
{
    uint8_t channel;
    uint8_t control;
    uint8_t value;
} midi_control_t;

#ifdef __cplusplus
extern "C"
{
#endif

void midi_control_reset(midi_control_t* ctx);
int midi_control_unmarshal(midi_control_t* ctx, const midi_cmd_t cmd, const uint8_t* data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_CONTROL_H
