#ifndef MIDI_EVENT_SYSTEM_META_SMPTE_OFFSET_H
#define MIDI_EVENT_SYSTEM_META_SMPTE_OFFSET_H

#include <stdint.h>

typedef enum _smpte_fps_t
{
    SMPTE_FPS_24,
    SMPTE_FPS_25,
    SMPTE_FPS_29_97,
    SMPTE_FPS_30
} smpte_fps_t;

typedef union _hours_offset_u
{
    uint8_t raw;
    struct
    {
        uint8_t hours : 5;
        smpte_fps_t fps : 2;
        uint8_t ____reserved : 1;
    };

} hours_offset_u;

typedef struct _midi_smpte_offset_t
{
    hours_offset_u hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t frames;
    uint8_t subframes;
} midi_smpte_offset_t;

int midi_smpte_offset_unmarshal(midi_smpte_offset_t* ctx, const uint8_t* data, const uint32_t size);

#endif // MIDI_EVENT_SYSTEM_META_SMPTE_OFFSET_H
