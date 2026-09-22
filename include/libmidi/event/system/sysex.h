#ifndef MIDI_EVENT_SYSTEM_SYSEX_H
#define MIDI_EVENT_SYSTEM_SYSEX_H

#include "libmidi/protocol.h"
#include "libmidi/util.h"
#include "libmidi/event/system/vendor.h"
#include <stdint.h>

typedef enum _midi_sysex_state_e
{
    MIDI_SYSEX_STATE_INIT = 0,
    MIDI_SYSEX_STATE_BEGIN,
    MIDI_SYSEX_STATE_PAYLOAD,
    MIDI_SYSEX_STATE_END,
    MIDI_SYSEX_STATE_MAX
} midi_sysex_state_e;

typedef struct _midi_sysex
{
    midi_sysex_state_e state;
    uint8_t data;
} midi_sysex_t;

#ifdef __cplusplus
extern "C"
{
#endif

static inline int midi_sysex_set(midi_sysex_t* ctx, const uint8_t b)
{
    if (b == 0xF0)
    {
        ctx->state = MIDI_SYSEX_STATE_BEGIN;
    }
    else if (b == 0xF7)
    {
        ctx->state = MIDI_SYSEX_STATE_END;
    }
    else
    {
        ctx->state = MIDI_SYSEX_STATE_PAYLOAD;
    }

    ctx->data = b;

    return 0;
}

// int midi_sysex_unmarshal(midi_sysex_t* ctx, const uint8_t* data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_EVENT_SYSTEM_SYSEX_H
