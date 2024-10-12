#ifndef MIDI_PROGRAM_CHANGE_H
#define MIDI_PROGRAM_CHANGE_H

#include "protocol.h"

typedef struct _program_change
{
    uint8_t channel;
    uint8_t program;
} midi_program_change_t;

#ifdef __cplusplus
extern "C" {
#endif

int midi_program_change_unmarshal(midi_program_change_t *ctx, midi_cmd_t cmd, uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_PROGRAM_CHANGE_H
