#ifndef MIDI_UTIL_H
#define MIDI_UTIL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t readu16bswap(uint8_t *data, uint32_t *iterator);
uint32_t readu32bswap(uint8_t *data, uint32_t *iterator);

#ifdef __cplusplus
}
#endif

#endif // MIDI_UTIL_H
