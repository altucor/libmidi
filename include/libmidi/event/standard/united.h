#ifndef MIDI_EVENT_STANDARD_UNITED_H
#define MIDI_EVENT_STANDARD_UNITED_H

#include "libmidi/event/standard/note.h"
#include "libmidi/event/standard/key_pressure.h"
#include "libmidi/event/standard/control.h"
#include "libmidi/event/standard/program_change.h"
#include "libmidi/event/standard/channel_pressure.h"
#include "libmidi/event/standard/pitch.h"

typedef union _midi_event_standard
{
    midi_note_t note;
    midi_key_pressure_t key_pressure;
    midi_control_t control;
    midi_program_change_t program_change;
    midi_channel_pressure_t channel_pressure;
    midi_pitch_t pitch;
} midi_event_standard_u;

#endif // MIDI_EVENT_STANDARD_UNITED_H
