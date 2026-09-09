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

static inline bool midi_event_is_standard(const midi_event_t* ctx)
{
    return ctx->message.status >= MIDI_STATUS_NOTE_OFF && ctx->message.status <= MIDI_STATUS_PITCH_BEND;
}

static inline bool midi_event_is_system(const midi_event_t* ctx)
{
    return !midi_event_is_standard(ctx);
}

static inline bool midi_event_is_meta(const midi_event_t* ctx)
{
    if (midi_event_is_standard(ctx))
    {
        return false;
    }

    switch (ctx->message_meta)
    {
        case MIDI_META_EVENT_SEQUENCE_NUMBER:
        case MIDI_META_EVENT_TEXT:
        case MIDI_META_EVENT_COPYRIGHT:
        case MIDI_META_EVENT_TRACK_NAME:
        case MIDI_META_EVENT_INSTRUMENT_NAME:
        case MIDI_META_EVENT_LYRIC_TEXT:
        case MIDI_META_EVENT_TEXT_MARKER:
        case MIDI_META_EVENT_CUE_POINT:
        case MIDI_META_EVENT_PROGRAM_PATCH_NAME:
        case MIDI_META_EVENT_DEVICE_PORT_NAME:
        case MIDI_META_EVENT_MIDI_CHANNEL_PREFIX:
        case MIDI_META_EVENT_MIDI_PORT:
        case MIDI_META_EVENT_TRACK_END:
        case MIDI_META_EVENT_M_LIVE_TAG:
        case MIDI_META_EVENT_TEMPO:
        case MIDI_META_EVENT_SMPTE_OFFSET:
        case MIDI_META_EVENT_TIME_SIGNATURE:
        case MIDI_META_EVENT_KEY_SIGNATURE:
        case MIDI_META_EVENT_PROPRIETARY_EVENT:
        {
            return true;
        }

        default:
        {
            return false;
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif // MIDI_EVENT_H
