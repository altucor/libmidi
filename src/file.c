#include "file.h"

#include <stdlib.h>

midi_file_t *midi_file_new()
{
    midi_file_t *ctx = calloc(1, sizeof(midi_file_t));
    return ctx;
}

void midi_file_free(midi_file_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    if (ctx->mtrk != NULL)
    {
        free(ctx->mtrk);
        ctx->mtrk = NULL;
    }
    free(ctx);
}

int midi_file_unmarshal(midi_file_t *ctx, uint8_t *data, const uint32_t size)
{
    int res = 0;
    uint32_t iterator = 0;
    if (res = mthd_unmarshal(&ctx->mthd, data, size), res < 0)
    {
        return -1;
    }
    iterator += res;
    ctx->mtrk = calloc(ctx->mthd.track_count, sizeof(mtrk_t));
    for (uint32_t i = 0; i < ctx->mthd.track_count; i++)
    {
        if (res = mtrk_unmarshal(&ctx->mtrk[i], data+iterator, size), res < 0)
        {
            return -1;
        }
        iterator += res;
    }
    return iterator;
}
