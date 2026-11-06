#ifndef SYSEX_NON_REALTIME_HANDLER_H
#define SYSEX_NON_REALTIME_HANDLER_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define NON_REAL_SIZE (1024)

typedef struct _sysex_non_real_t
{
    uint32_t counter;
    uint8_t buffer[NON_REAL_SIZE];
} sysex_non_real_t;

static inline void sysex_non_real_init(sysex_non_real_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    memset(ctx, 0x00, sizeof(sysex_non_real_t));
}

static inline void sysex_non_real_feed(sysex_non_real_t* ctx, const uint8_t b)
{
    ctx->buffer[ctx->counter++] = b;
}

#endif // SYSEX_NON_REALTIME_HANDLER_H
