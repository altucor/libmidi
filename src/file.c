#include "file.h"

#include <stdio.h>
#include <stdlib.h>

midi_file_t *midi_file_new()
{
    midi_file_t *ctx = calloc(1, sizeof(midi_file_t));
    ctx->state_machine = input_state_machine_new(true);
    return ctx;
}

void midi_file_free(midi_file_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    input_state_machine_free(ctx->state_machine);
    if (ctx->mtrk != NULL && ctx->mthd.track_count != 0)
    {
        for (uint32_t i = 0; i < ctx->mthd.track_count; i++)
        {
            mtrk_free(ctx->mtrk[i]);
        }
        free(ctx->mtrk);
        ctx->mtrk = NULL;
    }
    free(ctx);
}

int midi_file_debug(midi_file_t *ctx, char *data, uint32_t size)
{
    uint32_t iterator = 0;
    iterator += mthd_debug(&ctx->mthd, data + iterator, size);
    for (uint32_t i = 0; i < ctx->mthd.track_count; i++)
    {
        iterator += sprintf(data + iterator, "CurrentTrack #%04d\n", i);
        iterator += mtrk_debug(ctx->mtrk[i], data + iterator, size);
    }

    return iterator;
}

int midi_file_unmarshal(midi_file_t *ctx, uint8_t *data, uint32_t size)
{
    int res = 0;
    uint32_t iterator = 0;
    if (res = mthd_unmarshal(&ctx->mthd, data, size), res < 0)
    {
        return -1;
    }
    iterator += res;
    size -= res;

    ctx->mtrk = calloc(ctx->mthd.track_count, sizeof(mtrk_t));
    for (uint32_t i = 0; i < ctx->mthd.track_count; i++)
    {
        ctx->mtrk[i] = calloc(1, sizeof(mtrk_t));
        if (res = mtrk_unmarshal(ctx->mtrk[i], data + iterator, size), res < 0)
        {
            return -1;
        }
        iterator += res;
        size -= res;
    }
    return iterator;
}
