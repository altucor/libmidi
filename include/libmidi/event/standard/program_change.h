#ifndef MIDI_EVENT_STANDARD_PROGRAM_CHANGE_H
#define MIDI_EVENT_STANDARD_PROGRAM_CHANGE_H

#include "libmidi/protocol.h"

typedef struct _program_change
{
    uint8_t channel;
    uint8_t program;
} midi_program_change_t;

#ifdef __cplusplus
extern "C"
{
#endif

int midi_program_change_unmarshal(
    midi_program_change_t* ctx,
    const midi_cmd_t cmd,
    const uint8_t* data,
    const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_EVENT_STANDARD_PROGRAM_CHANGE_H
