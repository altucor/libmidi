#ifndef MIDI_EVENT_META_H
#define MIDI_EVENT_META_H

#include "libmidi/event/meta/tempo.h"
#include "libmidi/event/meta/text.h"
#include "libmidi/event/meta/time_signature.h"

typedef union _midi_event_meta
{
    midi_tempo_t tempo;
    midi_time_signature_t time_signature;
    midi_text_event_t text;
} midi_event_meta_t;

#endif // MIDI_EVENT_META_H
