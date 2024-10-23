#include "events/meta/text.h"

#include <memory.h>
#include <stdlib.h>

void midi_text_event_free_data(midi_text_event_t *ctx)
{
    if (ctx == NULL || !ctx->size || ctx->data == NULL)
    {
        return;
    }

    free(ctx->data);
}

int midi_text_event_unmarshal(midi_text_event_t *ctx, uint8_t *data, uint32_t size)
{
    // FF 0X XX cc cc cc...
    uint32_t iterator = 0;
    if (size < 1)
    {
        return iterator;
    }
    ctx->size = size;
    ctx->data = (char *)data;
    iterator += size;
    return iterator;
}

void midi_text_event_copy(midi_text_event_t *dst, midi_text_event_t *src)
{
    memcpy(dst->data, src->data, src->size);
}