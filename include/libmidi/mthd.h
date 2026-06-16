#ifndef MIDI_MTHD_H
#define MIDI_MTHD_H

#include "protocol.h"

#include <stdint.h>

typedef enum mthd_format
{
    MTHD_FORMAT_SINGLE_TRACK = 0x00,
    MTHD_FORMAT_MULTI_TRACK,
    MTHD_FORMAT_MULTI_SONG,
} mthd_format_e;

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif

typedef struct __attribute__((packed)) _mthd
{
    char mthd[MTHD_MARKER_SIZE];
    uint32_t length;
    uint16_t format;
    uint16_t track_count;
    uint16_t ppqn;
} mthd_t;

#ifdef _MSC_VER
#pragma pack(pop)
#endif

#ifdef __cplusplus
extern "C" {
#endif

mthd_t *mthd_new();
void mthd_free(mthd_t *ctx);
int mthd_debug(mthd_t *ctx, char *data, uint32_t size);
int mthd_unmarshal(mthd_t *ctx, const uint8_t *data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_MTHD_H
