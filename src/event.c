#include "event.h"

#include "events/meta.h"
#include <stdlib.h>

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
