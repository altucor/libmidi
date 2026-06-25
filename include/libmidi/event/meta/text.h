#ifndef MIDI_TEXT_EVENT_H
#define MIDI_TEXT_EVENT_H

#include <stdint.h>

typedef struct text_event
{
    uint32_t size;
    char* data;
} midi_text_event_t;

#ifdef __cplusplus
extern "C"
{
#endif

void midi_text_event_cleanup(midi_text_event_t* ctx);
int midi_text_event_unmarshal(midi_text_event_t* ctx, const uint8_t* data, const uint32_t size);
void midi_text_event_copy(midi_text_event_t* dst, const midi_text_event_t* src);
midi_text_event_t midi_text_event_new_from(const midi_text_event_t* src);
midi_text_event_t midi_text_event_new_from_string(const char* string);

#ifdef __cplusplus
}
#endif

#endif // MIDI_TEXT_EVENT_H
