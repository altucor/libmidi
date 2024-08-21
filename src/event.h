#ifndef MIDI_EVENT_H
#define MIDI_EVENT_H

#include "protocol.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct midi_event
{
    midi_status_e status;
    uint32_t size;
    uint8_t *data;
} midi_event_t;

#ifdef __cplusplus
extern "C" {
#endif

midi_event_t *midi_event_new();
void midi_event_free(midi_event_t *ctx);
bool midi_event_unmarshal(midi_event_t *ctx, uint8_t *data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_EVENT_H
