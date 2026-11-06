#ifndef SYSEX_INPUT_DEVICE_H
#define SYSEX_INPUT_DEVICE_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "libmidi/generic_state.h"
#include "libmidi/event/system/vendor.h"
#include "libmidi/util.h"
#include "libmidi/sysex_router.h"

#include "libmidi/sysex_non_realtime_handler.h"

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
    sysex_router_t* router;

    // Handlers
    sysex_non_real_t non_real_ctx;
} sysex_input_device_t;

int sysex_input_device_init(sysex_input_device_t* ctx)
{
    if (!ctx)
    {
        return -1;
    }

    memset(ctx, 0x00, sizeof(sysex_input_device_t));

    ctx->router = sysex_router_new(8);
    if (!ctx->router)
    {
        return -1;
    }

    sysex_route_entry_t non_real = {0};
    non_real.vendor.raw = SYSEX_ID_UNIVERSAL_NON_REALTIME;
    non_real.context = &ctx->non_real_ctx;
    non_real.handler = (sysex_handler_cb_f*)&sysex_non_real_feed;

    return sysex_router_add(ctx->router, non_real);
}

static inline int sysex_input_device_route(sysex_input_device_t* ctx, const uint8_t b)
{
    //
    return -1;
}

static inline int sysex_input_device_feed(sysex_input_device_t* ctx, const uint8_t b)
{
    if (b == 0xF7)
    {
        ctx->state = SYSEX_STATE_INIT;
        ctx->vendor.raw = 0;
        memset(&ctx->generic_state, 0x00, sizeof(generic_state_t));
    }

    switch (ctx->state)
    {
        case SYSEX_STATE_INIT:
        {
            if (b == 0xF0)
            {
                ctx->state = SYSEX_STATE_VENDOR_PARSE;
            }

            break;
        }

        case SYSEX_STATE_VENDOR_PARSE:
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

        case SYSEX_STATE_PAYLOAD_RECEIVE:
        {
            return sysex_router_handle(ctx->router, ctx->vendor, b);
        }

        case SYSEX_STATE_MAX:
        default:
        {
            return -1;
        }
    }

    return -1;
}

#endif // SYSEX_INPUT_DEVICE_H
