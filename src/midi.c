#include "midi.h"
#include "protocol.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

midi_t *midi_new(uint8_t channel)
{
    midi_t *ctx = (midi_t *)calloc(1, sizeof(midi_t));
    ctx->channel = channel;
    ctx->state = MIDI_STATE_NEW_STATUS;
    ctx->midi_status = MIDI_STATUS_NOTE_OFF;
    ctx->payload_size = 0;
    ctx->payload_index = 0;
    return ctx;
}

void midi_free(midi_t *ctx)
{
    if (ctx == NULL)
        return;
    free(ctx);
}

void midi_process_new_status_system(midi_t *midi, uint8_t b)
{
    switch (b)
    {
    case MIDI_STATUS_SYSTEM_ACTIVE_SENSING:
        /* code */
        break;

    default:
        break;
    }
}

void midi_process_new_status(midi_t *ctx, uint8_t b)
{
    uint8_t status = (b >> 4) & MIDI_STATUS_MESSAGE_CMD_MASK;
    uint8_t channel = (b & MIDI_STATUS_MESSAGE_CHANNEL_MASK);

    if (status == MIDI_STATUS_SYSTEM)
    {
        midi_process_new_status_system(ctx, channel);
        return;
    }

    if (ctx->channel != MIDI_CHANNEL_ANY && channel != ctx->channel)
    {
        // ignore other channels
        return;
    }

    ctx->midi_status = status;
    ctx->state = MIDI_STATE_READ_CHANNEL_PAYLOAD;

    switch (status)
    {
    case MIDI_STATUS_NOTE_OFF:
    case MIDI_STATUS_NOTE_ON:
    case MIDI_STATUS_KEY_PRESSURE:
    case MIDI_STATUS_CONTROLLER_CHANGE:
    case MIDI_STATUS_PITCH_BEND:
        ctx->payload_size = 2;
        break;
    case MIDI_STATUS_PROGRAM_CHANGE:
    case MIDI_STATUS_CHANNEL_PRESSURE:
        ctx->payload_size = 1;
        break;
    default:
        break;
    }
    // printf("MIDI STATUS: %d\n", status);
}

void midi_synth_send(midi_t *ctx, midi_callback_data_t *synth)
{
    if (synth == NULL || synth->synth == NULL)
        return;
    switch (ctx->midi_status)
    {
    case MIDI_STATUS_NOTE_ON:
        // printf("MIDI_STATUS_NOTE_ON MIDI_STATUS_NOTE_ON MIDI_STATUS_NOTE_ON\n");
        if (synth->note != NULL)
        {
            synth->note(synth->synth, true, ctx->payload[0], ctx->payload[1]);
        }
        break;
    case MIDI_STATUS_NOTE_OFF:
        // printf("MIDI_STATUS_NOTE_OFF MIDI_STATUS_NOTE_OFF MIDI_STATUS_NOTE_OFF\n");
        if (synth->note != NULL)
        {
            synth->note(synth->synth, false, ctx->payload[0], ctx->payload[1]);
        }
        break;
    case MIDI_STATUS_CONTROLLER_CHANGE:
        // printf("MIDI_STATUS_CONTROLLER_CHANGE MIDI_STATUS_CONTROLLER_CHANGE MIDI_STATUS_CONTROLLER_CHANGE\n");
        if (synth->controller != NULL)
        {
            synth->controller(synth->synth, ctx->payload[0], ctx->payload[1]);
        }
        break;
    case MIDI_STATUS_PITCH_BEND:
        // printf("MIDI_STATUS_PITCH_BEND MIDI_STATUS_PITCH_BEND MIDI_STATUS_PITCH_BEND\n");
        if (synth->pitch_bend != NULL)
        {
            uint16_t temp = (ctx->payload[1] << 8) | ctx->payload[0];
            // printf("midi pitch: b1: %03d b2: %03d val: %05d\n", ctx->payload[0], ctx->payload[1], temp);
            synth->pitch_bend(synth->synth, temp);
        }
        break;
    default:
        break;
    }
}

void midi_process_payload(midi_t *ctx)
{
    if (ctx->synth_cb[MIDI_CHANNEL_ANY] != NULL)
    {
        midi_synth_send(ctx, ctx->synth_cb[MIDI_CHANNEL_ANY]);
    }
    for (uint8_t i = 0; i < 16; i++)
    {
        if (ctx->synth_cb[i] == NULL)
            continue;
        midi_synth_send(ctx, ctx->synth_cb[i]);
    }
}

void midi_read_payload(midi_t *ctx, uint8_t b)
{
    // printf("MIDI STATUS: %d\n", ctx->midi_status);
    // printf("BEFORE midi state: %d midi status: %d index: %d size: %d\n", ctx->state, ctx->midi_status, ctx->payload_index,
    // ctx->payload_size);
    if (ctx->payload_size == 0)
    {
        return;
    }
    ctx->payload[ctx->payload_index] = b;
    ctx->payload_index++;
    if (ctx->payload_index >= ctx->payload_size)
    {
        // printf("midi state: %d midi status: %d\n", ctx->state, ctx->midi_status);
        midi_process_payload(ctx);
        ctx->payload_index = 0;
        ctx->payload_size = 0;
        ctx->state = MIDI_STATE_NEW_STATUS;
        // printf("AFTER midi state: %d midi status: %d index: %d size: %d\n", ctx->state, ctx->midi_status, ctx->payload_index,
        // ctx->payload_size);
        return;
    }
}

void midi_feed(midi_t *ctx, uint8_t b)
{
    if ((MIDI_NEW_MESSAGE_BYTE_MASK | (MIDI_STATUS_SYSTEM << 4) | MIDI_STATUS_SYSTEM_ACTIVE_SENSING) == b)
    {
        // printf("ping ignore 0x%02X\n", (MIDI_NEW_MESSAGE | (MIDI_STATUS_SYSTEM << 4) | MIDI_STATUS_SYSTEM_ACTIVE_SENSING));
        // printf("LOL midi state: %d midi status: %d index: %d size: %d\n", ctx->state, ctx->midi_status, ctx->payload_index,
        // ctx->payload_size);
        return;
    }
    // printf("MIDI Processing Byte: 0x%02X\n", b);
    if ((b & MIDI_NEW_MESSAGE_BYTE_MASK) == MIDI_NEW_MESSAGE_BYTE_MASK)
    {
        ctx->state = MIDI_STATE_NEW_STATUS;
    }
    switch (ctx->state)
    {
    case MIDI_STATE_NEW_STATUS:
        midi_process_new_status(ctx, b);
        break;
    case MIDI_STATE_READ_CHANNEL_PAYLOAD:
        midi_read_payload(ctx, b);
        break;
    default:
        break;
    }
}

void midi_link_synth_to_channel(midi_t *ctx, uint8_t channel, midi_callback_data_t *cb_data)
{
    if (channel == MIDI_CHANNEL_ANY)
    {
        ctx->synth_cb[MIDI_CHANNEL_ANY] = cb_data;
        return;
    }
    if (channel >= MIDI_CHANNEL_ANY)
        return;
    ctx->synth_cb[channel] = cb_data;
}
