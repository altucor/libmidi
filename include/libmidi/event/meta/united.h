#ifndef MIDI_EVENT_META_UNITED_H
#define MIDI_EVENT_META_UNITED_H

#include "libmidi/event/meta/channel_prefix.h"
#include "libmidi/event/meta/key_signature.h"
#include "libmidi/event/meta/m_live_tag.h"
#include "libmidi/event/meta/port.h"
#include "libmidi/event/meta/proprietary.h"
#include "libmidi/event/meta/sequence_number.h"
#include "libmidi/event/meta/smpte_offset.h"
#include "libmidi/event/meta/tempo.h"
#include "libmidi/event/meta/text.h"
#include "libmidi/event/meta/time_signature.h"

typedef union _midi_event_meta
{
    midi_channel_prefix_t channel_prefix;
    midi_key_signature_t key_signature;
    midi_m_live_tag_t m_live_tag;
    midi_port_t port;
    midi_proprietary_t proprietary;
    midi_tempo_t tempo;
    midi_time_signature_t time_signature;
    midi_text_event_t text;
    midi_sequence_number_t sequence_number;
    midi_smpte_offset_t smpte_offset;
} midi_event_meta_u;

#endif // MIDI_EVENT_META_UNITED_H
