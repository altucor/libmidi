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
void alloc_and_copy_from_stream(uint8_t **dst, uint8_t *src, const uint32_t size, uint32_t *iterator);
void alloc_and_copy_from_stream_with_cmd(uint8_t **dst, uint8_t cmd, uint8_t *src, const uint32_t size, uint32_t *iterator);
float pulses_per_second(const uint16_t ppqn, const uint32_t bpm);
float duration_to_ms(const uint32_t vlv_value, const float pulsesPerSecond);
float pitch_to_freq(const uint8_t pitch);
const char *pitch_to_name(const uint8_t pitch);

#ifdef __cplusplus
}
#endif

#endif // MIDI_UTIL_H
