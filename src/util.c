#include "util.h"

#include "protocol.h"

#include <memory.h>
#include <stdlib.h>

uint16_t readu16bswap(uint8_t *data, uint32_t *iterator)
{
    uint16_t out = *(uint16_t *)&data[*iterator];
#ifdef _WIN32
    out = _byteswap_ushort(out);
#else
    out = __builtin_bswap16(out);
#endif
    *iterator += sizeof(uint16_t);
    return out;
}

uint32_t readu32(uint8_t *data, uint32_t *iterator)
{
    uint32_t out = *(uint32_t *)&data[*iterator];
    *iterator += sizeof(uint32_t);
    return out;
}

uint32_t readu32bswap(uint8_t *data, uint32_t *iterator)
{
    uint32_t out = *(uint32_t *)&data[*iterator];
#ifdef _WIN32
    out = _byteswap_ulong(out);
#else
    out = __builtin_bswap32(out);
#endif
    *iterator += sizeof(uint32_t);
    return out;
}

bool is_sysex_end_byte(const uint8_t c)
{
    return c & (MIDI_NEW_MESSAGE_BYTE_MASK & (MIDI_STATUS_SYSTEM << 4) & MIDI_STATUS_SYSTEM_END_OF_SYSEX);
}

void alloc_and_copy_from_stream(uint8_t **dst, uint8_t *src, const uint32_t size, uint32_t *iterator)
{
    (*dst) = calloc(size, sizeof(uint8_t));
    memcpy((*dst), src + *iterator, size);
    *iterator += size;
}

void alloc_and_copy_from_stream_with_cmd(uint8_t **dst, uint8_t cmd, uint8_t *src, const uint32_t size, uint32_t *iterator)
{
    (*dst) = calloc(size + 1, sizeof(uint8_t));
    (*dst[0]) = cmd;
    memcpy((*dst) + 1, src + *iterator, size);
    *iterator += size;
}

float pulses_per_second(const uint16_t ppqn, const uint32_t bpm)
{
    return (float)(60000.0f / (float)(ppqn * bpm));
}

float duration_to_ms(const uint32_t vlv_value, const float pulsesPerSecond)
{
    return ((float)vlv_value) * pulsesPerSecond;
}

float pitch_to_freq(const uint8_t pitch)
{
    return kNotesFreq[pitch];
}

const char *pitch_to_name(const uint8_t pitch)
{
    return kNotesStr[pitch];
}

bool is_meta_text_event(const uint8_t message_meta)
{
    switch (message_meta)
    {
    case MIDI_META_EVENT_TEXT:
    case MIDI_META_EVENT_COPYRIGHT:
    case MIDI_META_EVENT_TRACK_NAME:
    case MIDI_META_EVENT_INSTRUMENT_NAME:
    case MIDI_META_EVENT_LYRIC_TEXT:
    case MIDI_META_EVENT_TEXT_MARKER:
    case MIDI_META_EVENT_CUE_POINT:
    case MIDI_META_EVENT_PROGRAM_PATCH_NAME:
    case MIDI_META_EVENT_DEVICE_PORT_NAME:
        return true;
    }

    return false;
}
