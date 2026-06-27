#ifndef MIDI_EVENT_META_UNITED_H
#define MIDI_EVENT_META_UNITED_H

#include "libmidi/event/meta/channel.h"
#include "libmidi/event/meta/sequence_number.h"
#include "libmidi/event/meta/tempo.h"
#include "libmidi/event/meta/text.h"
#include "libmidi/event/meta/time_signature.h"

typedef union _midi_event_meta
{
    midi_channel_t channel;
    midi_tempo_t tempo;
    midi_time_signature_t time_signature;
    midi_text_event_t text;
    midi_sequence_number_t sequence_number;
} midi_event_meta_u;

#endif // MIDI_EVENT_META_UNITED_H
