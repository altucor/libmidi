#ifndef MIDI_EVENT_H
#define MIDI_EVENT_H

#include "libmidi/protocol.h"

#include "libmidi/vlv.h"

#include "libmidi/event/standard/united.h"
#include "libmidi/event/system/united.h"
#include "libmidi/event/meta/united.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct _midi_event
{
    uint32_t predelay;
    midi_cmd_t message;
    midi_meta_event_e message_meta;
    union
    {
        midi_event_standard_u standard;
        midi_event_system_u system;
        midi_event_meta_u meta;
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
