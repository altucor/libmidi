#include "libmidi/midi_output_device.h"
#include "libmidi/protocol.h"
#include "libmidi/event.h"

#include <stdlib.h>

midi_output_device_t* midi_output_device_new(const bool smf)
{
    midi_output_device_t* ctx = calloc(1, sizeof(midi_output_device_t));
    if (!ctx)
    {
        return NULL;
    }

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

int midi_output_device_send(midi_output_device_t* ctx, const midi_event_t* event, uint8_t* data, const uint32_t size)
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
        if (ret = vlv_encode(&ctx->vlv, data + iterator, size - iterator), ret < 0)
        {
            return ret;
        }

        iterator += ret;
        ret = 0;
    }

    switch (event->message.status)
    {
        case MIDI_STATUS_NOTE_OFF:
        case MIDI_STATUS_NOTE_ON:
        {
            ret = midi_note_marshal(&event->standard.note, data + iterator, size - iterator);
            break;
        }

        case MIDI_STATUS_KEY_PRESSURE:
        {
            ret = midi_key_pressure_marshal(&event->standard.key_pressure, data + iterator, size - iterator);
            break;
        }

        case MIDI_STATUS_CONTROLLER_CHANGE:
        {
            ret = midi_control_marshal(&event->standard.control, data + iterator, size - iterator);
            break;
        }

        case MIDI_STATUS_PROGRAM_CHANGE:
        {
            ret = midi_program_change_marshal(&event->standard.program_change, data + iterator, size - iterator);
            break;
        }

        case MIDI_STATUS_CHANNEL_PRESSURE:
        {
            ret = midi_channel_pressure_marshal(&event->standard.channel_pressure, data + iterator, size - iterator);
            break;
        }

        case MIDI_STATUS_PITCH_BEND:
        {
            ret = midi_pitch_marshal(&event->standard.pitch, data + iterator, size - iterator);
            break;
        }

        case MIDI_STATUS_SYSTEM:
        {
            ret = -1;
            // ret = midi_pitch_marshal(&event->standard.pitch, data + iterator, size - iterator);
            break;
        }

        default: break;
    }

    if (ret < 0)
    {
        return ret;
    }

    iterator += ret;

    return iterator;
}
