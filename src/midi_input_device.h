#ifndef MIDI_INPUT_STATE_MACHINE
#define MIDI_INPUT_STATE_MACHINE

#include "buffer.h"
#include "event.h"
#include "protocol.h"
#include "vlv.h"

#include <stdbool.h>

typedef void(midi_cb_event_f)(void *ctx, midi_cmd_t msg, uint8_t message_meta, midi_event_t event);
typedef void(midi_cb_note_f)(void *ctx, midi_note_t note);
typedef void(midi_cb_key_pressure_f)(void *ctx, midi_key_pressure_t pressure);
typedef void(midi_cb_control_f)(void *ctx, midi_control_t control);
typedef void(midi_cb_program_change_f)(void *ctx, midi_program_change_t program_change);
typedef void(midi_cb_channel_pressure_f)(void *ctx, midi_channel_pressure_t pressure);
typedef void(midi_cb_pitch_f)(void *ctx, midi_pitch_t pitch);
typedef void(midi_cb_tempo_f)(void *ctx, midi_tempo_t tempo);

typedef struct midi_device_callback_data
{
    uint8_t channel;
    void *handle;
    midi_cb_event_f *event;
    midi_cb_note_f *note;
    midi_cb_key_pressure_f *key_pressure;
    midi_cb_control_f *control;
    midi_cb_program_change_f *program_change;
    midi_cb_channel_pressure_f *channel_pressure;
    midi_cb_pitch_f *pitch;
    midi_cb_tempo_f *tempo;
} midi_device_callback_data_t;

typedef void(midi_cb_state_handler_f)(void *ctx, const uint8_t b);
typedef enum midi_input_state
{
    MIDI_INPUT_STATE_READY_TO_NEW = 0,
    MIDI_INPUT_STATE_PREDELAY,
    MIDI_INPUT_STATE_NEW_MESSAGE,
    MIDI_INPUT_STATE_READ_PAYLOAD,
    MIDI_INPUT_STATE_SYSTEM_META,
    MIDI_INPUT_STATE_READ_META_PAYLOAD_SIZE,
    MIDI_INPUT_STATE_COUNT_TOTAL
} midi_input_state_t;

typedef struct input_state_handlers
{
    midi_cb_state_handler_f *arr[MIDI_INPUT_STATE_COUNT_TOTAL];
} input_state_handlers_t;

typedef struct input_state_machine
{
    input_state_handlers_t handlers;
    midi_device_callback_data_t listener;
    bool smf; // SMF - Standard Midi File, if set then expect to see VLV pre-delays in stream
    midi_input_state_t state;
    midi_event_smf_t event_smf;
    buffer_t payload;
} midi_input_device_t;

#ifdef __cplusplus
extern "C" {
#endif

void midi_input_device_reset(midi_input_device_t *ctx);
midi_input_device_t *midi_input_device_new(bool smf);
void midi_input_device_free(midi_input_device_t *ctx);
void midi_input_device_set_listener(midi_input_device_t *ctx, midi_device_callback_data_t listener);
uint32_t midi_input_device_get_predelay(midi_input_device_t *ctx);
void midi_input_device_feed(midi_input_device_t *ctx, const uint8_t b);

#ifdef __cplusplus
}
#endif

#endif // MIDI_INPUT_STATE_MACHINE
