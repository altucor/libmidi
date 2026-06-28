#ifndef MIDI_EVENT_SYSTEM_META_KEY_SIGNATURE_H
#define MIDI_EVENT_SYSTEM_META_KEY_SIGNATURE_H

#include <stdint.h>

typedef enum _midi_scale_e : uint8_t
{
    MIDI_SCALE_MAJOR,
    MIDI_SCALE_MINOR
} midi_scale_e;

typedef struct _midi_key_signature_t
{
    int8_t sharps_flats;
    midi_scale_e scale;
} midi_key_signature_t;

#ifdef __cplusplus
extern "C"
{
#endif

int midi_key_signature_unmarshal(midi_key_signature_t* ctx, const uint8_t* data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_EVENT_SYSTEM_META_KEY_SIGNATURE_H
