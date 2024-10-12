#ifndef MIDI_PITCH_H
#define MIDI_PITCH_H

#include "protocol.h"

#include <stdint.h>

typedef struct _midi_pitch
{
    uint8_t channel;
    uint16_t value;
} midi_pitch_t;

#ifdef __cplusplus
extern "C" {
#endif

void midi_pitch_reset(midi_pitch_t *ctx);
int midi_pitch_unmarshal(midi_pitch_t *ctx, midi_cmd_t cmd, uint8_t *data, uint32_t size);
void midi_pitch_to_str(midi_pitch_t *ctx, char *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_PITCH_H
