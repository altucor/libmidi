#include "events/meta.h"

// #include "util.h"

// #include "memory.h"
// #include "stdlib.h"

// void midi_event_meta_reset(midi_event_meta_t *ctx)
// {
//     ctx->type = 0;
//     vlv_reset(&ctx->length);
// }

// midi_event_meta_t *midi_event_meta_new()
// {
//     midi_event_meta_t *ctx = calloc(1, sizeof(midi_event_meta_t));
//     midi_event_meta_reset(ctx);
//     return ctx;
// }

// void midi_event_meta_free(midi_event_meta_t *ctx)
// {
//     if (ctx == NULL)
//     {
//         return;
//     }
//     free(ctx);
// }

// int midi_event_meta_unmarshal(midi_event_meta_t *ctx, uint8_t *data, uint32_t size)
// {
//     int res = 0;
//     uint32_t iterator = 0;
//     (*(uint8_t *)&ctx->type) = data[iterator++];
//     if (res = vlv_unmarshal(&ctx->length, data + iterator, size), res < 0)
//     {
//         return -1;
//     }
//     iterator += res;
//     size -= res;

//     return iterator;
// }

// bool midi_event_meta_is_track_end(midi_event_meta_t *ctx)
// {
//     return ctx->type == MIDI_META_EVENT_TRACK_END;
// }
