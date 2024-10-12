#ifndef MIDI_VLV_H
#define MIDI_VLV_H

#include "protocol.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct _vlv
{
    uint8_t counter;
    uint32_t val;
} vlv_t;

#ifdef __cplusplus
extern "C" {
#endif

void vlv_reset(vlv_t *ctx);
vlv_t *vlv_new();
void vlv_free(vlv_t *ctx);
bool vlv_feed(vlv_t *ctx, uint8_t b);

#ifdef __cplusplus
}
#endif

#endif // MIDI_VLV_H
