#ifndef MIDI_UTIL_H
#define MIDI_UTIL_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t readu16bswap(const uint8_t *data, uint32_t *iterator);
uint32_t readu32(const uint8_t *data, uint32_t *iterator);
uint32_t readu32bswap(const uint8_t *data, uint32_t *iterator);
float pulses_per_second(const uint16_t ppqn, const uint32_t bpm);
float duration_to_ms(const uint32_t vlv_value, const float pulsesPerSecond);
float pitch_to_freq(const uint8_t pitch);
const char *pitch_to_name(const uint8_t pitch);
bool is_meta_text_event(const uint8_t message_meta);

#ifdef __cplusplus
}
#endif

#endif // MIDI_UTIL_H
