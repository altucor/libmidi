#include "libmidi/event/meta/text.h"
#include "libmidi/errors.h"

#include <memory.h>
#include <stdlib.h>

void midi_text_event_cleanup(midi_text_event_t* ctx)
{
    if (!ctx || !ctx->size || !ctx->data)
    {
        return;
    }

    free(ctx->data);
}

int midi_text_event_unmarshal(midi_text_event_t* ctx, const uint8_t* data, const uint32_t size)
{
    // FF 0X XX cc cc cc...

    if (!ctx || !data || !size)
    {
        return MIDI_ERROR_GENERAL;
    }

    ctx->size = size;
    ctx->data = calloc(size, sizeof(char));

    if (!ctx->data)
    {
        return MIDI_ERROR_GENERAL;
    }

    memcpy(ctx->data, data, size);

    return size;
}

void midi_text_event_copy(midi_text_event_t* dst, const midi_text_event_t* src)
{
    if (!dst || !src)
    {
        return;
    }

    dst->size = src->size;
    memcpy(dst->data, src->data, src->size);
}

midi_text_event_t midi_text_event_new_from(const midi_text_event_t* src)
{
    midi_text_event_t out = {.size = 0, .data = NULL};

    if (!src)
    {
        return out;
    }

    out.data = calloc(src->size, sizeof(char));
    if (!out.data)
    {
        return out;
    }

    out.size = src->size;
    midi_text_event_copy(&out, src);

    return out;
}

midi_text_event_t midi_text_event_new_from_string(const char* string)
{
    midi_text_event_t out = {.size = 0, .data = NULL};

    if (!string)
    {
        return out;
    }

    const size_t size = strlen(string);
    if (!size || size >= UINT32_MAX)
    {
        return out;
    }

    out.data = calloc(size, sizeof(char));
    if (!out.data)
    {
        return out;
    }

    out.size = (uint32_t)size;
    memcpy(out.data, string, size);

    return out;
}
