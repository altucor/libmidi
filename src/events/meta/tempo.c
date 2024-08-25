#include "tempo.h"

int midi_tempo_unmarshal(midi_tempo_t *ctx, uint8_t *data, uint32_t size)
{
    // FF 51 03 tt tt tt
    ctx->val = data[0] << 16;
    ctx->val |= data[1] << 8;
    ctx->val |= data[2];
    ctx->val /= 60000000;
}