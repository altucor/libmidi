#ifndef MIDI_EVENT_SYSTEM_MTC_QUARTER_FRAME_H
#define MIDI_EVENT_SYSTEM_MTC_QUARTER_FRAME_H

// TODO: hours field in MTC_QUARTER_FRAME should be handled in
// the same way as in SMPTE offset to get FPS
#include "libmidi/event/meta/smpte_offset.h"

#include <stdint.h>

// nibble - 4 bits of data
// LSN - Least significant nibble
// MSN - Most significant nibble

typedef enum _mtc_piece_e : uint8_t
{
    MTC_PIECE_FRAMES_LSN = 0,
    MTC_PIECE_FRAMES_MSN,
    MTC_PIECE_SECONDS_LSN,
    MTC_PIECE_SECONDS_MSN,
    MTC_PIECE_MINUTES_LSN,
    MTC_PIECE_MINUTES_MSN,
    MTC_PIECE_HOURS_LSN,
    MTC_PIECE_HOURS_MSN,
} mtc_piece_e;

typedef union _mtc_u
{
    uint8_t raw;
    struct
    {
        uint8_t data : 4;
        mtc_piece_e piece : 3;
        uint8_t ____reserved : 1;
    };
} mtc_u;

typedef struct _midi_mtc_quarter_frame_t
{
    mtc_u value;
} midi_mtc_quarter_frame_t;

int midi_mtc_quarter_frame_unmarshal(midi_mtc_quarter_frame_t* ctx, const uint8_t* data, const uint32_t size);

#endif // MIDI_EVENT_SYSTEM_MTC_QUARTER_FRAME_H
