#include "libmidi/event/meta/proprietary.h"
#include "libmidi/errors.h"

#include <memory.h>
#include <stdlib.h>

void midi_proprietary_clean(midi_proprietary_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    ctx->size = 0;

    if (ctx->data)
    {
        free(ctx->data);
        ctx->data = NULL;
    }
}

int midi_proprietary_unmarshal(midi_proprietary_t* ctx, const uint8_t* data, const uint32_t size)
{
    // FF 7F sz b1 b2 b3...

    if (!ctx || !data || !size)
    {
        return MIDI_ERROR_GENERAL;
    }

    midi_proprietary_clean(ctx);

    ctx->data = malloc(size);
    if (!ctx->data)
    {
        return MIDI_ERROR_GENERAL;
    }

    ctx->size = size;
    memcpy(ctx->data, data, ctx->size);

    return size;
}
