#include "events/meta/text.h"

#include <memory.h>
#include <stdlib.h>

int midi_text_event_unmarshal(midi_text_event_t *ctx, uint8_t *data, uint32_t size)
{
    // FF 0X XX cc cc cc...
    uint32_t iterator = 0;
    if (size < 1)
    {
        return iterator;
    }
    ctx->val = malloc(size + 1);
    memcpy(ctx->val, data, size);
    ctx->val[size] = 0x00;

    return iterator;
}