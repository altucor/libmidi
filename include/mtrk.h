#ifndef MIDI_MTRK_H
#define MIDI_MTRK_H

#include "event.h"
#include "midi_input_device.h"
#include "protocol.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct _mtrk
{
    midi_input_device_t *device;
    midi_device_callback_data_t cb;
    char mtrk[MTRK_MARKER_SIZE];
    uint32_t size;
    uint32_t events_count;
    midi_event_smf_t **events;
} mtrk_t;

#ifdef __cplusplus
extern "C" {
#endif

mtrk_t *mtrk_new(midi_input_device_t *device);
void mtrk_free(mtrk_t *ctx);
int mtrk_debug(mtrk_t *ctx, char *data, uint32_t size);
int mtrk_unmarshal(mtrk_t *ctx, uint8_t *data, uint32_t size);
uint32_t mtrk_get_events_count(mtrk_t *ctx);
midi_event_smf_t *mtrk_get_event(mtrk_t *ctx, const uint32_t index);
int32_t mtrk_find_event_index(mtrk_t *ctx, const uint32_t start_index, const midi_cmd_t cmd, const uint8_t message_meta);
int32_t mtrk_find_corresponding_note_off(mtrk_t *ctx, const uint32_t start_index, midi_event_smf_t noteOn);
uint64_t mtrk_get_duration(mtrk_t *ctx);
uint64_t mtrk_get_duration_channel(mtrk_t *ctx, const uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif // MIDI_MTRK_H
