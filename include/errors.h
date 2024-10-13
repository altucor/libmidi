#ifndef MIDI_ERRORS_H
#define MIDI_ERRORS_H

#include <stdint.h>

typedef enum _midi_error
{
    MIDI_ERROR_OK = 0,
    MIDI_ERROR_GENERAL = -1,
    MIDI_ERROR_NOT_ENOUGH_DATA = -2,
    MIDI_ERROR_NOT_DATA = -3,
    MIDI_ERROR_STATUS_INVALID = -4,
    MIDI_ERROR_INVALID_MTHD_MARKER = -5,
    MIDI_ERROR_INVALID_MTHD_FORMAT = -6,
} midi_error_e;

#endif // MIDI_ERRORS_H
