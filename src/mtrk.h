#ifndef MIDI_MTRK_H
#define MIDI_MTRK_H

#include "event.h"
#include "protocol.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct _mtrk
{
    char mtrk[MTRK_MARKER_SIZE];
    uint32_t size;
    uint32_t events_count;
    midi_event_t **events;
} mtrk_t;

#ifdef __cplusplus
extern "C" {
#endif

mtrk_t *mtrk_new();
void mtrk_free(mtrk_t *ctx);
int mtrk_unmarshal(mtrk_t *ctx, uint8_t *data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_MTRK_H
