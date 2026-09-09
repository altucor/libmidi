#include "libmidi/midi_output_device.h"
#include "libmidi/protocol.h"
#include "libmidi/event.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

midi_output_device_t* midi_output_device_new(const bool smf, void* user_ctx, midi_cb_user_send_f* user_cb)
{
    if (!user_cb)
    {
        return NULL;
    }

    midi_output_device_t* ctx = calloc(1, sizeof(midi_output_device_t));
    if (!ctx)
    {
        return NULL;
    }

    ctx->smf = smf;
    ctx->user_ctx = user_ctx;
    ctx->user_cb = user_cb;

    return ctx;
}

void midi_output_device_free(midi_output_device_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    free(ctx);
}

static int handle_meta(const midi_event_t* event, uint8_t* buffer, const uint32_t size)
{
    int ret = 0;
    // uint32_t iterator = 0;

    switch (event->message_meta)
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
        case MIDI_META_EVENT_MIDI_CHANNEL_PREFIX:
        case MIDI_META_EVENT_MIDI_PORT:
        case MIDI_META_EVENT_TRACK_END:
        case MIDI_META_EVENT_M_LIVE_TAG:
        case MIDI_META_EVENT_TEMPO:
        case MIDI_META_EVENT_SMPTE_OFFSET:
        case MIDI_META_EVENT_TIME_SIGNATURE:
        case MIDI_META_EVENT_KEY_SIGNATURE:
        case MIDI_META_EVENT_PROPRIETARY_EVENT:
        {
            ret = -1;
            break;
        }
    }

    return ret;
}

static int handle_system(const midi_event_t* event, uint8_t* buffer, const uint32_t size)
{
    int ret = 0;
    uint32_t iterator = 0;

    switch (event->message.system)
    {
        case MIDI_STATUS_SYSTEM_COMMON_SYSEX_START:
        {
            memcpy(buffer + iterator, &event->system.sysex.vendor, sizeof(event->system.sysex.vendor));
            iterator += sizeof(event->system.sysex.vendor);

            memcpy(buffer + iterator, event->system.sysex.data, event->system.sysex.size);
            iterator += event->system.sysex.size;

            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_MTC_QUARTER_FRAME:
        {
            // 0xF1 message
            midi_cmd_t temp = {0};
            temp.new_msg = true;
            temp.status = MIDI_STATUS_SYSTEM;
            temp.system = MIDI_STATUS_SYSTEM_COMMON_MTC_QUARTER_FRAME;
            buffer[iterator++] = temp.raw;

            memcpy(buffer + iterator, &event->system.mtc_quarter_frame, sizeof(event->system.mtc_quarter_frame.value));
            iterator += sizeof(event->system.mtc_quarter_frame.value);
            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_SONG_POSITION:
        {
            memcpy(buffer + iterator, &event->system.song_position, sizeof(event->system.song_position));
            iterator += sizeof(event->system.song_position);
            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_SONG_SELECT:
        {
            memcpy(buffer + iterator, &event->system.song_select, sizeof(event->system.song_select));
            iterator += sizeof(event->system.song_select);
            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_TUNE_REQUEST:
        case MIDI_STATUS_SYSTEM_REALTIME_TIMING_TICK:
        case MIDI_STATUS_SYSTEM_REALTIME_SONG_START:
        case MIDI_STATUS_SYSTEM_REALTIME_SONG_CONTINUE:
        case MIDI_STATUS_SYSTEM_REALTIME_SONG_STOP:
        case MIDI_STATUS_SYSTEM_REALTIME_ACTIVE_SENSING:
        {
            memcpy(buffer + iterator, &event->message.raw, sizeof(event->message.raw));
            iterator += sizeof(event->message.raw);
            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_META:
        {
            ret = handle_meta(event, buffer + iterator, size - iterator);
            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_UNOFFICIAL_BUS_SELECT:
        case MIDI_STATUS_SYSTEM_COMMON_RESERVED_4:
        case MIDI_STATUS_SYSTEM_COMMON_SYSEX_END:
        case MIDI_STATUS_SYSTEM_REALTIME_RESERVED_9:
        case MIDI_STATUS_SYSTEM_REALTIME_RESERVED_13:
        default:
        {
            // Unexpected case
            ret = -1;
            break;
        }
    }

    if (ret < 0)
    {
        return ret;
    }

    return iterator;
}

static int handle_standard(const midi_event_t* event, uint8_t* buffer, const uint32_t size)
{
    int ret = 0;
    uint32_t iterator = 0;

    switch (event->message.status)
    {
        case MIDI_STATUS_NOTE_OFF:
        case MIDI_STATUS_NOTE_ON:
        {
            ret = midi_note_marshal(&event->standard.note, buffer + iterator, size);
            break;
        }

        case MIDI_STATUS_KEY_PRESSURE:
        {
            ret = midi_key_pressure_marshal(&event->standard.key_pressure, buffer + iterator, size);
            break;
        }

        case MIDI_STATUS_CONTROLLER_CHANGE:
        {
            ret = midi_control_marshal(&event->standard.control, buffer + iterator, size);
            break;
        }

        case MIDI_STATUS_PROGRAM_CHANGE:
        {
            ret = midi_program_change_marshal(&event->standard.program_change, buffer + iterator, size);
            break;
        }

        case MIDI_STATUS_CHANNEL_PRESSURE:
        {
            ret = midi_channel_pressure_marshal(&event->standard.channel_pressure, buffer + iterator, size);
            break;
        }

        case MIDI_STATUS_PITCH_BEND:
        {
            ret = midi_pitch_marshal(&event->standard.pitch, buffer + iterator, size);
            break;
        }

        case MIDI_STATUS_SYSTEM:
        {
            ret = handle_system(event, buffer + iterator, size - iterator);
            break;
        }

        default:
        {
            // Unexpected case
            ret = -1;
            break;
        }
    }

    return ret;
}

int midi_output_device_send(midi_output_device_t* ctx, const midi_event_t* event)
{
    if (!ctx || !event || !event->message.new_msg)
    {
        return -1;
    }

    int ret = 0;
    uint32_t iterator = 0;

    if (ctx->smf)
    {
        vlv_set_value(&ctx->vlv, event->predelay);
        if (ret = vlv_encode(&ctx->vlv, ctx->buffer + iterator, MIDI_OUTPUT_DEVICE_BUFFER_MAX - iterator), ret < 0)
        {
            return ret;
        }

        iterator += ret;
        ret = 0;
    }

    ret = handle_standard(event, ctx->buffer + iterator, MIDI_OUTPUT_DEVICE_BUFFER_MAX - iterator);
    if (ret < 0)
    {
        return ret;
    }

    iterator += ret;
    ret = 0;

    ret = ctx->user_cb(ctx->user_ctx, ctx->buffer, iterator);

    return ret;
}
