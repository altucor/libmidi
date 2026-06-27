#ifndef MIDI_EVENT_SYSTEM_UNITED_H
#define MIDI_EVENT_SYSTEM_UNITED_H

#include "libmidi/event/system/sysex.h"

typedef union _midi_event_system
{
    midi_sysex_t sysex;
} midi_event_system_u;

#endif // MIDI_EVENT_SYSTEM_UNITED_H
