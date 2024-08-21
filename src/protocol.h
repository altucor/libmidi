#ifndef MIDI_PROTOCOL_H
#define MIDI_PROTOCOL_H

#define MIDI_STATUS_MESSAGE_CMD_MASK 0x07
#define MIDI_STATUS_MESSAGE_CHANNEL_MASK 0x0F
#define MIDI_NEW_MESSAGE 0x80

#define MTHD_MARKER_SIZE (4)
#define MTRK_MARKER_SIZE (4)

const char *mthd_header_reference = "MThd";
const char *mtrk_header_reference = "MTrk";

typedef enum midi_status
{
    MIDI_STATUS_NOTE_OFF = 0x00,
    MIDI_STATUS_NOTE_ON,
    MIDI_STATUS_KEY_PRESSURE,
    MIDI_STATUS_CONTROLLER_CHANGE,
    MIDI_STATUS_PROGRAM_CHANGE,
    MIDI_STATUS_CHANNEL_PRESSURE,
    MIDI_STATUS_PITCH_BEND,
    MIDI_STATUS_SYSTEM,
} midi_status_e;

typedef enum midi_status_system
{
    MIDI_STATUS_SYSTEM_EXCLUSIVE = 0x00,
    MIDI_STATUS_SYSTEM_RESERVED_1,
    MIDI_STATUS_SYSTEM_SONG_POSITION,
    MIDI_STATUS_SYSTEM_SONG_SELECT,
    MIDI_STATUS_SYSTEM_RESERVED_4,
    MIDI_STATUS_SYSTEM_UNOFFICIAL_BUS_SELECT,
    MIDI_STATUS_SYSTEM_TUNE_REQUEST,
    MIDI_STATUS_SYSTEM_END_OF_SYSEX,
    MIDI_STATUS_SYSTEM_TIMING_TICK,
    MIDI_STATUS_SYSTEM_RESERVED_9,
    MIDI_STATUS_SYSTEM_START_SONG,
    MIDI_STATUS_SYSTEM_CONTINUE_SONG,
    MIDI_STATUS_SYSTEM_STOP_SONG,
    MIDI_STATUS_SYSTEM_RESERVED_13,
    MIDI_STATUS_SYSTEM_ACTIVE_SENSING,
    MIDI_STATUS_SYSTEM_SYSTEM_RESET,
} midi_status_system_e;

typedef enum midi_meta_event
{
    MIDI_META_EVENT_SEQUENCE_NUMBER = 0x00,
    MIDI_META_EVENT_TEXT = 0x01,
    MIDI_META_EVENT_COPYRIGHT = 0x02,
    MIDI_META_EVENT_TRACK_NAME = 0x03,
    MIDI_META_EVENT_INSTRUMENT_NAME = 0x04,
    MIDI_META_EVENT_LYRIC_TEXT = 0x05, // A single Lyric MetaEvent should contain only one syllable
    MIDI_META_EVENT_TEXT_MARKER = 0x06,
    MIDI_META_EVENT_CUE_POINT = 0x07,
    MIDI_META_EVENT_PROGRAM_PATCH_NAME = 0x08,
    MIDI_META_EVENT_DEVICE_PORT_NAME = 0x09,
    MIDI_META_EVENT_MIDI_CHANNEL = 0x20,
    MIDI_META_EVENT_MIDI_PORT = 0x21,
    MIDI_META_EVENT_TRACK_END = 0x2F,
    MIDI_META_EVENT_TEMPO = 0x51,
    MIDI_META_EVENT_SMPTE_OFFSET = 0x54,
    MIDI_META_EVENT_TIME_SIGNATURE = 0x58,
    MIDI_META_EVENT_KEY_SIGNATURE = 0x59,
    MIDI_META_EVENT_PROPRIETARY_EVENT = 0x7F,
} midi_meta_event_e;

#endif // MIDI_PROTOCOL_H