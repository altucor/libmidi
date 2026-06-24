#ifndef MIDI_EVENT_H
#define MIDI_EVENT_H

#include "libmidi/protocol.h"

#include "libmidi/events/channel_pressure.h"
#include "libmidi/events/control.h"
#include "libmidi/events/key_pressure.h"
#include "libmidi/events/meta.h"
#include "libmidi/events/note.h"
#include "libmidi/events/pitch.h"
#include "libmidi/events/program_change.h"
#include "libmidi/vlv.h"

#include <stdbool.h>
#include <stdint.h>

typedef union midi_event
{
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
    uint32_t predelay;
    midi_cmd_t message;
    uint8_t message_meta;
    uint32_t meta_length;
    midi_event_t event;
} midi_event_smf_t;

#ifdef __cplusplus
extern "C"
{
#endif

void midi_event_smf_reset(midi_event_smf_t* ctx);
midi_event_smf_t* midi_event_smf_new();
midi_event_smf_t* midi_event_smf_new_from(
    const uint32_t predelay,
    midi_cmd_t msg,
    uint8_t message_meta,
    midi_event_t* event);
void midi_event_smf_free(midi_event_smf_t* ctx);

#ifdef __cplusplus
}
#endif

#endif // MIDI_EVENT_H
