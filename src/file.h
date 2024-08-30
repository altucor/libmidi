#ifndef MIDI_FILE_H
#define MIDI_FILE_H

#include "midi_input_device.h"
#include "mthd.h"
#include "mtrk.h"

typedef struct midi_file
{
    midi_input_device_t *device;
    mthd_t mthd;
    mtrk_t **mtrk;
} midi_file_t;

#ifdef __cplusplus
extern "C" {
#endif

midi_file_t *midi_file_new();
void midi_file_free(midi_file_t *ctx);
int midi_file_debug(midi_file_t *ctx, char *data, uint32_t size);
int midi_file_unmarshal(midi_file_t *ctx, uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_FILE_H
