#ifndef MIDI_UTIL_H
#define MIDI_UTIL_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t readu16bswap(uint8_t *data, uint32_t *iterator);
uint32_t readu32(uint8_t *data, uint32_t *iterator);
uint32_t readu32bswap(uint8_t *data, uint32_t *iterator);
bool is_sysex_end_byte(const uint8_t c);
void alloc_and_copy_from_stream(uint8_t *dst, uint8_t *src, const uint32_t size, uint32_t *iterator);

#ifdef __cplusplus
}
#endif

#endif // MIDI_UTIL_H
