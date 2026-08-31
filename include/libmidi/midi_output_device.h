#ifndef MIDI_OUTPUT_STATE_MACHINE
#define MIDI_OUTPUT_STATE_MACHINE

#include "libmidi/protocol.h"

#include "libmidi/buffer.h"

#include "libmidi/event.h"
#include "libmidi/vlv.h"

#include <stdbool.h>
#include <stdint.h>

typedef void(midi_cb_output_state_handler_f)(void* ctx, const uint8_t b);

typedef enum _midi_output_state_t
{
    MIDI_OUTPUT_STATE_READY_TO_NEW = 0,
    MIDI_OUTPUT_STATE_COUNT
} midi_output_state_t;

typedef struct _output_state_handlers_t
{
    midi_cb_output_state_handler_f* arr[MIDI_OUTPUT_STATE_COUNT];
} output_state_handlers_t;

typedef int(midi_cb_user_send_f)(void* ctx, const uint8_t* buffer, const uint32_t size);

#define MIDI_OUTPUT_DEVICE_BUFFER_MAX (128)

typedef struct _midi_output_device_t
{
    bool smf;
    vlv_t vlv;

    uint8_t buffer[MIDI_OUTPUT_DEVICE_BUFFER_MAX];
    void* user_ctx;
    midi_cb_user_send_f* user_cb;
} midi_output_device_t;

#ifdef __cplusplus
extern "C"
{
#endif

midi_output_device_t* midi_output_device_new(const bool smf, void* user_ctx, midi_cb_user_send_f* user_cb);
void midi_output_device_free(midi_output_device_t* ctx);
int midi_output_device_send(midi_output_device_t* ctx, const midi_event_t* event);

#ifdef __cplusplus
}
#endif

#endif // MIDI_OUTPUT_STATE_MACHINE
