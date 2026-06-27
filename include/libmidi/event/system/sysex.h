#ifndef MIDI_EVENT_SYSTEM_SYSEX_H
#define MIDI_EVENT_SYSTEM_SYSEX_H

#include "libmidi/protocol.h"
#include "libmidi/util.h"

#include <stdint.h>

typedef struct _midi_sysex
{
    uint32_t size;
    uint8_t* data;
} midi_sysex_t;

#ifdef __cplusplus
extern "C"
{
#endif

int midi_sysex_unmarshal(midi_sysex_t* ctx, const uint8_t* data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_EVENT_SYSTEM_SYSEX_H
