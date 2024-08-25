#include "input_state_machine.h"

#include <stdlib.h>

void handle_ready_to_new(input_state_machine_t *ctx, const uint8_t b);
void handle_new_message(input_state_machine_t *ctx, const uint8_t b);
void handle_read_payload(input_state_machine_t *ctx, const uint8_t b);
void handle_read_payload_end(input_state_machine_t *ctx, const uint8_t b);
void handle_system_meta_event(input_state_machine_t *ctx, const uint8_t b);
void handle_system_meta_event_payload_size(input_state_machine_t *ctx, const uint8_t b);

void init_handlers(input_state_machine_t *ctx)
{
    ctx->handlers.arr[MIDI_INPUT_STATE_READY_TO_NEW] = (midi_cb_state_handler_f *)&handle_ready_to_new;
    ctx->handlers.arr[MIDI_INPUT_STATE_NEW_MESSAGE] = (midi_cb_state_handler_f *)&handle_new_message;
    ctx->handlers.arr[MIDI_INPUT_STATE_READ_PAYLOAD] = (midi_cb_state_handler_f *)&handle_read_payload;
    ctx->handlers.arr[MIDI_INPUT_STATE_READ_PAYLOAD_END] = (midi_cb_state_handler_f *)&handle_read_payload_end;
    ctx->handlers.arr[MIDI_INPUT_STATE_SYSTEM_META] = (midi_cb_state_handler_f *)&handle_system_meta_event;
    ctx->handlers.arr[MIDI_INPUT_STATE_READ_META_PAYLOAD_SIZE] =
        (midi_cb_state_handler_f *)&handle_system_meta_event_payload_size;
}

void input_state_machine_reset(input_state_machine_t *ctx)
{
    ctx->state = MIDI_INPUT_STATE_READY_TO_NEW;
    ctx->message.status = 0;
    ctx->message.subCmd = 0;
    vlv_reset(&ctx->predelay);
    buffer_reset(&ctx->payload);
}

input_state_machine_t *input_state_machine_new(bool smf)
{
    input_state_machine_t *ctx = calloc(1, sizeof(input_state_machine_t));
    init_handlers(ctx);
    input_state_machine_reset(ctx);
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

uint32_t input_state_machine_get_predelay(input_state_machine_t *ctx)
{
    return ctx->predelay.val;
}

void notify_watchers_system(input_state_machine_t *ctx)
{
    switch (ctx->event.meta.type)
    {
    case MIDI_META_EVENT_SEQUENCE_NUMBER:
        break;
    case MIDI_META_EVENT_TEXT:
        break;
    case MIDI_META_EVENT_COPYRIGHT:
        break;
    case MIDI_META_EVENT_TRACK_NAME:
        break;
    case MIDI_META_EVENT_INSTRUMENT_NAME:
        break;
    case MIDI_META_EVENT_LYRIC_TEXT:
        break;
    case MIDI_META_EVENT_TEXT_MARKER:
        break;
    case MIDI_META_EVENT_CUE_POINT:
        break;
    case MIDI_META_EVENT_PROGRAM_PATCH_NAME:
        break;
    case MIDI_META_EVENT_DEVICE_PORT_NAME:
        break;
    case MIDI_META_EVENT_MIDI_CHANNEL:
        break;
    case MIDI_META_EVENT_MIDI_PORT:
        break;
    case MIDI_META_EVENT_TRACK_END:
        // no size, just read zero byte at the end
        break;
    case MIDI_META_EVENT_TEMPO:
        // midi_tempo_unmarshal();
        break;
    case MIDI_META_EVENT_SMPTE_OFFSET:
        break;
    case MIDI_META_EVENT_TIME_SIGNATURE:
        break;
    case MIDI_META_EVENT_KEY_SIGNATURE:
        break;
    case MIDI_META_EVENT_PROPRIETARY_EVENT:
        break;
    default:
        break;
    }
}

void notify_watchers(input_state_machine_t *ctx)
{
    if (ctx->listener.handle == NULL)
    {
        return;
    }
    if (ctx->message.status == MIDI_STATUS_SYSTEM)
    {
        notify_watchers_system(ctx);
        return;
    }
    switch (ctx->message.status)
    {
    case MIDI_STATUS_NOTE_OFF:
    case MIDI_STATUS_NOTE_ON:
        ctx->listener.note(ctx->listener.handle, ctx->event.note);
        break;
    case MIDI_STATUS_KEY_PRESSURE:
        break;
    case MIDI_STATUS_CONTROLLER_CHANGE:
        ctx->listener.control(ctx->listener.handle, ctx->event.control);
        break;
    case MIDI_STATUS_PITCH_BEND:
        ctx->listener.pitch(ctx->listener.handle, ctx->event.pitch);
        break;
    case MIDI_STATUS_PROGRAM_CHANGE:
        break;
    case MIDI_STATUS_CHANNEL_PRESSURE:
        break;
    }
}

void handle_system_meta_event_payload_size(input_state_machine_t *ctx, const uint8_t b)
{
    if (vlv_feed(&ctx->event.meta.length, b))
    {
        // if full switch to filling bugger
        buffer_set_size(&ctx->payload, ctx->event.meta.length.val);
        ctx->state = MIDI_INPUT_STATE_READ_PAYLOAD;
    }
}

void handle_system_meta_event(input_state_machine_t *ctx, const uint8_t b)
{
    ctx->event.meta.type = b;
    switch (ctx->event.meta.type)
    {
    case MIDI_META_EVENT_SEQUENCE_NUMBER:
        break;
    case MIDI_META_EVENT_TEXT:
        break;
    case MIDI_META_EVENT_COPYRIGHT:
        break;
    case MIDI_META_EVENT_TRACK_NAME:
        break;
    case MIDI_META_EVENT_INSTRUMENT_NAME:
        break;
    case MIDI_META_EVENT_LYRIC_TEXT:
        break;
    case MIDI_META_EVENT_TEXT_MARKER:
        break;
    case MIDI_META_EVENT_CUE_POINT:
        break;
    case MIDI_META_EVENT_PROGRAM_PATCH_NAME:
        break;
    case MIDI_META_EVENT_DEVICE_PORT_NAME:
        break;
    case MIDI_META_EVENT_MIDI_CHANNEL:
        break;
    case MIDI_META_EVENT_MIDI_PORT:
        break;
    case MIDI_META_EVENT_TRACK_END:
        // no size, just read zero byte at the end
        break;
    case MIDI_META_EVENT_TEMPO:
        ctx->state = MIDI_INPUT_STATE_READ_META_PAYLOAD_SIZE;
        break;
    case MIDI_META_EVENT_SMPTE_OFFSET:
        break;
    case MIDI_META_EVENT_TIME_SIGNATURE:
        break;
    case MIDI_META_EVENT_KEY_SIGNATURE:
        break;
    case MIDI_META_EVENT_PROPRIETARY_EVENT:
        break;
    default:
        break;
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
    case MIDI_STATUS_SYSTEM_RESET_OR_META:
        // here feed meta event handler
        ctx->state = MIDI_INPUT_STATE_SYSTEM_META;
        break;
    default:
        break;
    }
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
        handle_system_status(ctx, b);
        break;
    default:
        break;
    }
}

void handle_predelay(input_state_machine_t *ctx, const uint8_t b)
{
    if (vlv_feed(&ctx->predelay, b))
    {
        // if full switch to filling bugger
        ctx->state = MIDI_INPUT_STATE_NEW_MESSAGE;
    }
}

void handle_new_message(input_state_machine_t *ctx, const uint8_t b)
{
    if (!(MIDI_NEW_MESSAGE_BYTE_MASK & b))
    {
        return;
    }
    (*(uint8_t *)&ctx->message) = b;
    ctx->message.status &= ~MIDI_NEW_MESSAGE_4BIT_MASK;
    handle_new_status(ctx, b);
}

void handle_ready_to_new(input_state_machine_t *ctx, const uint8_t b)
{
    if (ctx->smf)
    {
        ctx->state = MIDI_INPUT_STATE_PREDELAY;
        handle_predelay(ctx, b);
        return;
    }
    else
    {
        handle_new_message(ctx, b);
    }
}

void handle_read_payload(input_state_machine_t *ctx, const uint8_t b)
{
    buffer_append_u8(&ctx->payload, b);
    if (buffer_space_left(&ctx->payload) == 0)
    {
        notify_watchers(ctx);
        ctx->state = MIDI_INPUT_STATE_READY_TO_NEW;
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

void input_state_machine_feed(input_state_machine_t *ctx, const uint8_t b)
{
    ctx->handlers.arr[ctx->state](ctx, b);
    if (ctx->listener.handle == NULL)
    {
        return;
    }
    // switch (ctx->message.status)
    // {
    // case MIDI_STATUS_NOTE_OFF:
    // case MIDI_STATUS_NOTE_ON:
    //     ctx->listener.note(ctx->listener.handle, ctx->event.note);
    //     break;
    // case MIDI_STATUS_KEY_PRESSURE:
    // case MIDI_STATUS_CONTROLLER_CHANGE:
    //     ctx->listener.control(ctx->listener.handle, ctx->event.control);
    //     break;
    // case MIDI_STATUS_PITCH_BEND:
    //     ctx->listener.pitch(ctx->listener.handle, ctx->event.pitch);
    //     break;
    // case MIDI_STATUS_PROGRAM_CHANGE:
    // case MIDI_STATUS_CHANNEL_PRESSURE:
    //     buffer_set_size(&ctx->payload, 1);
    //     ctx->state = MIDI_INPUT_STATE_READ_PAYLOAD;
    //     break;
    // case MIDI_STATUS_SYSTEM:
    //     // ctx->state = MIDI_INPUT_STATE_STATUS_SYSTEM;
    //     break;
    // default:
    //     break;
    // }
}
