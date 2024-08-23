#include "events/control.h"

#include "protocol.h"

void midi_control_reset(midi_control_t *ctx)
{
    ctx->channel = 0;
    ctx->control = 0;
    ctx->value = 0;
}

int midi_control_unmarshal(midi_control_t *ctx, uint8_t *data, uint32_t size)
{
    uint32_t iterator = 0;
    ctx->channel = (data[iterator++] & MIDI_STATUS_MESSAGE_SUBCMD_MASK);
    ctx->control = data[iterator++];
    ctx->value = data[iterator++];
    return iterator;
}