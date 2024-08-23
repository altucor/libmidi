#ifndef MIDI_INPUT_STATE_MACHINE
#define MIDI_INPUT_STATE_MACHINE

#include "buffer.h"
#include "events/control.h"
#include "events/note.h"
#include "events/pitch.h"
#include "protocol.h"
#include "vlv.h"

#include <stdbool.h>

typedef void(midi_cb_state_handler_f)(void *ctx, const uint8_t b);

typedef struct input_state_handlers
{
    midi_cb_state_handler_f *arr[5];
} input_state_handlers_t;

// typedef struct state_handler
// {
//     /* data */
// } state_handler_t;

typedef void(midi_cb_note_f)(void *ctx, midi_note_t note);
typedef void(midi_cb_control_f)(void *ctx, midi_control_t control);
typedef void(midi_cb_pitch_f)(void *ctx, midi_pitch_t pitch);

typedef struct midi_device_callback_data
{
    void *synth;
    midi_cb_note_f *note;
    midi_cb_control_f *control;
    midi_cb_pitch_f *pitch;
} midi_device_callback_data_t;

typedef enum midi_input_state
{
    MIDI_INPUT_STATE_NEW_MESSAGE = 0,
    MIDI_INPUT_STATE_NEW_STATUS,
    MIDI_INPUT_STATE_READ_PAYLOAD,
    MIDI_INPUT_STATE_READ_PAYLOAD_END,
    MIDI_INPUT_STATE_STATUS_SYSTEM,
} midi_input_state_t;

typedef struct input_state_machine
{
    input_state_handlers_t handlers;
    bool smf; // SMF - Standard Midi File, if set then expect to see VLV in stream
    midi_input_state_t state;
    midi_cmd_t message;
    vlv_t vlv;
    buffer_t payload;
} input_state_machine_t;

#ifdef __cplusplus
extern "C" {
#endif

void input_state_machine_reset(input_state_machine_t *ctx);
input_state_machine_t *input_state_machine_new(bool smf);
void input_state_machine_free(input_state_machine_t *ctx);
void input_state_machine_feed(input_state_machine_t *ctx, const uint8_t b);

#ifdef __cplusplus
}
#endif

void handle_new_message(input_state_machine_t *ctx, const uint8_t b);
void handle_new_status(input_state_machine_t *ctx, const uint8_t b);
void handle_system_status(input_state_machine_t *ctx, const uint8_t b);
void handle_read_payload(input_state_machine_t *ctx, const uint8_t b);
void handle_read_payload_end(input_state_machine_t *ctx, const uint8_t b);

#endif // MIDI_INPUT_STATE_MACHINE
