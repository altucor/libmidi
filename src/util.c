#include "util.h"

#include "protocol.h"

#include <memory.h>
#include <stdlib.h>

uint16_t readu16bswap(uint8_t *data, uint32_t *iterator)
{
    uint16_t out = *(uint16_t *)&data[*iterator];
    out = __builtin_bswap16(out);
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
    out = __builtin_bswap32(out);
    *iterator += sizeof(uint32_t);
    return out;
}

bool is_sysex_end_byte(const uint8_t c)
{
    return c & (MIDI_NEW_MESSAGE_BYTE_MASK & (MIDI_STATUS_SYSTEM << 4) & MIDI_STATUS_SYSTEM_END_OF_SYSEX);
}

void alloc_and_copy_from_stream(uint8_t *dst, uint8_t *src, const uint32_t size, uint32_t *iterator)
{
    dst = calloc(size, sizeof(uint8_t));
    memcpy(dst, src + *iterator, size);
    *iterator += size;
}