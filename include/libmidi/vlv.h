#ifndef MIDI_VLV_H
#define MIDI_VLV_H

#include "protocol.h"

#include <stdbool.h>
#include <stdint.h>

typedef union _vlv_byte {
    uint8_t raw;
    struct
    {
        uint8_t val : 7;
        bool continuation : 1;
    };
} vlv_byte_t;

typedef struct _vlv
{
    uint8_t counter;
    uint32_t val;
} vlv_t;

#ifdef __cplusplus
extern "C" {
#endif

void vlv_reset(vlv_t *ctx);
bool vlv_feed(vlv_t *ctx, uint8_t b);
uint32_t vlv_get_value(vlv_t *ctx);
void vlv_set_value(vlv_t *ctx, const uint32_t val);
bool vlv_can_fetch(vlv_t *ctx);
uint8_t vlv_fetch(vlv_t *ctx);

#ifdef __cplusplus
}
#endif

#endif // MIDI_VLV_H
