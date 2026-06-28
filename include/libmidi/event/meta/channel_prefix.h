#ifndef MIDI_SYSTEM_META_CHANNEL_PREFIX_H
#define MIDI_SYSTEM_META_CHANNEL_PREFIX_H

#include <stdint.h>

typedef struct _midi_channel_prefix_t
{
    uint8_t val;
} midi_channel_prefix_t;

#ifdef __cplusplus
extern "C"
{
#endif

int midi_channel_prefix_unmarshal(midi_channel_prefix_t* ctx, const uint8_t* data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_SYSTEM_META_CHANNEL_PREFIX_H
