#include "input_state_machine.h"

#include <stdlib.h>

void init_handlers(input_state_machine_t *ctx)
{
    ctx->handlers.arr[MIDI_INPUT_STATE_NEW_MESSAGE] = (midi_cb_state_handler_f *)&handle_new_message;
    ctx->handlers.arr[MIDI_INPUT_STATE_NEW_STATUS] = (midi_cb_state_handler_f *)&handle_new_status;
    ctx->handlers.arr[MIDI_INPUT_STATE_READ_PAYLOAD] = (midi_cb_state_handler_f *)&handle_read_payload;
    ctx->handlers.arr[MIDI_INPUT_STATE_READ_PAYLOAD_END] = (midi_cb_state_handler_f *)&handle_read_payload_end;
    ctx->handlers.arr[MIDI_INPUT_STATE_STATUS_SYSTEM] = (midi_cb_state_handler_f *)&handle_system_status;
}

void input_state_machine_reset(input_state_machine_t *ctx)
{
    ctx->state = MIDI_INPUT_STATE_NEW_MESSAGE;
    ctx->message.status = 0;
    ctx->message.subCmd = 0;
    vlv_reset(&ctx->vlv);
    buffer_reset(&ctx->payload);
    init_handlers(ctx);
}

input_state_machine_t *input_state_machine_new(bool smf)
{
    input_state_machine_t *ctx = calloc(1, sizeof(input_state_machine_t));
    input_state_machine_reset(ctx);
    init_handlers(ctx);
    ctx->smf = smf;
    return ctx;
}

void input_state_machine_free(input_state_machine_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    free(ctx);
}

void handle_new_message(input_state_machine_t *ctx, const uint8_t b)
{
    if (!(MIDI_NEW_MESSAGE_BYTE_MASK & b))
    {
        return;
    }
    (*(uint8_t *)&ctx->message) = b;
    ctx->state = MIDI_INPUT_STATE_NEW_STATUS;
}

void handle_new_status(input_state_machine_t *ctx, const uint8_t b)
{
    switch (ctx->message.status)
    {
    case MIDI_STATUS_NOTE_OFF:
    case MIDI_STATUS_NOTE_ON:
    case MIDI_STATUS_KEY_PRESSURE:
    case MIDI_STATUS_CONTROLLER_CHANGE:
    case MIDI_STATUS_PITCH_BEND:
        buffer_set_size(&ctx->payload, 2);
        ctx->state = MIDI_INPUT_STATE_READ_PAYLOAD;
        break;
    case MIDI_STATUS_PROGRAM_CHANGE:
    case MIDI_STATUS_CHANNEL_PRESSURE:
        buffer_set_size(&ctx->payload, 1);
        ctx->state = MIDI_INPUT_STATE_READ_PAYLOAD;
        break;
    case MIDI_STATUS_SYSTEM:
        ctx->state = MIDI_INPUT_STATE_STATUS_SYSTEM;
        break;
    default:
        break;
    }
}

void handle_read_payload(input_state_machine_t *ctx, const uint8_t b)
{
    buffer_append_u8(&ctx->payload, b);
    if (buffer_space_left(&ctx->payload) == 0)
    {
        if (ctx->message.status != MIDI_STATUS_SYSTEM)
        {
            midi_input_device_handle_unmarshal_status(ctx);
        }
        ctx->state = MIDI_INPUT_STATE_READ_PAYLOAD_END;
        return;
    }
}

void handle_read_payload_end(input_state_machine_t *ctx, const uint8_t b)
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

void handle_system_status(input_state_machine_t *ctx, const uint8_t b)
{
    switch (ctx->message.subCmd)
    {
    case MIDI_STATUS_SYSTEM_EXCLUSIVE:
        // System Exclusive (data dump) 2nd byte= Vendor ID followed by more data bytes and ending with EOX (0x7F).
        break;
    case MIDI_STATUS_SYSTEM_SONG_POSITION:
        // 2 bytes payload
        buffer_set_size(&ctx->payload, 2);
        ctx->state = MIDI_INPUT_STATE_READ_PAYLOAD;
        break;
    case MIDI_STATUS_SYSTEM_SONG_SELECT:
        // 1 byte payload
        buffer_set_size(&ctx->payload, 1);
        ctx->state = MIDI_INPUT_STATE_READ_PAYLOAD;
        break;
    case MIDI_STATUS_SYSTEM_RESET_OR_META: {
        // here feed meta event handler
        // midi_event_meta_reset(&ctx->meta);
        // if (res = midi_event_meta_unmarshal(&ctx->meta, data + iterator, size), res < 0)
        // {
        //     return -1;
        // }
        // iterator += res;
        // size -= res;

        // if (ctx->meta.length.val > 0)
        // {
        //     alloc_and_copy_from_stream(&ctx->data, data, ctx->meta.length.val, &iterator);
        //     size -= ctx->meta.length.val;
        // }
    }
    break;

    default:
        break;
    }
}

void input_state_machine_feed(input_state_machine_t *ctx, const uint8_t b)
{
    ctx->handlers.arr[ctx->state](ctx, b);
}
