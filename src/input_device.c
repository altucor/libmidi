#include "input_device.h"

#include <stdio.h>
#include <stdlib.h>

midi_input_device_t *midi_input_device_new(uint8_t channel)
{
    midi_input_device_t *ctx = (midi_input_device_t *)calloc(1, sizeof(midi_input_device_t));
    ctx->channel = channel;
    ctx->state = MIDI_INPUT_DEVICE_STATE_NEW_STATUS;
    ctx->message.status = 0;
    ctx->message.subCmd = 0;
    // ctx->payload_size = 0;
    // ctx->payload_index = 0;
    return ctx;
}

void midi_input_device_free(midi_input_device_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    free(ctx);
}

void midi_input_device_process_new_status_system(midi_input_device_t *midi, uint8_t b)
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

// void midi_input_device_process_new_status(midi_input_device_t *ctx, uint8_t b)
// {
//     uint8_t status = (b >> 4) & MIDI_STATUS_MESSAGE_CMD_MASK;
//     uint8_t channel = (b & MIDI_STATUS_MESSAGE_SUBCMD_MASK);

//     if (status == MIDI_STATUS_SYSTEM)
//     {
//         midi_input_device_process_new_status_system(ctx, channel);
//         return;
//     }

//     if (ctx->channel != MIDI_CHANNEL_ANY && channel != ctx->channel)
//     {
//         // ignore other channels
//         return;
//     }

//     ctx->message.status = status;
//     ctx->state = MIDI_INPUT_DEVICE_STATE_READ_CHANNEL_PAYLOAD;

//     switch (status)
//     {
//     case MIDI_STATUS_NOTE_OFF:
//     case MIDI_STATUS_NOTE_ON:
//     case MIDI_STATUS_KEY_PRESSURE:
//     case MIDI_STATUS_CONTROLLER_CHANGE:
//     case MIDI_STATUS_PITCH_BEND:
//         ctx->payload_size = 2;
//         break;
//     case MIDI_STATUS_PROGRAM_CHANGE:
//     case MIDI_STATUS_CHANNEL_PRESSURE:
//         ctx->payload_size = 1;
//         break;
//     default:
//         break;
//     }
//     // printf("MIDI STATUS: %d\n", status);
// }

void midi_input_device_synth_send(midi_input_device_t *ctx, midi_device_callback_data_t *synth)
{
    if (synth == NULL || synth->synth == NULL)
        return;
    switch (ctx->message.status)
    {
    case MIDI_STATUS_NOTE_ON:
        // printf("MIDI_STATUS_NOTE_ON MIDI_STATUS_NOTE_ON MIDI_STATUS_NOTE_ON\n");
        if (synth->note != NULL)
        {
            // synth->note(synth->synth, true, ctx->payload[0], ctx->payload[1]);
        }
        break;
    case MIDI_STATUS_NOTE_OFF:
        // printf("MIDI_STATUS_NOTE_OFF MIDI_STATUS_NOTE_OFF MIDI_STATUS_NOTE_OFF\n");
        if (synth->note != NULL)
        {
            // synth->note(synth->synth, false, ctx->payload[0], ctx->payload[1]);
        }
        break;
    case MIDI_STATUS_CONTROLLER_CHANGE:
        // printf("MIDI_STATUS_CONTROLLER_CHANGE MIDI_STATUS_CONTROLLER_CHANGE MIDI_STATUS_CONTROLLER_CHANGE\n");
        if (synth->controller != NULL)
        {
            // synth->controller(synth->synth, ctx->payload[0], ctx->payload[1]);
        }
        break;
    case MIDI_STATUS_PITCH_BEND:
        // printf("MIDI_STATUS_PITCH_BEND MIDI_STATUS_PITCH_BEND MIDI_STATUS_PITCH_BEND\n");
        if (synth->pitch_bend != NULL)
        {
            // uint16_t temp = (ctx->payload[1] << 8) | ctx->payload[0];
            //  printf("midi pitch: b1: %03d b2: %03d val: %05d\n", ctx->payload[0], ctx->payload[1], temp);
            // synth->pitch_bend(synth->synth, temp);
        }
        break;
    default:
        break;
    }
}

void midi_input_device_process_payload(midi_input_device_t *ctx)
{
    if (ctx->synth_cb[MIDI_CHANNEL_ANY] != NULL)
    {
        midi_input_device_synth_send(ctx, ctx->synth_cb[MIDI_CHANNEL_ANY]);
    }
    for (uint8_t i = 0; i < 16; i++)
    {
        if (ctx->synth_cb[i] == NULL)
            continue;
        midi_input_device_synth_send(ctx, ctx->synth_cb[i]);
    }
}

void midi_input_device_read_payload(midi_input_device_t *ctx, uint8_t b)
{
    // printf("MIDI STATUS: %d\n", ctx->message.status);
    // printf("BEFORE midi state: %d midi status: %d index: %d size: %d\n", ctx->state, ctx->message.status, ctx->payload_index,
    // ctx->payload_size);
    // if (ctx->payload_size == 0)
    // {
    //     return;
    // }
    // ctx->payload[ctx->payload_index] = b;
    // ctx->payload_index++;
    // if (ctx->payload_index >= ctx->payload_size)
    // {
    //     // printf("midi state: %d midi status: %d\n", ctx->state, ctx->message.status);
    //     midi_input_device_process_payload(ctx);
    //     ctx->payload_index = 0;
    //     ctx->payload_size = 0;
    //     ctx->state = MIDI_INPUT_DEVICE_STATE_NEW_STATUS;
    //     // printf("AFTER midi state: %d midi status: %d index: %d size: %d\n", ctx->state, ctx->message.status,
    //     // ctx->payload_index, ctx->payload_size);
    //     return;
    // }
}

void midi_input_device_handle_new_message(midi_input_device_t *ctx, const uint8_t b)
{
    if (!(MIDI_NEW_MESSAGE_BYTE_MASK & b))
    {
        return;
    }
    (*(uint8_t *)&ctx->message) = b;
    ctx->state = MIDI_INPUT_DEVICE_STATE_NEW_STATUS;
}

void midi_input_device_handle_new_status(midi_input_device_t *ctx, const uint8_t b)
{
    switch (ctx->message.status)
    {
    case MIDI_STATUS_NOTE_OFF:
    case MIDI_STATUS_NOTE_ON:
    case MIDI_STATUS_KEY_PRESSURE:
    case MIDI_STATUS_CONTROLLER_CHANGE:
    case MIDI_STATUS_PITCH_BEND:
        buffer_set_size(&ctx->payload, 2);
        ctx->state = MIDI_INPUT_DEVICE_STATE_READ_PAYLOAD;
        break;
    case MIDI_STATUS_PROGRAM_CHANGE:
    case MIDI_STATUS_CHANNEL_PRESSURE:
        buffer_set_size(&ctx->payload, 1);
        ctx->state = MIDI_INPUT_DEVICE_STATE_READ_PAYLOAD;
        break;
    case MIDI_STATUS_SYSTEM:
        ctx->state = MIDI_INPUT_DEVICE_STATE_STATUS_SYSTEM;
        break;
    default:
        break;
    }
}

void midi_input_device_handle_system_status(midi_input_device_t *ctx, const uint8_t b)
{
    // handle system events
}

void midi_input_device_handle_unmarshal_status(midi_input_device_t *ctx)
{
    if (ctx->channel != ctx->message.subCmd)
    {
        // ignore not our channel
        return;
    }
    switch (ctx->message.status)
    {
    case MIDI_STATUS_NOTE_OFF:
    case MIDI_STATUS_NOTE_ON: {
        midi_note_t note;
        midi_note_unmarshal(&note, ctx->message, (uint8_t *)&ctx->payload.data, ctx->payload.size);
        ctx->synth_cb[0]->note(ctx->synth_cb[0]->synth, note);
        ctx->state = MIDI_INPUT_DEVICE_STATE_NEW_MESSAGE;
        break;
    }
    case MIDI_STATUS_KEY_PRESSURE:
    case MIDI_STATUS_CONTROLLER_CHANGE:
    case MIDI_STATUS_PITCH_BEND:
        // 2 bytes
        break;
    case MIDI_STATUS_PROGRAM_CHANGE:
    case MIDI_STATUS_CHANNEL_PRESSURE:
        // 1 byte
        break;
    case MIDI_STATUS_SYSTEM:
        ctx->state = MIDI_INPUT_DEVICE_STATE_STATUS_SYSTEM;
        break;
    default:
        break;
    }
}

void midi_input_device_handle_read_payload(midi_input_device_t *ctx, const uint8_t b)
{
    buffer_append_u8(&ctx->payload, b);
    if (buffer_space_left(&ctx->payload) == 0)
    {
        if (ctx->message.status != MIDI_STATUS_SYSTEM)
        {
            midi_input_device_handle_unmarshal_status(ctx);
        }
        ctx->state = MIDI_INPUT_DEVICE_STATE_READ_PAYLOAD_END;
        return;
    }
}

void midi_input_device_handle_read_payload_end(midi_input_device_t *ctx, const uint8_t b)
{
    if (ctx->message.status == MIDI_STATUS_SYSTEM)
    {
        // handle system event after collecting payload
    }
    else
    {
        // ctx->state
    }
}

void midi_input_device_feed(midi_input_device_t *ctx, const uint8_t b)
{
    switch (ctx->state)
    {
    case MIDI_INPUT_DEVICE_STATE_NEW_MESSAGE:
        midi_input_device_handle_new_message(ctx, b);
        break;
    case MIDI_INPUT_DEVICE_STATE_NEW_STATUS:
        midi_input_device_handle_new_status(ctx, b);
        break;
    case MIDI_INPUT_DEVICE_STATE_STATUS_SYSTEM:
        midi_input_device_handle_system_status(ctx, b);
        break;
    case MIDI_INPUT_DEVICE_STATE_READ_PAYLOAD:
        midi_input_device_handle_read_payload(ctx, b);
        break;
    case MIDI_INPUT_DEVICE_STATE_READ_PAYLOAD_END:
        midi_input_device_handle_read_payload_end(ctx, b);
        break;
    default:
        break;
    }
}

void midi_input_device_link_synth_to_channel(midi_input_device_t *ctx, uint8_t channel, midi_device_callback_data_t *cb_data)
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