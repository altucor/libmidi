#include "libmidi/file.h"

#include <stdlib.h>

midi_file_t* midi_file_new()
{
    midi_file_t* ctx = calloc(1, sizeof(midi_file_t));
    if (!ctx)
    {
        return NULL;
    }

    ctx->device = midi_input_device_new(true);
    if (!ctx->device)
    {
        midi_file_free(ctx);
        return NULL;
    }

    ctx->mtrk = NULL;
    return ctx;
}

void midi_file_free(midi_file_t* ctx)
{
    if (ctx == NULL)
    {
        return;
    }

    if (ctx->device)
    {
        midi_input_device_free(ctx->device);
    }

    if (ctx->mtrk && ctx->mthd.track_count != 0)
    {
        for (uint32_t i = 0; i < ctx->mthd.track_count; i++)
        {
            if (ctx->mtrk[i])
            {
                mtrk_free(ctx->mtrk[i]);
            }
        }

        free(ctx->mtrk);
        ctx->mtrk = NULL;
    }

    free(ctx);
}

int midi_file_unmarshal(midi_file_t* ctx, uint8_t* data, const uint32_t size)
{
    if (!ctx || !data || !size)
    {
        return -1;
    }

    int res = 0;
    uint32_t iterator = 0;

    if (res = mthd_unmarshal(&ctx->mthd, data, size - iterator), res < 0)
    {
        return -1;
    }

    iterator += res;
    ctx->mtrk = calloc(ctx->mthd.track_count, sizeof(mtrk_t));
    if (!ctx->mtrk)
    {
        return -1;
    }

    for (uint32_t i = 0; i < ctx->mthd.track_count; i++)
    {
        ctx->mtrk[i] = mtrk_new(ctx->device);
        if (!ctx->mtrk[i])
        {
            return -1;
        }

        if (res = mtrk_unmarshal(ctx->mtrk[i], data + iterator, size), res < 0)
        {
            return -1;
        }

        iterator += res;
    }

    return iterator;
}

mthd_t midi_file_get_mthd(midi_file_t* ctx)
{
    if (!ctx)
    {
        mthd_t mthd = {};
        return mthd;
    }

    return ctx->mthd;
}

uint16_t midi_file_get_tracks_count(midi_file_t* ctx)
{
    if (!ctx)
    {
        return 0;
    }

    return ctx->mthd.track_count;
}

mtrk_t* midi_file_get_track(midi_file_t* ctx, const uint16_t index)
{
    if (!ctx)
    {
        return NULL;
    }

    if (!ctx || index >= ctx->mthd.track_count)
    {
        return NULL;
    }

    return ctx->mtrk[index];
}
