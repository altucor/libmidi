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
    free(ctx);
}

bool midi_file_unmarshal(midi_file_t *ctx, uint8_t *data, const uint32_t size)
{
    return false;
}
