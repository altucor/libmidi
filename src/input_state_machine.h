#ifndef MIDI_INPUT_STATE_MACHINE
#define MIDI_INPUT_STATE_MACHINE

#include "buffer.h"
#include "event.h"
#include "protocol.h"
#include "vlv.h"

#include <stdbool.h>

typedef void(midi_cb_event_f)(void *ctx, midi_cmd_t msg, uint8_t message_meta, midi_event_t event);
typedef void(midi_cb_note_f)(void *ctx, midi_note_t note);
typedef void(midi_cb_control_f)(void *ctx, midi_control_t control);
typedef void(midi_cb_pitch_f)(void *ctx, midi_pitch_t pitch);
typedef void(midi_cb_tempo_f)(void *ctx, midi_tempo_t tempo);

typedef struct midi_device_callback_data
{
    void *handle;
    midi_cb_event_f *event;
    midi_cb_note_f *note;
    midi_cb_control_f *control;
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
    MIDI_INPUT_STATE_READ_PAYLOAD_END,
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
    bool smf; // SMF - Standard Midi File, if set then expect to see VLV in stream
    midi_input_state_t state;
    midi_cmd_t message;
    uint8_t message_meta;
    vlv_t meta_length;
    vlv_t predelay;
    midi_event_t event;
    buffer_t payload;
} input_state_machine_t;

#ifdef __cplusplus
extern "C" {
#endif

void input_state_machine_reset(input_state_machine_t *ctx);
input_state_machine_t *input_state_machine_new(bool smf);
void input_state_machine_free(input_state_machine_t *ctx);
void input_state_machine_set_listener(input_state_machine_t *ctx, midi_device_callback_data_t listener);
uint32_t input_state_machine_get_predelay(input_state_machine_t *ctx);
void input_state_machine_feed(input_state_machine_t *ctx, const uint8_t b);

#ifdef __cplusplus
}
#endif

#endif // MIDI_INPUT_STATE_MACHINE
