#include "libmidi/midi_input_device.h"

#include <stdlib.h>

void handle_ready_to_new(midi_input_device_t* ctx, const uint8_t b);
void handle_predelay(midi_input_device_t* ctx, const uint8_t b);
void handle_new_message(midi_input_device_t* ctx, const uint8_t b);
void handle_read_payload(midi_input_device_t* ctx, const uint8_t b);
void handle_system_meta_event(midi_input_device_t* ctx, const uint8_t b);
void handle_system_meta_event_payload_size(midi_input_device_t* ctx, const uint8_t b);

void init_handlers(midi_input_device_t* ctx)
{
    ctx->handlers.arr[MIDI_INPUT_STATE_READY_TO_NEW] = (midi_cb_state_handler_f*)&handle_ready_to_new;
    ctx->handlers.arr[MIDI_INPUT_STATE_PREDELAY] = (midi_cb_state_handler_f*)&handle_predelay;
    ctx->handlers.arr[MIDI_INPUT_STATE_NEW_MESSAGE] = (midi_cb_state_handler_f*)&handle_new_message;
    ctx->handlers.arr[MIDI_INPUT_STATE_READ_PAYLOAD] = (midi_cb_state_handler_f*)&handle_read_payload;
    ctx->handlers.arr[MIDI_INPUT_STATE_SYSTEM_META] = (midi_cb_state_handler_f*)&handle_system_meta_event;
    ctx->handlers.arr[MIDI_INPUT_STATE_READ_META_PAYLOAD_SIZE] =
        (midi_cb_state_handler_f*)&handle_system_meta_event_payload_size;
}

void midi_input_device_reset(midi_input_device_t* ctx)
{
    ctx->state = MIDI_INPUT_STATE_READY_TO_NEW;
    ctx->event.message_meta = 0;
    ctx->state_data.meta_length = 0;
    midi_event_reset(&ctx->event);
    buffer_reset(&ctx->buffer);
    vlv_reset(&ctx->vlv);
}

midi_input_device_t* midi_input_device_new(const bool smf)
{
    midi_input_device_t* ctx = calloc(1, sizeof(midi_input_device_t));
    if (!ctx)
    {
        return NULL;
    }

    init_handlers(ctx);
    midi_input_device_reset(ctx);
    ctx->smf = smf;

    return ctx;
}

void midi_input_device_free(midi_input_device_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    free(ctx);
}

void midi_input_device_set_listener(midi_input_device_t* ctx, midi_device_callback_data_t* listener)
{
    if (!ctx || !listener)
    {
        return;
    }

    ctx->listener = listener;
}

void midi_input_device_remove_listener(midi_input_device_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    ctx->listener = NULL;
}

uint32_t midi_input_device_get_predelay(midi_input_device_t* ctx)
{
    if (!ctx)
    {
        return 0;
    }

    return ctx->event.predelay;
}

void notify_watchers_system(midi_input_device_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    switch (ctx->event.message_meta)
    {
        case MIDI_META_EVENT_SEQUENCE_NUMBER:
        {
            break;
        }

        case MIDI_META_EVENT_TEXT:
        case MIDI_META_EVENT_COPYRIGHT:
        case MIDI_META_EVENT_TRACK_NAME:
        case MIDI_META_EVENT_INSTRUMENT_NAME:
        case MIDI_META_EVENT_LYRIC_TEXT:
        case MIDI_META_EVENT_TEXT_MARKER:
        case MIDI_META_EVENT_CUE_POINT:
        case MIDI_META_EVENT_PROGRAM_PATCH_NAME:
        case MIDI_META_EVENT_DEVICE_PORT_NAME:
        {
            const int ret =
                midi_text_event_unmarshal(&ctx->event.meta.text, ctx->buffer.data, ctx->buffer.expected_size);

            if (!ctx->listener)
            {
                break;
            }

            if (MIDI_NOT_SUCCESS(ret))
            {
                if (!ctx->listener->error)
                {
                    break;
                }

                ctx->listener->error(ctx->listener->handle, ret);
            }

            break;
        }

        case MIDI_META_EVENT_MIDI_CHANNEL:
        {
            break;
        }

        case MIDI_META_EVENT_MIDI_PORT:
        {
            break;
        }

        case MIDI_META_EVENT_TRACK_END:
        {
            // no size, just read zero byte at the end
            break;
        }

        case MIDI_META_EVENT_M_LIVE_TAG:
        {
            break;
        }

        case MIDI_META_EVENT_TEMPO:
        {
            const int ret = midi_tempo_unmarshal(&ctx->event.meta.tempo, ctx->buffer.data, ctx->buffer.expected_size);

            if (!ctx->listener)
            {
                break;
            }

            if (MIDI_NOT_SUCCESS(ret))
            {
                if (!ctx->listener->error)
                {
                    break;
                }

                ctx->listener->error(ctx->listener->handle, ret);
            }

            if (ctx->listener->tempo)
            {
                ctx->listener->tempo(ctx->listener->handle, ctx->event.meta.tempo);
            }

            break;
        }

        case MIDI_META_EVENT_SMPTE_OFFSET:
        {
            break;
        }

        case MIDI_META_EVENT_TIME_SIGNATURE:
        {
            const int ret = midi_time_signature_unmarshal(
                &ctx->event.meta.time_signature, ctx->buffer.data, ctx->buffer.expected_size);

            if (!ctx->listener)
            {
                break;
            }

            if (MIDI_NOT_SUCCESS(ret))
            {
                if (!ctx->listener->error)
                {
                    break;
                }

                ctx->listener->error(ctx->listener->handle, ret);
            }

            break;
        }

        case MIDI_META_EVENT_KEY_SIGNATURE:
        {
            break;
        }

        case MIDI_META_EVENT_PROPRIETARY_EVENT:
        {
            break;
        }

        // return to avoid notifying ctx->listener->event
        default: return;
    }

    if (ctx->listener->event)
    {
        ctx->listener->event(ctx->listener->handle, &ctx->event);
    }
}

void notify_watchers(midi_input_device_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    switch (ctx->event.message.status)
    {
        case MIDI_STATUS_NOTE_OFF:
        case MIDI_STATUS_NOTE_ON:
        {
            const int ret = midi_note_unmarshal(
                &ctx->event.standard.note, ctx->event.message, ctx->buffer.data, ctx->buffer.expected_size);

            if (!ctx->listener)
            {
                break;
            }

            if (MIDI_NOT_SUCCESS(ret))
            {
                if (!ctx->listener->error)
                {
                    break;
                }

                ctx->listener->error(ctx->listener->handle, ret);
            }

            if (ctx->listener->note)
            {
                ctx->listener->note(ctx->listener->handle, ctx->event.standard.note);
            }

            break;
        }

        case MIDI_STATUS_KEY_PRESSURE:
        {
            const int ret = midi_key_pressure_unmarshal(
                &ctx->event.standard.key_pressure, ctx->event.message, ctx->buffer.data, ctx->buffer.expected_size);

            if (!ctx->listener)
            {
                break;
            }

            if (MIDI_NOT_SUCCESS(ret))
            {
                if (!ctx->listener->error)
                {
                    break;
                }

                ctx->listener->error(ctx->listener->handle, ret);
            }

            if (ctx->listener->key_pressure)
            {
                ctx->listener->key_pressure(ctx->listener->handle, ctx->event.standard.key_pressure);
            }

            break;
        }

        case MIDI_STATUS_CONTROLLER_CHANGE:
        {
            const int ret = midi_control_unmarshal(
                &ctx->event.standard.control, ctx->event.message, ctx->buffer.data, ctx->buffer.expected_size);

            if (!ctx->listener)
            {
                break;
            }

            if (MIDI_NOT_SUCCESS(ret))
            {
                if (!ctx->listener->error)
                {
                    break;
                }

                ctx->listener->error(ctx->listener->handle, ret);
            }

            if (ctx->listener->control)
            {
                ctx->listener->control(ctx->listener->handle, ctx->event.standard.control);
            }

            break;
        }

        case MIDI_STATUS_PROGRAM_CHANGE:
        {
            const int ret = midi_program_change_unmarshal(
                &ctx->event.standard.program_change, ctx->event.message, ctx->buffer.data, ctx->buffer.expected_size);

            if (!ctx->listener)
            {
                break;
            }

            if (MIDI_NOT_SUCCESS(ret))
            {
                if (!ctx->listener->error)
                {
                    break;
                }

                ctx->listener->error(ctx->listener->handle, ret);
            }

            if (ctx->listener->program_change)
            {
                ctx->listener->program_change(ctx->listener->handle, ctx->event.standard.program_change);
            }

            break;
        }

        case MIDI_STATUS_CHANNEL_PRESSURE:
        {
            const int ret = midi_channel_pressure_unmarshal(
                &ctx->event.standard.channel_pressure, ctx->event.message, ctx->buffer.data, ctx->buffer.expected_size);

            if (!ctx->listener)
            {
                break;
            }

            if (MIDI_NOT_SUCCESS(ret))
            {
                if (!ctx->listener->error)
                {
                    break;
                }

                ctx->listener->error(ctx->listener->handle, ret);
            }

            if (ctx->listener->channel_pressure)
            {
                ctx->listener->channel_pressure(ctx->listener->handle, ctx->event.standard.channel_pressure);
            }

            break;
        }

        case MIDI_STATUS_PITCH_BEND:
        {
            const int ret = midi_pitch_unmarshal(
                &ctx->event.standard.pitch, ctx->event.message, ctx->buffer.data, ctx->buffer.expected_size);

            if (!ctx->listener)
            {
                break;
            }

            if (MIDI_NOT_SUCCESS(ret))
            {
                if (!ctx->listener->error)
                {
                    break;
                }

                ctx->listener->error(ctx->listener->handle, ret);
            }

            if (ctx->listener->pitch)
            {
                ctx->listener->pitch(ctx->listener->handle, ctx->event.standard.pitch);
            }

            break;
        }

        case MIDI_STATUS_SYSTEM:
        {
            notify_watchers_system(ctx);
            // return to avoid notifying ctx->listener->event
            return;
        }

        case MIDI_STATUS_COUNT:
        default:
        {
            break;
        }
    }

    if (ctx->listener->event)
    {
        ctx->listener->event(ctx->listener->handle, &ctx->event);
    }
}

void handle_system_meta_event_payload_size(midi_input_device_t* ctx, const uint8_t b)
{
    if (!ctx)
    {
        return;
    }

    if (!vlv_feed(&ctx->vlv, b))
    {
        return;
    }

    // if vlv full switch to filling buffer with midi bytes
    ctx->state_data.meta_length = vlv_get_value(&ctx->vlv);
    if (ctx->state_data.meta_length != 0)
    {
        buffer_set_expected_size(&ctx->buffer, ctx->state_data.meta_length);
        ctx->state = MIDI_INPUT_STATE_READ_PAYLOAD;
        return;
    }

    notify_watchers(ctx);
    ctx->state = MIDI_INPUT_STATE_READY_TO_NEW;
}

void handle_system_meta_event(midi_input_device_t* ctx, const uint8_t b)
{
    if (!ctx)
    {
        return;
    }

    ctx->event.message_meta = b;

    switch (ctx->event.message_meta)
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
            ctx->state_data.meta_length = 0;
            vlv_reset(&ctx->vlv);
            buffer_reset(&ctx->buffer);
            break;

        default: break;
    }
}

void handle_system_status(midi_input_device_t* ctx, const uint8_t b)
{
    if (!ctx)
    {
        return;
    }

    switch (ctx->event.message.system)
    {
        case MIDI_STATUS_SYSTEM_EXCLUSIVE:
            // System Exclusive (data dump) 2nd byte= Vendor ID followed by more data bytes and ending with EOX (0x7F).
            break;

        case MIDI_STATUS_SYSTEM_SONG_POSITION:
            // 2 bytes payload
            buffer_set_expected_size(&ctx->buffer, 2);
            ctx->state = MIDI_INPUT_STATE_READ_PAYLOAD;
            break;

        case MIDI_STATUS_SYSTEM_SONG_SELECT:
            // 1 byte payload
            buffer_set_expected_size(&ctx->buffer, 1);
            ctx->state = MIDI_INPUT_STATE_READ_PAYLOAD;
            break;

        case MIDI_STATUS_SYSTEM_RESET_OR_META:
            // here feed meta event handler
            ctx->state = MIDI_INPUT_STATE_SYSTEM_META;
            break;

        default: break;
    }
}

void handle_new_status(midi_input_device_t* ctx, const uint8_t b)
{
    if (!ctx)
    {
        return;
    }

    switch (ctx->event.message.status)
    {
        case MIDI_STATUS_NOTE_OFF:
        case MIDI_STATUS_NOTE_ON:
        case MIDI_STATUS_KEY_PRESSURE:
        case MIDI_STATUS_CONTROLLER_CHANGE:
        case MIDI_STATUS_PITCH_BEND:
        {
            buffer_set_expected_size(&ctx->buffer, 2);
            ctx->state = MIDI_INPUT_STATE_READ_PAYLOAD;
            break;
        }

        case MIDI_STATUS_PROGRAM_CHANGE:
        case MIDI_STATUS_CHANNEL_PRESSURE:
        {
            buffer_set_expected_size(&ctx->buffer, 1);
            ctx->state = MIDI_INPUT_STATE_READ_PAYLOAD;
            break;
        }

        case MIDI_STATUS_SYSTEM:
        {
            handle_system_status(ctx, b);
            break;
        }

        default: break;
    }
}

void handle_predelay(midi_input_device_t* ctx, const uint8_t b)
{
    if (!ctx)
    {
        return;
    }

    if (!vlv_feed(&ctx->vlv, b))
    {
        return;
    }

    ctx->event.predelay = vlv_get_value(&ctx->vlv);
    ctx->state = MIDI_INPUT_STATE_NEW_MESSAGE;
}

void handle_new_message(midi_input_device_t* ctx, const uint8_t b)
{
    if (!ctx)
    {
        return;
    }

    ctx->event.message.raw = b;

    if (!ctx->event.message.new_msg)
    {
        ctx->event.message.raw = 0x00;
        if (ctx->listener->error)
        {
            ctx->listener->error(ctx->listener->handle, MIDI_ERROR_NOT_NEW_MESSAGE);
        }
        return;
    }

    handle_new_status(ctx, b);
}

void handle_ready_to_new(midi_input_device_t* ctx, const uint8_t b)
{
    if (!ctx)
    {
        return;
    }

    midi_event_reset(&ctx->event);

    if (ctx->smf)
    {
        ctx->state = MIDI_INPUT_STATE_PREDELAY;
        vlv_reset(&ctx->vlv);
        handle_predelay(ctx, b);
    }
    else
    {
        handle_new_message(ctx, b);
    }
}

void handle_read_payload(midi_input_device_t* ctx, const uint8_t b)
{
    if (!ctx)
    {
        return;
    }

    buffer_append_u8(&ctx->buffer, b);

    if (buffer_space_left(&ctx->buffer) == 0)
    {
        notify_watchers(ctx);
        ctx->state = MIDI_INPUT_STATE_READY_TO_NEW;
        return;
    }
}

void midi_input_device_feed(midi_input_device_t* ctx, const uint8_t b)
{
    if (!ctx)
    {
        return;
    }

    ctx->handlers.arr[ctx->state](ctx, b);
}

void midi_input_device_feed_chunk(midi_input_device_t* ctx, const uint8_t* data, const uint32_t size)
{
    if (!ctx)
    {
        return;
    }

    for (uint32_t i = 0; i < size; i++)
    {
        midi_input_device_feed(ctx, data[i]);
    }
}
