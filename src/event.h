#ifndef MIDI_EVENT_H
#define MIDI_EVENT_H

#include "events/channel_pressure.h"
#include "events/control.h"
#include "events/key_pressure.h"
#include "events/meta.h"
#include "events/note.h"
#include "events/pitch.h"
#include "events/program_change.h"
#include "protocol.h"
#include "vlv.h"

#include <stdbool.h>
#include <stdint.h>

typedef union midi_event {
    midi_note_t note;
    midi_key_pressure_t key_pressure;
    midi_control_t control;
    midi_program_change_t program_change;
    midi_channel_pressure_t channel_pressure;
    midi_pitch_t pitch;
    midi_event_meta_t meta;
} midi_event_t;

typedef struct midi_event_smf
{
    vlv_t predelay;
    midi_cmd_t message;
    uint8_t message_meta;
    vlv_t meta_length;
    midi_event_t event;
} midi_event_smf_t;

#ifdef __cplusplus
extern "C" {
#endif

void midi_event_smf_reset(midi_event_smf_t *ctx);
midi_event_smf_t *midi_event_smf_new();
void midi_event_smf_free(midi_event_smf_t *ctx);

#ifdef __cplusplus
}
#endif

#endif // MIDI_EVENT_H
