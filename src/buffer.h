#ifndef MIDI_BUFFER_H
#define MIDI_BUFFER_H

#include <stdint.h>

#define BUFFER_SIZE (32)

typedef struct buffer
{
    uint32_t size;
    uint32_t iterator_write;
    uint32_t iterator_read;
    uint8_t data[BUFFER_SIZE];
} buffer_t;

#ifdef __cplusplus
extern "C" {
#endif

void buffer_reset(buffer_t *ctx);
void buffer_set_size(buffer_t *ctx, const uint32_t size);
uint32_t buffer_space_left(buffer_t *ctx);
void buffer_append_u8(buffer_t *ctx, const uint8_t b);

#ifdef __cplusplus
}
#endif

#endif // MIDI_BUFFER_H
