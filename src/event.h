#ifndef MIDI_EVENT_H
#define MIDI_EVENT_H

#include "events/control.h"
#include "events/meta.h"
#include "events/note.h"
#include "events/pitch.h"
#include "protocol.h"
#include "vlv.h"

#include <stdbool.h>
#include <stdint.h>

typedef union midi_event {
    midi_note_t note;
    midi_control_t control;
    midi_pitch_t pitch;
    midi_event_meta_t meta;
} midi_event_t;

typedef struct midi_event_smf
{
    vlv_t predelay;
    midi_cmd_t message;
    uint8_t message_meta;
    midi_event_t event;
} midi_event_smf_t;

#ifdef __cplusplus
extern "C" {
#endif

midi_event_smf_t *midi_event_smf_new();
void midi_event_smf_free(midi_event_smf_t *ctx);
// int midi_event_unmarshal(midi_event_t *ctx, uint8_t *data, uint32_t size);
// bool midi_event_is_system(midi_event_t *ctx);
// bool midi_event_is_note(midi_event_t *ctx);

#ifdef __cplusplus
}
#endif

#endif // MIDI_EVENT_H
