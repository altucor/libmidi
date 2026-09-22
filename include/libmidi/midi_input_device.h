#ifndef MIDI_INPUT_STATE_MACHINE
#define MIDI_INPUT_STATE_MACHINE

#include <stdbool.h>
#include <stdint.h>

#include "libmidi/protocol.h"

#include "libmidi/buffer.h"

#include "libmidi/event.h"
#include "libmidi/util.h"
#include "libmidi/vlv.h"

#define MIDI_INPUT_DEVICE_BUFFER_SIZE (1024)

typedef void(midi_cb_error_f)(void* ctx, const midi_error_e err);
typedef void(midi_cb_event_f)(void* ctx, const midi_event_t* event);

typedef struct _midi_device_callback_data
{
    void* handle;
    midi_cb_error_f* error;
    midi_cb_event_f* event;
} midi_device_callback_data_t;

typedef void(midi_cb_input_state_handler_f)(void* ctx, const uint8_t b);
typedef enum _midi_input_state_t
{
    MIDI_INPUT_STATE_READY_TO_NEW = 0,
    MIDI_INPUT_STATE_PREDELAY,
    MIDI_INPUT_STATE_NEW_MESSAGE,
    MIDI_INPUT_STATE_READ_PAYLOAD,
    MIDI_INPUT_STATE_SYSTEM_META,
    MIDI_INPUT_STATE_READ_META_PAYLOAD_SIZE,
    MIDI_INPUT_STATE_READ_SYSEX,
    MIDI_INPUT_STATE_COUNT
} midi_input_state_t;

typedef struct _input_state_handlers_t
{
    midi_cb_input_state_handler_f* arr[MIDI_INPUT_STATE_COUNT];
} input_state_handlers_t;

typedef struct _midi_input_state_data_t
{
    uint32_t meta_length;
    uint32_t sysex_length;
    generic_state_t generic_state;
} midi_input_state_data_t;

typedef struct _midi_input_device_t
{
    input_state_handlers_t handlers;
    midi_device_callback_data_t* listener;
    bool smf; // SMF - Standard Midi File, if set then expect to see VLV pre-delays in stream
    midi_input_state_t state;
    midi_input_state_data_t state_data;
    midi_event_t event;
    vlv_t vlv;
    buffer_t buffer;
} midi_input_device_t;

#ifdef __cplusplus
extern "C"
{
#endif

void midi_input_device_reset(midi_input_device_t* ctx);
midi_input_device_t* midi_input_device_new(const bool smf);
void midi_input_device_free(midi_input_device_t* ctx);
void midi_input_device_set_listener(midi_input_device_t* ctx, midi_device_callback_data_t* listener);
void midi_input_device_remove_listener(midi_input_device_t* ctx);
void midi_input_device_feed(midi_input_device_t* ctx, const uint8_t b);
void midi_input_device_feed_chunk(midi_input_device_t* ctx, const uint8_t* data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_INPUT_STATE_MACHINE
