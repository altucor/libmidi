#ifndef MIDI_SYSEX_VENDOR_ID
#define MIDI_SYSEX_VENDOR_ID

#include <stdint.h>
#include <assert.h>

#include "libmidi/util.h"
#include "libmidi/event/system/sysex_vendor_id.h"

typedef union _midi_vendor_t
{
    // Raw data
    uint32_t raw;

    // Raw halfs
    uint16_t half[2];

    // Raw array
    uint8_t arr[4];

    struct
    {
        uint8_t b1;
        uint8_t b2;
        uint8_t b3;
        uint8_t b4;
    };
} midi_vendor_t;

static_assert(sizeof(uint32_t) == sizeof(midi_vendor_t), "Invalid size of \"midi_vendor_t\"");

static inline bool midi_vendor_compare(const midi_vendor_t* v1, const midi_vendor_t* v2)
{
    return v1->raw == v2->raw;
}

static inline void midi_vendor_reset(midi_vendor_t* ctx)
{
    ctx->raw = 0;
}

static inline int midi_vendor_unmarshal(midi_vendor_t* ctx, const uint8_t* data, const uint32_t size)
{
    uint32_t iterator = 0;

    if (!size)
    {
        return -1;
    }

    // First byte zero means 3 byte id
    // Otherwise if first byte non zero, ID is single byte
    if (data[0] != 0x00)
    {
        ctx->b1 = data[0];
        iterator = 1;
        return iterator;
    }

    if (size < 3)
    {
        return -1;
    }

    // Skip byte at pos zero because it always zero
    // Example: 00H 20H 32H	Behringer GmbH
    for (uint8_t i = 0; i < 3; i++)
    {
        ctx->arr[i + 1] = data[i];
    }

    iterator = 3;

    return iterator;
}

static inline generic_state_error_e midi_vendor_feed(midi_vendor_t* ctx, generic_state_t* state, const uint8_t b)
{
    switch (state->state)
    {
        case GENERIC_STATE_INIT:
        {
            if (b == 0xF0)
            {
                return GENERIC_STATE_ERROR_NEED_MORE;
            }
            else if (b != 0)
            {
                ctx->arr[state->counter] = b;
                state->state = GENERIC_STATE_COMPLETE;

                return GENERIC_STATE_ERROR_OK;
            }
            else
            {
                state->counter_max = 3;
                ctx->arr[state->counter] = b;
                state->counter++;
                state->state = GENERIC_STATE_PROCESSING;

                return GENERIC_STATE_ERROR_NEED_MORE;
            }
        }

        case GENERIC_STATE_PROCESSING:
        {
            ctx->arr[state->counter + 1] = b;
            state->counter++;
            if (state->counter >= state->counter_max)
            {
                state->state = GENERIC_STATE_COMPLETE;
            }

            return GENERIC_STATE_ERROR_OK;
        }

        case GENERIC_STATE_COMPLETE:
        {
            return GENERIC_STATE_ERROR_OK;
        }

        case GENERIC_STATE_ERROR:
        default:
        {
            return GENERIC_STATE_ERROR_BAD_INPUT;
        }
    }
}

static inline int midi_vendor_marshal(midi_vendor_t* ctx)
{
    //

    return -1;
}

#endif // MIDI_SYSEX_VENDOR_ID
