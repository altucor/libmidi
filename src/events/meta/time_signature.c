#include "time_signature.h"

int midi_time_signature_unmarshal(midi_time_signature_t *ctx, uint8_t *data, uint32_t size)
{
    // FF 58 04 nn dd cc bb
    ctx->nn = data[0];
    ctx->dd = data[1];
    ctx->cc = data[2];
    ctx->bb = data[3];

    return 4;
}