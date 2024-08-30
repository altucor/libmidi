#include "events/program_change.h"

int midi_program_change_unmarshal(midi_program_change_t *ctx, midi_cmd_t cmd, uint8_t *data, uint32_t size)
{
    uint32_t iterator = 0;
    ctx->channel = cmd.subCmd;
    ctx->program = data[iterator++];
    return iterator;
}
