#ifndef MIDI_EVENT_H
#define MIDI_EVENT_H

#include "libmidi/protocol.h"

#include "libmidi/event/channel_pressure.h"
#include "libmidi/event/control.h"
#include "libmidi/event/key_pressure.h"
#include "libmidi/event/meta.h"
#include "libmidi/event/note.h"
#include "libmidi/event/pitch.h"
#include "libmidi/event/program_change.h"
#include "libmidi/vlv.h"

#include <stdbool.h>
#include <stdint.h>

typedef union _midi_event_standard
{
    midi_note_t note;
    midi_key_pressure_t key_pressure;
    midi_control_t control;
    midi_program_change_t program_change;
    midi_channel_pressure_t channel_pressure;
    midi_pitch_t pitch;
} midi_event_standard_t;

typedef struct _midi_event
{
    uint32_t predelay;
    midi_cmd_t message;
    midi_meta_event_e message_meta;
    union
    {
        midi_event_standard_t standard;
        midi_event_meta_t meta;
    };
} midi_event_t;

#ifdef __cplusplus
extern "C"
{
#endif

void midi_event_reset(midi_event_t* ctx);
midi_event_t* midi_event_new();
void midi_event_copy(midi_event_t* dst, const midi_event_t* src);
void midi_event_cleanup(midi_event_t* ctx);
void midi_event_free(midi_event_t* ctx);

#ifdef __cplusplus
}
#endif

#endif // MIDI_EVENT_H
