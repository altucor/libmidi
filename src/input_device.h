#ifndef MIDI_DEVICE_H
#define MIDI_DEVICE_H

#include "buffer.h"
#include "events/note.h"
#include "protocol.h"

#include <stdbool.h>
#include <stdint.h>

#define MIDI_CHANNEL_ANY 16
#define MIDI_SYNTH_SLOTS 32

// #define MIDI_INPUT_DEVICE_PAYLOAD_BUFFER_SIZE (32)

typedef void(midi_cb_note_f)(void *ctx, midi_note_t note);
typedef void(midi_cb_controller_f)(void *ctx, uint8_t controller, uint8_t value);
typedef void(midi_cb_pitch_bend_f)(void *ctx, uint16_t value);

typedef enum midi_input_device_state
{
    MIDI_INPUT_DEVICE_STATE_NEW_MESSAGE = 0,
    MIDI_INPUT_DEVICE_STATE_NEW_STATUS,
    MIDI_INPUT_DEVICE_STATE_READ_PAYLOAD,
    MIDI_INPUT_DEVICE_STATE_READ_PAYLOAD_END,
    MIDI_INPUT_DEVICE_STATE_STATUS_SYSTEM,
    MIDI_INPUT_DEVICE_STATE_UNMARSHAL_STATUS,
} midi_input_device_state_t;

typedef struct midi_device_callback_data
{
    void *synth;
    midi_cb_note_f *note;
    midi_cb_controller_f *controller;
    midi_cb_pitch_bend_f *pitch_bend;
} midi_device_callback_data_t;

typedef struct midi_input_device
{
    uint8_t channel;
    midi_input_device_state_t state;
    // uint8_t midi_status;
    midi_cmd_t message;
    buffer_t payload;
    // uint32_t payload_size;
    // uint32_t payload_index;
    // uint8_t payload[MIDI_INPUT_DEVICE_PAYLOAD_BUFFER_SIZE];
    midi_device_callback_data_t *synth_cb[MIDI_SYNTH_SLOTS];
} midi_input_device_t;

#ifdef __cplusplus
extern "C" {
#endif

midi_input_device_t *midi_input_device_new(uint8_t channel);
void midi_input_device_free(midi_input_device_t *ctx);
void midi_input_device_feed(midi_input_device_t *ctx, const uint8_t b);
void midi_input_device_link_synth_to_channel(midi_input_device_t *ctx, uint8_t channel, midi_device_callback_data_t *cb_data);

#ifdef __cplusplus
}
#endif

#endif // MIDI_DEVICE_H
