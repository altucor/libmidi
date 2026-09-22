#ifndef SYSEX_INPUT_DEVICE_H
#define SYSEX_INPUT_DEVICE_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "libmidi/generic_state.h"
#include "libmidi/event/system/vendor.h"
#include "libmidi/util.h"

typedef enum _sysex_state_e
{
    SYSEX_STATE_INIT = 0,
    SYSEX_STATE_VENDOR_PARSE,
    SYSEX_STATE_PAYLOAD_RECEIVE,
    SYSEX_STATE_MAX
} sysex_state_e;

typedef struct _sysex_input_device_t
{
    sysex_state_e state;
    generic_state_t generic_state;
    midi_vendor_t vendor;
} sysex_input_device_t;

int sysex_input_device_init(sysex_input_device_t* ctx)
{
    if (!ctx)
    {
        return -1;
    }

    memset(ctx, 0x00, sizeof(sysex_input_device_t));

    return 0;
}

static inline int sysex_input_device_route(sysex_input_device_t* ctx, const uint8_t b)
{
    //
}

static inline int sysex_input_device_feed(sysex_input_device_t* ctx, const uint8_t b)
{
    switch (ctx->state)
    {
        case SYSEX_STATE_INIT:
        {
            generic_state_error_e ret = midi_vendor_feed(&ctx->vendor, &ctx->generic_state, b);
            switch (ret)
            {
                case GENERIC_STATE_ERROR_OK:
                {
                    ctx->state = SYSEX_STATE_PAYLOAD_RECEIVE;
                    return 0;
                }
                case GENERIC_STATE_ERROR_NEED_MORE:
                {
                    // do nothing
                    return 0;
                }
                default:
                {
                    return -1;
                }
            }

            break;
        }

        case SYSEX_STATE_VENDOR_PARSE:
        {
            break;
        }

        case case SYSEX_STATE_PAYLOAD_RECEIVE:
        {
            return sysex_input_device_route(ctx, b);
        }

        case SYSEX_STATE_MAX:
        default:
        {
            return -1;
        }
    }
}

#endif // SYSEX_INPUT_DEVICE_H
