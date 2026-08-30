#ifndef MIDI_EVENT_SYSTEM_UNITED_H
#define MIDI_EVENT_SYSTEM_UNITED_H

#include "libmidi/event/system/mtc_quarter_frame.h"
#include "libmidi/event/system/song_position.h"
#include "libmidi/event/system/song_select.h"
#include "libmidi/event/system/sysex.h"

typedef union _midi_event_system
{
    midi_mtc_quarter_frame_t mtc_quarter_frame;
    midi_song_position_t song_position;
    midi_song_select_t song_select;
    midi_sysex_t sysex;
} midi_event_system_u;

#endif // MIDI_EVENT_SYSTEM_UNITED_H
