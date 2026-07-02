#ifndef MIDI_OUTPUT_STATE_MACHINE
#define MIDI_OUTPUT_STATE_MACHINE

#include "libmidi/protocol.h"

#include "libmidi/buffer.h"

#include "libmidi/event.h"
#include "libmidi/vlv.h"

#include <stdbool.h>

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

typedef struct _midi_output_device_t
{
    bool smf;
    vlv_t vlv;
} midi_output_device_t;

#ifdef __cplusplus
extern "C"
{
#endif

midi_output_device_t* midi_output_device_new(const bool smf);
void midi_output_device_free(midi_output_device_t* ctx);
int midi_output_device_send(midi_output_device_t* ctx, const midi_event_t* event, uint8_t* data, const uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_OUTPUT_STATE_MACHINE
