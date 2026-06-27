#include "libmidi/event/system/sysex.h"
#include "libmidi/errors.h"

#include <memory.h>
#include <stdlib.h>

void midi_sysex_clean(midi_sysex_t* ctx)
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

int midi_sysex_unmarshal(midi_sysex_t* ctx, const uint8_t* data, const uint32_t size)
{
    if (!ctx || !data || !size)
    {
        return MIDI_ERROR_GENERAL;
    }

    midi_sysex_clean(ctx);

    ctx->data = malloc(size);
    if (!ctx->data)
    {
        return MIDI_ERROR_GENERAL;
    }

    ctx->size = size;
    memcpy(ctx->data, data, ctx->size);

    return size;
}
