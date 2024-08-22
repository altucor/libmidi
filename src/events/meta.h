#ifndef MIDI_EVENT_META_H
#define MIDI_EVENT_META_H

#include "protocol.h"
#include "vlv.h"

#include <stdint.h>

typedef struct midi_event_meta
{
    uint8_t type;
    vlv_t length;
    uint8_t *data;
} midi_event_meta_t;

void midi_event_meta_reset(midi_event_meta_t *ctx);
midi_event_meta_t *midi_event_meta_new();
void midi_event_meta_free(midi_event_meta_t *ctx);
int midi_event_meta_unmarshal(midi_event_meta_t *ctx, uint8_t *data, const uint32_t size);

#endif // MIDI_EVENT_META_H
