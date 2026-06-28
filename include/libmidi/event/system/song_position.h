#ifndef MIDI_EVENT_SYSTEM_SONG_POSITION_H
#define MIDI_EVENT_SYSTEM_SONG_POSITION_H

#include <stdint.h>

typedef struct _midi_song_position_t
{
    uint16_t value; // actually 14-bit value
} midi_song_position_t;

int midi_song_position_unmarshal(midi_song_position_t* ctx, const uint8_t* data, const uint32_t size);

#endif // MIDI_EVENT_SYSTEM_SONG_POSITION_H
