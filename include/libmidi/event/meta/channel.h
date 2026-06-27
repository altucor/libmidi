#ifndef MIDI_CHANNEL_H
#define MIDI_CHANNEL_H

#include <stdint.h>

typedef struct _midi_channel_t
{
    uint16_t val;
} midi_channel_t;

#ifdef __cplusplus
extern "C"
{
#endif

int midi_channel_unmarshal(midi_channel_t* ctx, const uint8_t* data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_CHANNEL_H
