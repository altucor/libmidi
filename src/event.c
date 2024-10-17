#include "event.h"

#include "events/meta.h"

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

void midi_event_smf_free(midi_event_smf_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    free(ctx);
}
