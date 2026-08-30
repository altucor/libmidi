#ifndef MIDI_EVENT_META_M_LIVE_TAG_H
#define MIDI_EVENT_META_M_LIVE_TAG_H

#include <stdint.h>

typedef enum _m_live_tag_type_e
{
    MIDI_M_LIVE_TAG_TYPE_GENRE = 0,
    MIDI_M_LIVE_TAG_TYPE_ARTIST,
    MIDI_M_LIVE_TAG_TYPE_COMPOSER,
    MIDI_M_LIVE_TAG_TYPE_DURATION,
    MIDI_M_LIVE_TAG_TYPE_TEMPO,
} m_live_tag_type_e;

typedef struct _midi_m_live_tag_t
{
    m_live_tag_type_e type;
    uint32_t size;
    uint8_t* data;
} midi_m_live_tag_t;

void midi_m_live_tag_clean(midi_m_live_tag_t* ctx);
int midi_m_live_tag_unmarshal(midi_m_live_tag_t* ctx, const uint8_t* data, const uint32_t size);

#endif // MIDI_EVENT_META_M_LIVE_TAG_H
