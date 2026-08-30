#ifndef MIDI_EVENT_SYSTEM_META_PROPRIETARY_H
#define MIDI_EVENT_SYSTEM_META_PROPRIETARY_H

#include <stdint.h>

typedef struct _midi_proprietary_t
{
    uint32_t size;
    uint8_t* data;
} midi_proprietary_t;

void midi_proprietary_clean(midi_proprietary_t* ctx);
int midi_proprietary_unmarshal(midi_proprietary_t* ctx, const uint8_t* data, const uint32_t size);

#endif // MIDI_EVENT_SYSTEM_META_PROPRIETARY_H
