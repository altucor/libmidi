#ifndef MIDI_EVENT_META_H
#define MIDI_EVENT_META_H

#include "meta/tempo.h"
#include "meta/time_signature.h"
#include "protocol.h"

typedef union midi_event_meta {
    midi_tempo_t tempo;
    midi_time_signature_t time_signature;
} midi_event_meta_t;

#endif // MIDI_EVENT_META_H
