#ifndef MIDI_FILE_H
#define MIDI_FILE_H

#include "mthd.h"
#include "mtrk.h"

typedef struct midi_file
{
    mthd_t mthd;
    mtrk_t *mtrk;
} midi_file_t;

#ifdef __cplusplus
extern "C" {
#endif

midi_file_t *midi_file_new();
void midi_file_free(midi_file_t *ctx);
bool midi_file_unmarshal(midi_file_t *ctx, uint8_t *data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_FILE_H
