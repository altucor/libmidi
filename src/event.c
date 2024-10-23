#include "event.h"

#include "events/meta.h"
#include "util.h"

#include <memory.h>
#include <stdlib.h>

void midi_event_smf_reset(midi_event_smf_t *ctx)
{
    ctx->predelay = 0;
    ctx->message.raw = 0;
    ctx->message_meta = 0;
    ctx->meta_length = 0;
    memset(&ctx->event, 0x00, sizeof(midi_event_t));
}

midi_event_smf_t *midi_event_smf_new()
{
    midi_event_smf_t *ctx = calloc(1, sizeof(midi_event_smf_t));
    ctx->predelay = 0;
    return ctx;
}

midi_event_smf_t *midi_event_smf_new_from(const uint32_t predelay, midi_cmd_t msg, uint8_t message_meta, midi_event_t *event)
{
    midi_event_smf_t *ctx = midi_event_smf_new();
    ctx->predelay = predelay;
    ctx->message = msg;
    ctx->message_meta = message_meta;
    ctx->event = *event;

    if (is_meta_text_event(ctx->message_meta))
    {
        ctx->event.meta.text.data = malloc(event->meta.text.size);
        midi_text_event_copy(&ctx->event.meta.text, &event->meta.text);
    }
    return ctx;
}

void midi_event_smf_free(midi_event_smf_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }

    if (is_meta_text_event(ctx->message_meta))
    {
        midi_text_event_free_data(&ctx->event.meta.text);
    }

    free(ctx);
}
