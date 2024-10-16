#include "event.h"

#include "events/meta.h"

#include <memory.h>
#include <stdlib.h>

void midi_event_smf_reset(midi_event_smf_t *ctx)
{
    vlv_reset(&ctx->predelay);
    ctx->message.status = 0x00;
    ctx->message.channel = 0x00;
    ctx->message_meta = 0x00;
    vlv_reset(&ctx->meta_length);
    memset(&ctx->event, 0x00, sizeof(midi_event_t));
}

midi_event_smf_t *midi_event_smf_new()
{
    midi_event_smf_t *ctx = calloc(1, sizeof(midi_event_smf_t));
    vlv_reset(&ctx->predelay);
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
