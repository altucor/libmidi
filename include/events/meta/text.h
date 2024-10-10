#ifndef MIDI_TEXT_EVENT_H
#define MIDI_TEXT_EVENT_H

#include <stdint.h>

typedef struct text_event
{
    char *val;
} midi_text_event_t;

#ifdef __cplusplus
extern "C" {
#endif

int midi_text_event_unmarshal(midi_text_event_t *ctx, uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_TEXT_EVENT_H
