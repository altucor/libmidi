#ifndef MIDI_EVENT_SYSTEM_SONG_SELECT_H
#define MIDI_EVENT_SYSTEM_SONG_SELECT_H

#include <stdint.h>

typedef union _song_select_u
{
    uint8_t raw;
    struct
    {
        uint8_t val : 7;
        uint8_t ____reserved : 1;
    };
} song_select_u;

typedef struct _midi_song_select_t
{
    song_select_u value;
} midi_song_select_t;

int midi_song_select_unmarshal(midi_song_select_t* ctx, const uint8_t* data, const uint32_t size);

#endif // MIDI_EVENT_SYSTEM_SONG_SELECT_H
