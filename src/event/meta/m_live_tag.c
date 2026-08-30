#include "libmidi/event/meta/m_live_tag.h"
#include "libmidi/errors.h"

#include <memory.h>
#include <stdlib.h>

void midi_m_live_tag_clean(midi_m_live_tag_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    ctx->type = MIDI_M_LIVE_TAG_TYPE_GENRE;
    ctx->size = 0;

    if (ctx->data)
    {
        free(ctx->data);
        ctx->data = NULL;
    }
}

int midi_m_live_tag_unmarshal(midi_m_live_tag_t* ctx, const uint8_t* data, const uint32_t size)
{
    // FF 4B sz tt b1 b2 b3...

    // 1 byte type and at least 1 byte data
    if (!ctx || !data || size < 2)
    {
        return MIDI_ERROR_GENERAL;
    }

    midi_m_live_tag_clean(ctx);

    ctx->type = (m_live_tag_type_e)data[0];

    ctx->data = malloc(size - 1);
    if (!ctx->data)
    {
        return MIDI_ERROR_GENERAL;
    }

    ctx->size = size - 1;
    memcpy(ctx->data, data++, ctx->size);

    return size;
}
