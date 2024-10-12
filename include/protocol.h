#ifndef MIDI_PROTOCOL_H
#define MIDI_PROTOCOL_H

#include <stdint.h>

#define MIDI_NOTES_IN_OCTAVE (12)
#define MIDI_CHANNELS_MAX_COUNT (16)
#define MIDI_CONTROLLERS_MAX_COUNT (128)

#define MIDI_STATUS_MESSAGE_CMD_MASK (0x07)
#define MIDI_STATUS_MESSAGE_SUBCMD_MASK (0x0F)
#define MIDI_NEW_MESSAGE_BYTE_MASK (0x80)
#define MIDI_NEW_MESSAGE_4BIT_MASK (0x08)

#define MIDI_VLV_CONTINUATION_BIT (0x80)
#define MIDI_VLV_DATA_MASK (0x7F)
#define MIDI_VLV_MAX_SIZE (sizeof(uint32_t))

#define MTHD_MARKER_SIZE (4)
#define MTRK_MARKER_SIZE (4)

const static char *mthd_header_reference = "MThd";
const static char *mtrk_header_reference = "MTrk";

const static char *kNotesStr[] = {
    "C0", "C#0", "D0", "Eb0", "E0", "F0", "F#0", "G0", "G#0", "A0", "Bb0", "B0", /* #0 */
    "C1", "C#1", "D1", "Eb1", "E1", "F1", "F#1", "G1", "G#1", "A1", "Bb1", "B1", /* #1 */
    "C2", "C#2", "D2", "Eb2", "E2", "F2", "F#2", "G2", "G#2", "A2", "Bb2", "B2", /* #2 */
    "C3", "C#3", "D3", "Eb3", "E3", "F3", "F#3", "G3", "G#3", "A3", "Bb3", "B3", /* #3 */
    "C4", "C#4", "D4", "Eb4", "E4", "F4", "F#4", "G4", "G#4", "A4", "Bb4", "B4", /* #4 */
    "C5", "C#5", "D5", "Eb5", "E5", "F5", "F#5", "G5", "G#5", "A5", "Bb5", "B5", /* #5 */
    "C6", "C#6", "D6", "Eb6", "E6", "F6", "F#6", "G6", "G#6", "A6", "Bb6", "B6", /* #6 */
    "C7", "C#7", "D7", "Eb7", "E7", "F7", "F#7", "G7", "G#7", "A7", "Bb7", "B7", /* #7 */
    "C8", "C#8", "D8", "Eb8", "E8", "F8", "F#8", "G8", "G#8", "A8", "Bb8", "B8", /* #8 */
    "C9", "C#9", "D9", "Eb9", "E9", "F9", "F#9", "G9", "G#9", "A9", "Bb9", "B9"  /* #9 */
};

const static float kNotesFreq[] = {
    8.18f,    8.66f,    9.18f,    9.72f,    10.30f,   10.91f,   11.56f,   12.25f,   12.98f,   13.75f,  14.57f,   15.43f,   /* #0 */
    16.35f,   17.32f,   18.35f,   19.45f,   20.60f,   21.83f,   23.12f,   24.50f,   25.96f,   27.50f,  29.14f,   30.87f,   /* #1 */
    32.70f,   34.65f,   36.71f,   38.89f,   41.20f,   43.65f,   46.25f,   49.0f,    51.91f,   55.0f,   58.27f,   61.74f,   /* #2 */
    65.41f,   69.30f,   73.42f,   77.78f,   82.41f,   87.31f,   92.50f,   98.0f,    103.83f,  110.0f,  116.54f,  123.47f,  /* #3 */
    130.81f,  138.59f,  146.83f,  155.56f,  164.81f,  174.61f,  185.0f,   196.0f,   207.65f,  220.0f,  233.08f,  246.94f,  /* #4 */
    261.63f,  277.18f,  293.66f,  311.13f,  329.63f,  349.23f,  369.99f,  392.0f,   415.30f,  440.0f,  466.16f,  493.88f,  /* #5 */
    523.25f,  554.37f,  587.33f,  622.25f,  659.25f,  698.46f,  739.99f,  783.99f,  830.61f,  880.0f,  932.33f,  987.77f,  /* #6 */
    1046.5f,  1108.73f, 1174.66f, 1244.51f, 1318.51f, 1396.91f, 1479.98f, 1567.98f, 1661.22f, 1760.0f, 1864.66f, 1975.53f, /* #7 */
    2093.0f,  2217.46f, 2349.32f, 2489.02f, 2637.02f, 2793.83f, 2959.96f, 3135.96f, 3322.44f, 3520.0f, 3729.31f, 3951.07f, /* #8 */
    4186.01f, 4434.92f, 4698.63f, 4978.03f, 5274.04f, 5587.65f, 5919.91f, 6271.93f, 6644.88f, 7040.0f, 7458.62f, 7902.13f  /* #9 */
};

typedef enum _midi_status
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

typedef enum _midi_status_system
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
    MIDI_STATUS_SYSTEM_ACTIVE_SENSING, // Ping every 300ms
    MIDI_STATUS_SYSTEM_RESET_OR_META,
} midi_status_system_e;

// Meta events: https://www.mixagesoftware.com/en/midikit/help/HTML/meta_events.html
typedef enum _midi_meta_event
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
    MIDI_META_EVENT_M_LIVE_TAG = 0x4B,
    MIDI_META_EVENT_TEMPO = 0x51,
    MIDI_META_EVENT_SMPTE_OFFSET = 0x54,
    MIDI_META_EVENT_TIME_SIGNATURE = 0x58,
    MIDI_META_EVENT_KEY_SIGNATURE = 0x59,
    MIDI_META_EVENT_PROPRIETARY_EVENT = 0x7F,
} midi_meta_event_e;

typedef struct _midi_cmd
{
    uint8_t subCmd : 4;
    uint8_t status : 4;
} midi_cmd_t;

#endif // MIDI_PROTOCOL_H
