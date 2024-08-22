#ifndef MIDI_VLV_H
#define MIDI_VLV_H

#include "protocol.h"
#include <stdint.h>

typedef struct vlv
{
    uint8_t counter;
    uint32_t val;
} vlv_t;

void vlv_reset(vlv_t *ctx);
vlv_t *vlv_new();
void vlv_free(vlv_t *ctx);
int vlv_unmarshal(vlv_t *ctx, uint8_t *data, const uint32_t size);

#endif // MIDI_VLV_H