#ifndef MIDI_ERRORS_H
#define MIDI_ERRORS_H

#include <stdint.h>

typedef enum _midi_error
{
    MIDI_ERROR_OK = 0,
    MIDI_ERROR_GENERAL = -1,
    MIDI_ERROR_NOT_DATA = -2,
    MIDI_ERROR_STATUS_INVALID = -3,
} midi_error_e;

#endif // MIDI_ERRORS_H
