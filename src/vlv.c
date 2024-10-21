#include "vlv.h"

#include <stdio.h>

void vlv_reset(vlv_t *ctx)
{
    ctx->counter = 0;
    ctx->val = 0;
}

bool vlv_feed(vlv_t *ctx, uint8_t b)
{
    if (ctx->counter >= 4)
    {
        return true;
    }
    ctx->counter++;
    vlv_byte_t vlv_byte = {.raw = b};
    ctx->val += vlv_byte.val;
    if (vlv_byte.continuation)
    {
        ctx->val = (ctx->val << 7);
        return false;
    }
    return true; // means full
}

uint32_t vlv_get_value(vlv_t *ctx)
{
    return ctx->val;
}

void vlv_set_value(vlv_t *ctx, const uint32_t val)
{
    ctx->val = val;
    if ((ctx->val & 0xFE00000) != 0)
    {
        ctx->counter = (MIDI_VLV_MAX_SIZE - 4);
        return;
    }
    if ((ctx->val & 0x1FC000) != 0)
    {
        ctx->counter = (MIDI_VLV_MAX_SIZE - 3);
        return;
    }
    if ((ctx->val & 0x3F80) != 0)
    {
        ctx->counter = (MIDI_VLV_MAX_SIZE - 2);
        return;
    }

    ctx->counter = (MIDI_VLV_MAX_SIZE - 1);
}

bool vlv_can_fetch(vlv_t *ctx)
{
    return ctx->counter <= (MIDI_VLV_MAX_SIZE - 1) && ctx->val != 0;
}

uint8_t vlv_fetch(vlv_t *ctx)
{
    if (!vlv_can_fetch(ctx))
    {
        return 0;
    }

    uint8_t masked_val = MIDI_VLV_EXTRACT_BYTE_AT_INDEX(ctx->val, ctx->counter);
    vlv_byte_t vlv_byte = {.raw = masked_val};
    ctx->counter++;
    if (vlv_can_fetch(ctx))
    {
        vlv_byte.continuation = true;
    }
    return vlv_byte.raw;
}
