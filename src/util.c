#include "util.h"

uint16_t readu16bswap(uint8_t *data, uint32_t *iterator)
{
    uint16_t out = *(uint16_t *)&data[*iterator];
    out = __builtin_bswap16(out);
    *iterator += sizeof(uint16_t);
    return out;
}

uint32_t readu32bswap(uint8_t *data, uint32_t *iterator)
{
    uint32_t out = *(uint32_t *)&data[*iterator];
    out = __builtin_bswap32(out);
    *iterator += sizeof(uint32_t);
    return out;
}
