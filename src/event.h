#ifndef MIDI_EVENT_H
#define MIDI_EVENT_H

#include "events/meta.h"
#include "protocol.h"
#include "vlv.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct midi_event
{
    vlv_t predelay;
    midi_cmd_t cmd;
    uint32_t size;
    midi_event_meta_t meta;
    uint8_t *data;
} midi_event_t;

#ifdef __cplusplus
extern "C" {
#endif

midi_event_t *midi_event_new();
void midi_event_free(midi_event_t *ctx);
int midi_event_unmarshal(midi_event_t *ctx, uint8_t *data, uint32_t size);
bool midi_event_is_system(midi_event_t *ctx);
bool midi_event_is_note(midi_event_t *ctx);

#ifdef __cplusplus
}
#endif

#endif // MIDI_EVENT_H
