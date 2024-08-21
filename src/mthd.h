#ifndef MIDI_MTHD_H
#define MIDI_MTHD_H

#include "protocol.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum uint16_t
{
    MTHD_SINGLE_TRACK = 0x00,
    MTHD_MULTI_TRACK,
    MTHD_MULTI_SONG,
} mthd_format_e;

typedef struct _mthd
{
    char mthd[MTHD_MARKER_SIZE];
    uint32_t length;
    mthd_format_e format;
    uint16_t ppqn;
} mthd_t;

#ifdef __cplusplus
extern "C" {
#endif

mthd_t *mthd_new();
void mthd_free(mthd_t *ctx);
bool mthd_unmarshal(mthd_t *ctx, uint8_t *data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_MTHD_H
