#ifndef CONTAINER_H
#define CONTAINER_H

#include "libmidi/event/system/vendor.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

// return entry->handler(entry->context, b);
typedef int(sysex_handler_cb_f)(void*, const uint8_t);

typedef struct _sysex_route_entry_t
{
    midi_vendor_t vendor;
    sysex_handler_cb_f* handler;
    void* context;
} sysex_route_entry_t;

typedef struct _sysex_router_t
{
    uint32_t size;
    uint32_t count;
    sysex_route_entry_t* routes;
} sysex_router_t;

sysex_router_t* sysex_router_new(const uint32_t size)
{
    sysex_router_t* ctx = calloc(1, sizeof(sysex_router_t));
    if (!ctx)
    {
        return NULL;
    }

    ctx->routes = calloc(size, sizeof(sysex_route_entry_t));
    if (!ctx->routes)
    {
        return ctx;
    }

    ctx->size = size;

    return ctx;
}

void sysex_router_free(sysex_router_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    if (ctx->routes != NULL)
    {
        free(ctx->routes);
    }

    free(ctx);
}

static inline uint32_t lower_bound(const sysex_router_t* ctx, midi_vendor_t key)
{
    uint32_t lo = 0u;
    uint32_t hi = ctx->count;

    while (lo < hi)
    {
        const uint32_t mid = lo + (hi - lo) / 2u;
        if (ctx->routes[mid].vendor.raw < key.raw)
        {
            lo = mid + 1u;
        }
        else
        {
            hi = mid;
        }
    }

    return lo;
}

int sysex_router_add(sysex_router_t* ctx, const sysex_route_entry_t entry)
{
    const uint32_t index = lower_bound(ctx, entry.vendor);
    ctx->routes[index] = entry;
    ctx->count++;
}

const sysex_route_entry_t* sysex_router_find(const sysex_router_t* ctx, const midi_vendor_t vendor)
{
    uint32_t i = lower_bound(ctx, vendor);
    if (i < ctx->count && ctx->routes[i].vendor_id == vendor.raw)
    {
        return &ctx->routes[i];
    }

    return NULL;
}

int sysex_router_handle(sysex_router_t* ctx, const midi_vendor_t vendor, const uint8_t b)
{
    sysex_route_entry_t* entry = sysex_router_find(ctx, vendor);
    if (!entry)
    {
        return -1;
    }

    return entry->handler(entry->context, b);
}

#endif // CONTAINER_H
