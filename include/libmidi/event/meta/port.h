#ifndef MIDI_SYSTEM_META_PORT_H
#define MIDI_SYSTEM_META_PORT_H

#include <stdint.h>

typedef struct _midi_port_t
{
    uint8_t val;
} midi_port_t;

#ifdef __cplusplus
extern "C"
{
#endif

int midi_port_unmarshal(midi_port_t* ctx, const uint8_t* data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_SYSTEM_META_PORT_H
