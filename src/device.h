#ifndef MIDI_DEVICE_H
#define MIDI_DEVICE_H

#include <stdbool.h>
#include <stdint.h>

#define MIDI_CHANNEL_ANY 16
#define MIDI_SYNTH_SLOTS 32

typedef void(f_midi_cb_note_t)(void *ctx, bool on, uint8_t pitch, uint8_t velocity);
typedef void(f_midi_cb_controller_t)(void *ctx, uint8_t controller, uint8_t value);
typedef void(f_midi_cb_pitch_bend_t)(void *ctx, uint16_t value);

typedef enum midi_device_state
{
    MIDI_STATE_NEW_STATUS = 0,
    MIDI_STATE_READ_CHANNEL_PAYLOAD,
} midi_device_state_t;

typedef struct midi_device_callback_data
{
    void *synth;
    f_midi_cb_note_t *note;
    f_midi_cb_controller_t *controller;
    f_midi_cb_pitch_bend_t *pitch_bend;
} midi_device_callback_data_t;

typedef struct midi_device
{
    uint8_t channel;
    midi_device_state_t state;
    uint8_t midi_status;
    uint32_t payload_size;
    uint32_t payload_index;
    uint8_t payload[2];
    midi_device_callback_data_t *synth_cb[MIDI_SYNTH_SLOTS];
} midi_device_t;

#ifdef __cplusplus
extern "C" {
#endif

midi_device_t *midi_device_new(uint8_t channel);
void midi_device_free(midi_device_t *ctx);
void midi_device_feed(midi_device_t *ctx, uint8_t b);
void midi_device_link_synth_to_channel(midi_device_t *ctx, uint8_t channel, midi_device_callback_data_t *cb_data);

#ifdef __cplusplus
}
#endif

#endif // MIDI_DEVICE_H
