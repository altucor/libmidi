#include "vlv.h"

#include <stdlib.h>

void vlv_reset(vlv_t *ctx)
{
    ctx->counter = 0;
    ctx->val = 0;
}

vlv_t *vlv_new()
{
    vlv_t *ctx = calloc(1, sizeof(vlv_t));
    vlv_reset(ctx);
    return ctx;
}

void vlv_free(vlv_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    free(ctx);
}

bool vlv_feed(vlv_t *ctx, uint8_t b)
{
    if (ctx->counter >= 4)
    {
        return true;
    }
    ctx->counter++;
    ctx->val += (b & MIDI_VLV_DATA_MASK);
    if (b & MIDI_VLV_CONTINUATION_BIT)
    {
        ctx->val = (ctx->val << 7);
        return false;
    }
    return true; // means full
}

int vlv_unmarshal(vlv_t *ctx, uint8_t *data, uint32_t size)
{
    uint32_t iterator = 0;

    while (1)
    {
        if (size == 0)
        {
            return -1;
        }
        uint8_t b = data[iterator++];
        size--;

        ctx->val += (b & MIDI_VLV_DATA_MASK);
        if (b & MIDI_VLV_CONTINUATION_BIT)
        {
            ctx->val = (ctx->val << 7);
        }
        if (iterator >= MIDI_VLV_MAX_SIZE || !(b & MIDI_VLV_CONTINUATION_BIT))
        {
            break;
        }
    }
    return iterator;
}