#ifndef MIDI_NOTE_H
#define MIDI_NOTE_H

#include "protocol.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct midi_note
{
    bool on;
    uint8_t channel;
    uint8_t pitch;
    uint8_t velocity;
} midi_note_t;

#ifdef __cplusplus
extern "C" {
#endif

void midi_note_reset(midi_note_t *ctx);
int midi_note_unmarshal(midi_note_t *ctx, midi_cmd_t cmd, uint8_t *data, uint32_t size);
void midi_note_to_str(midi_note_t *ctx, char *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // MIDI_NOTE_H
