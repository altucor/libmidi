#include "event.h"

#include "events/meta.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

midi_event_t *midi_event_new()
{
    midi_event_t *ctx = calloc(1, sizeof(midi_event_t));
    vlv_reset(&ctx->predelay);
    ctx->size = 0;
    ctx->data = NULL;
    return ctx;
}

void midi_event_free(midi_event_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }
    if (ctx->data != NULL)
    {
        free(ctx->data);
        ctx->data = NULL;
        ctx->size = 0;
    }
    free(ctx);
}

int midi_event_unmarshal_system(midi_event_t *ctx, uint8_t *data, uint32_t size)
{
    int res = 0;
    uint32_t iterator = 0;
    switch (ctx->cmd.subCmd)
    {
    case MIDI_STATUS_SYSTEM_EXCLUSIVE:
        // System Exclusive (data dump) 2nd byte= Vendor ID followed by more data bytes and ending with EOX (0x7F).
        {
            uint8_t b = 0;
            b = data[iterator++];
            size--;
            while (!is_sysex_end_byte(b))
            {
                if (size == 0)
                {
                    return -1;
                }
                if (ctx->data == NULL)
                {
                    ctx->data = malloc(1);
                    ctx->size = 1;
                }
                else
                {
                    ctx->size++;
                    ctx->data = realloc(ctx->data, ctx->size);
                }
                ctx->data[ctx->size - 1] = b;
                b = data[iterator++];
                size--;
            }
        }
        break;
    case MIDI_STATUS_SYSTEM_SONG_POSITION:
        // 2 bytes payload
        alloc_and_copy_from_stream(ctx->data, data, 2, &iterator);
        ctx->size = 2;
        size -= 2;
        break;
    case MIDI_STATUS_SYSTEM_SONG_SELECT:
        // 1 byte payload
        alloc_and_copy_from_stream(ctx->data, data, 1, &iterator);
        ctx->size = 1;
        size -= 1;
        break;
    case MIDI_STATUS_SYSTEM_RESET_OR_META: {
        midi_event_meta_reset(&ctx->meta);
        iterator += midi_event_meta_unmarshal(&ctx->meta, data + iterator, size);
    }
    break;

    default:
        break;
    }

    return iterator;
}

int midi_event_unmarshal(midi_event_t *ctx, uint8_t *data, uint32_t size)
{
    int res = 0;
    uint32_t iterator = 0;
    if (res = vlv_unmarshal(&ctx->predelay, data + iterator, size), res < 0)
    {
        return -1;
    }
    iterator += res;
    size -= res;

    (*(uint8_t *)&ctx->cmd) = data[iterator++];
    if (!(ctx->cmd.status & MIDI_NEW_MESSAGE_4BIT_MASK))
    {
        return -1;
    }
    ctx->cmd.status &= ~MIDI_NEW_MESSAGE_4BIT_MASK;

    switch (ctx->cmd.status)
    {
    case MIDI_STATUS_NOTE_OFF:
    case MIDI_STATUS_NOTE_ON:
    case MIDI_STATUS_KEY_PRESSURE:
    case MIDI_STATUS_CONTROLLER_CHANGE:
        // exclusive handling 1-2 bytes
        // http://www33146ue.sakura.ne.jp/staff/iz/formats/midi-cntl.html
    case MIDI_STATUS_PITCH_BEND:
        // 2 bytes of payload
        alloc_and_copy_from_stream(ctx->data, data, 2, &iterator);
        ctx->size = 2;
        size -= 2;
        break;
    case MIDI_STATUS_PROGRAM_CHANGE:
    case MIDI_STATUS_CHANNEL_PRESSURE:
        // 1 bytes of payload
        alloc_and_copy_from_stream(ctx->data, data, 1, &iterator);
        ctx->size = 1;
        size -= 1;
        break;
    case MIDI_STATUS_SYSTEM: {
        if (res = midi_event_unmarshal_system(ctx, data + iterator, size), res < 0)
        {
            return -1;
        }
        iterator += res;
        size -= res;
        break;
    }
    default:
        break;
    }

    return iterator;
}

bool midi_event_is_system(midi_event_t *ctx)
{
    return ctx->cmd.status == MIDI_STATUS_SYSTEM;
}

bool midi_event_is_note(midi_event_t *ctx)
{
    return ctx->cmd.status == MIDI_STATUS_NOTE_OFF || ctx->cmd.status == MIDI_STATUS_NOTE_ON;
}
