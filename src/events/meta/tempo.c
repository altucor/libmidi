#include "events/meta/tempo.h"

int midi_tempo_unmarshal(midi_tempo_t *ctx, uint8_t *data, uint32_t size)
{
    // FF 51 03 tt tt tt
    uint32_t iterator = 0;
    ctx->val = data[iterator] << 16;
    iterator++;
    ctx->val |= data[iterator] << 8;
    iterator++;
    ctx->val |= data[iterator];
    iterator++;
    ctx->val = 60000000 / ctx->val;

    return iterator;
}