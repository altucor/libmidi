#ifndef MIDI_MTRK_H
#define MIDI_MTRK_H

#include "event.h"
#include "input_state_machine.h"
#include "protocol.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct _mtrk
{
    input_state_machine_t *state_machine;
    midi_device_callback_data_t cb;
    char mtrk[MTRK_MARKER_SIZE];
    uint32_t size;
    uint32_t events_count;
    midi_event_smf_t **events;
} mtrk_t;

#ifdef __cplusplus
extern "C" {
#endif

mtrk_t *mtrk_new(input_state_machine_t *state_machine);
void mtrk_free(mtrk_t *ctx);
int mtrk_debug(mtrk_t *ctx, char *data, uint32_t size);
int mtrk_unmarshal(mtrk_t *ctx, uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_MTRK_H
