#include "midi_input_device.h"

#include <stdio.h>
#include <stdlib.h>

void handle_ready_to_new(midi_input_device_t *ctx, const uint8_t b);
void handle_predelay(midi_input_device_t *ctx, const uint8_t b);
void handle_new_message(midi_input_device_t *ctx, const uint8_t b);
void handle_read_payload(midi_input_device_t *ctx, const uint8_t b);
void handle_system_meta_event(midi_input_device_t *ctx, const uint8_t b);
void handle_system_meta_event_payload_size(midi_input_device_t *ctx, const uint8_t b);

void init_handlers(midi_input_device_t *ctx)
{
    ctx->handlers.arr[MIDI_INPUT_STATE_READY_TO_NEW] = (midi_cb_state_handler_f *)&handle_ready_to_new;
    ctx->handlers.arr[MIDI_INPUT_STATE_PREDELAY] = (midi_cb_state_handler_f *)&handle_predelay;
    ctx->handlers.arr[MIDI_INPUT_STATE_NEW_MESSAGE] = (midi_cb_state_handler_f *)&handle_new_message;
    ctx->handlers.arr[MIDI_INPUT_STATE_READ_PAYLOAD] = (midi_cb_state_handler_f *)&handle_read_payload;
    ctx->handlers.arr[MIDI_INPUT_STATE_SYSTEM_META] = (midi_cb_state_handler_f *)&handle_system_meta_event;
    ctx->handlers.arr[MIDI_INPUT_STATE_READ_META_PAYLOAD_SIZE] = (midi_cb_state_handler_f *)&handle_system_meta_event_payload_size;
}

void midi_input_device_reset(midi_input_device_t *ctx)
{
    ctx->state = MIDI_INPUT_STATE_READY_TO_NEW;
    midi_event_smf_reset(&ctx->event_smf);
    buffer_reset(ctx->payload);
}

midi_input_device_t *midi_input_device_new(bool smf)
{
    midi_input_device_t *ctx = calloc(1, sizeof(midi_input_device_t));
    ctx->payload = buffer_new();
    init_handlers(ctx);
    midi_input_device_reset(ctx);
    ctx->smf = smf;
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

void midi_input_device_set_listener(midi_input_device_t *ctx, midi_device_callback_data_t *listener)
{
    ctx->listener = listener;
}

uint32_t midi_input_device_get_predelay(midi_input_device_t *ctx)
{
    return ctx->event_smf.predelay.val;
}

void notify_watchers_system(midi_input_device_t *ctx)
{
    switch (ctx->event_smf.message_meta)
    {
    case MIDI_META_EVENT_SEQUENCE_NUMBER:
        break;
    case MIDI_META_EVENT_TEXT:
    case MIDI_META_EVENT_COPYRIGHT:
    case MIDI_META_EVENT_TRACK_NAME:
    case MIDI_META_EVENT_INSTRUMENT_NAME:
    case MIDI_META_EVENT_LYRIC_TEXT:
    case MIDI_META_EVENT_TEXT_MARKER:
    case MIDI_META_EVENT_CUE_POINT:
    case MIDI_META_EVENT_PROGRAM_PATCH_NAME:
    case MIDI_META_EVENT_DEVICE_PORT_NAME:
        midi_text_event_unmarshal(&ctx->event_smf.event.meta.text, ctx->payload->data, ctx->payload->size);
        break;
    case MIDI_META_EVENT_MIDI_CHANNEL:
        break;
    case MIDI_META_EVENT_MIDI_PORT:
        break;
    case MIDI_META_EVENT_TRACK_END:
        // no size, just read zero byte at the end
        break;
    case MIDI_META_EVENT_M_LIVE_TAG:
        break;
    case MIDI_META_EVENT_TEMPO: {
        midi_tempo_unmarshal(&ctx->event_smf.event.meta.tempo, ctx->payload->data, ctx->payload->size);
        if (ctx->listener->tempo != NULL)
        {
            ctx->listener->tempo(ctx->listener->handle, ctx->event_smf.event.meta.tempo);
        }
        break;
    }
    case MIDI_META_EVENT_SMPTE_OFFSET:
        break;
    case MIDI_META_EVENT_TIME_SIGNATURE:
        midi_time_signature_unmarshal(&ctx->event_smf.event.meta.time_signature, ctx->payload->data, ctx->payload->size);
        break;
    case MIDI_META_EVENT_KEY_SIGNATURE:
        break;
    case MIDI_META_EVENT_PROPRIETARY_EVENT:
        break;
    default:
        break;
    }
    if (ctx->listener->event != NULL)
    {
        ctx->listener->event(ctx->listener->handle, ctx->event_smf.message, ctx->event_smf.message_meta, ctx->event_smf.event);
    }
}

void notify_watchers(midi_input_device_t *ctx)
{
    if (ctx->listener == NULL)
    {
        return;
    }
    switch (ctx->event_smf.message.status)
    {
    case MIDI_STATUS_NOTE_OFF:
    case MIDI_STATUS_NOTE_ON: {
        midi_note_unmarshal(&ctx->event_smf.event.note, ctx->event_smf.message, ctx->payload->data, ctx->payload->size);
        if (ctx->listener->note != NULL)
        {
            ctx->listener->note(ctx->listener->handle, ctx->event_smf.event.note);
        }
        break;
    }
    case MIDI_STATUS_KEY_PRESSURE: {
        midi_key_pressure_unmarshal(&ctx->event_smf.event.key_pressure, ctx->event_smf.message, ctx->payload->data, ctx->payload->size);
        if (ctx->listener->key_pressure != NULL)
        {
            ctx->listener->key_pressure(ctx->listener->handle, ctx->event_smf.event.key_pressure);
        }
        break;
    }
    case MIDI_STATUS_CONTROLLER_CHANGE: {
        midi_control_unmarshal(&ctx->event_smf.event.control, ctx->event_smf.message, ctx->payload->data, ctx->payload->size);
        if (ctx->listener->control != NULL)
        {
            ctx->listener->control(ctx->listener->handle, ctx->event_smf.event.control);
        }
        break;
    }
    case MIDI_STATUS_PITCH_BEND: {
        midi_pitch_unmarshal(&ctx->event_smf.event.pitch, ctx->event_smf.message, ctx->payload->data, ctx->payload->size);
        if (ctx->listener->pitch != NULL)
        {
            ctx->listener->pitch(ctx->listener->handle, ctx->event_smf.event.pitch);
        }
        break;
    }
    case MIDI_STATUS_PROGRAM_CHANGE: {
        midi_program_change_unmarshal(&ctx->event_smf.event.program_change, ctx->event_smf.message, ctx->payload->data, ctx->payload->size);
        if (ctx->listener->program_change != NULL)
        {
            ctx->listener->program_change(ctx->listener->handle, ctx->event_smf.event.program_change);
        }
        break;
    }
    case MIDI_STATUS_CHANNEL_PRESSURE: {
        midi_channel_pressure_unmarshal(&ctx->event_smf.event.channel_pressure, ctx->event_smf.message, ctx->payload->data, ctx->payload->size);
        if (ctx->listener->channel_pressure != NULL)
        {
            ctx->listener->channel_pressure(ctx->listener->handle, ctx->event_smf.event.channel_pressure);
        }
        break;
    }
    case MIDI_STATUS_SYSTEM: {
        notify_watchers_system(ctx);
        return; // return to avoid notifying ctx->listener->event
    }
    }
    if (ctx->listener->event != NULL)
    {
        ctx->listener->event(ctx->listener->handle, ctx->event_smf.message, ctx->event_smf.message_meta, ctx->event_smf.event);
    }
}

void handle_system_meta_event_payload_size(midi_input_device_t *ctx, const uint8_t b)
{
    if (vlv_feed(&ctx->event_smf.meta_length, b))
    {
        // if full switch to filling buffer
        buffer_realloc(ctx->payload, ctx->event_smf.meta_length.val);
        if (ctx->event_smf.meta_length.val != 0)
        {
            ctx->state = MIDI_INPUT_STATE_READ_PAYLOAD;
            return;
        }
        else
        {
            notify_watchers(ctx);
            ctx->state = MIDI_INPUT_STATE_READY_TO_NEW;
            return;
        }
    }
}

void handle_system_meta_event(midi_input_device_t *ctx, const uint8_t b)
{
    ctx->event_smf.message_meta = b;
    switch (ctx->event_smf.message_meta)
    {
    case MIDI_META_EVENT_SEQUENCE_NUMBER:
    case MIDI_META_EVENT_TEXT:
    case MIDI_META_EVENT_COPYRIGHT:
    case MIDI_META_EVENT_TRACK_NAME:
    case MIDI_META_EVENT_INSTRUMENT_NAME:
    case MIDI_META_EVENT_LYRIC_TEXT:
    case MIDI_META_EVENT_TEXT_MARKER:
    case MIDI_META_EVENT_CUE_POINT:
    case MIDI_META_EVENT_PROGRAM_PATCH_NAME:
    case MIDI_META_EVENT_DEVICE_PORT_NAME:
    case MIDI_META_EVENT_MIDI_CHANNEL:
    case MIDI_META_EVENT_MIDI_PORT:
    case MIDI_META_EVENT_TRACK_END:
    case MIDI_META_EVENT_M_LIVE_TAG:
    case MIDI_META_EVENT_TEMPO:
    case MIDI_META_EVENT_SMPTE_OFFSET:
    case MIDI_META_EVENT_TIME_SIGNATURE:
    case MIDI_META_EVENT_KEY_SIGNATURE:
    case MIDI_META_EVENT_PROPRIETARY_EVENT:
        ctx->state = MIDI_INPUT_STATE_READ_META_PAYLOAD_SIZE;
        vlv_reset(&ctx->event_smf.meta_length);
        buffer_reset(ctx->payload);
        break;
    default:
        break;
    }
}

void handle_system_status(midi_input_device_t *ctx, const uint8_t b)
{
    switch (ctx->event_smf.message.system)
    {
    case MIDI_STATUS_SYSTEM_EXCLUSIVE:
        // System Exclusive (data dump) 2nd byte= Vendor ID followed by more data bytes and ending with EOX (0x7F).
        break;
    case MIDI_STATUS_SYSTEM_SONG_POSITION:
        // 2 bytes payload
        buffer_realloc(ctx->payload, 2);
        ctx->state = MIDI_INPUT_STATE_READ_PAYLOAD;
        break;
    case MIDI_STATUS_SYSTEM_SONG_SELECT:
        // 1 byte payload
        buffer_realloc(ctx->payload, 1);
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

void handle_new_status(midi_input_device_t *ctx, const uint8_t b)
{
    switch (ctx->event_smf.message.status)
    {
    case MIDI_STATUS_NOTE_OFF:
    case MIDI_STATUS_NOTE_ON:
    case MIDI_STATUS_KEY_PRESSURE:
    case MIDI_STATUS_CONTROLLER_CHANGE:
    case MIDI_STATUS_PITCH_BEND:
        buffer_realloc(ctx->payload, 2);
        ctx->state = MIDI_INPUT_STATE_READ_PAYLOAD;
        break;
    case MIDI_STATUS_PROGRAM_CHANGE:
    case MIDI_STATUS_CHANNEL_PRESSURE:
        buffer_realloc(ctx->payload, 1);
        ctx->state = MIDI_INPUT_STATE_READ_PAYLOAD;
        break;
    case MIDI_STATUS_SYSTEM:
        handle_system_status(ctx, b);
        break;
    default:
        break;
    }
}

void handle_predelay(midi_input_device_t *ctx, const uint8_t b)
{
    if (vlv_feed(&ctx->event_smf.predelay, b))
    {
        // if full switch to filling buffer
        ctx->state = MIDI_INPUT_STATE_NEW_MESSAGE;
    }
}

void handle_new_message(midi_input_device_t *ctx, const uint8_t b)
{
    ctx->event_smf.message.raw = b;
    if (!ctx->event_smf.message.new_msg)
    {
        ctx->event_smf.message.raw = 0x00;
        if (ctx->listener->error)
        {
            ctx->listener->error(ctx->listener->handle, MIDI_ERROR_NOT_NEW_MESSAGE);
        }
        return;
    }
    handle_new_status(ctx, b);
}

void handle_ready_to_new(midi_input_device_t *ctx, const uint8_t b)
{
    midi_event_smf_reset(&ctx->event_smf);

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

void handle_read_payload(midi_input_device_t *ctx, const uint8_t b)
{
    buffer_append_u8(ctx->payload, b);
    if (buffer_space_left(ctx->payload) == 0)
    {
        notify_watchers(ctx);
        ctx->state = MIDI_INPUT_STATE_READY_TO_NEW;
        return;
    }
}

void midi_input_device_feed(midi_input_device_t *ctx, const uint8_t b)
{
    ctx->handlers.arr[ctx->state](ctx, b);
}
