#include "libmidi/midi_output_device.h"

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
        ret = vlv_encode(&ctx->vlv, data + iterator, size - iterator);
        if (ret < 0)
        {
            //
        }
    }

    switch (event->message.status)
    {
        case MIDI_STATUS_NOTE_OFF:
        case MIDI_STATUS_NOTE_ON:
        {
            midi_note_marshal(&event->standard.note, data, size);
            break;
        }

        default: break;
    }

    return 0;
}
