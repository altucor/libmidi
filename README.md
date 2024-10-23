# libmidi

## Description
Another one midi library which allows to handle files and streams.
This project developed to cover needs in processing midi messages in personal projects.
It can be good candidate in processing midi messages on embedded platforms.
Can collect bytes from stream and call corresponding callbacks when some event occurs.
Parsing of SMF happends in the same manner just with vlv predelays.
Currently this project have big disadvantage - there is no code responsible for converting events back to stream because i didn't need that functionality at current moment

## Usage

### Stream unmarshaling
```C
// Include required headers
#include "protocol.h"
#include "event.h"
#include "util.h"
#include "midi_input_device.h"

#include <stdlib.h>

// Describe your general or specific handler for midi_input_device_t
void my_midi_event_handler(void *ctx, midi_cmd_t msg, uint8_t message_meta, midi_event_t *event)
{
    printf("MSG New: %d\n", msg.new_msg);
    printf("MSG Cmd: 0x%02X\n", msg.status);
    printf("MSG System: 0x%02X\n", msg.system);
    printf("MSG Channel: 0x%02X\n", msg.channel);
    
    printf("Meta msg code: 0x%02X\n", message_meta);

    // If this event is note then print it details
    if(msg.status == MIDI_STATUS_NOTE_OFF || msg.status == MIDI_STATUS_NOTE_ON)
    {
        midi_note_t nt = event.note;
        printf("Note is ON: %d channel: %d pitch: %d (freq: %.3f, note: %s) velocity: %d\n", nt.on, nt.channel, nt.pitch, pitch_to_freq(nt.pitch), pitch_to_name(nt.pitch), nt.velocity);
    }
}

int main(int argc, char *argv[])
{
    // Create stream object, false - means do not expect VLV predelays between events
    midi_input_device_t *input_device = midi_input_device_new(false);

    // Create callback table and set handlers for interesting types of events
    midi_device_callback_data_t *cb = malloc(sizeof(midi_device_callback_data_t));
    cb.handle = NULL; // Here you can set context of your object if your handler methods of some class
    cb.event = (midi_cb_event_f *)&my_midi_event_handler;

    // Register handler in input device
    midi_input_device_set_listener(input_device, cb);

    // Sample stream data
    uint8_t input_data[12] = {0x90, 0x3C, 0x64, 0x80, 0x3C, 0x40, 0x90, 0x38, 0x64, 0x80, 0x38, 0x40};
    
    // Feed input device with bytes and expect that your callback will be triggered
    for(uint32_t i=0; i<12; i++)
    {
        midi_input_device_feed(input_device, input_data[i]);
    }

    free(cb);
    // Remove input device
    midi_input_device_free(input_device);

    return 0;
}
```

### SMF unmarshaling
The real world exmaple can be checked in this project: https://github.com/altucor/midi_to_mikrotik_converter

```C++

#include "file.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char *argv[])
{
    std::ifstream midiStream{argv[1], midiStream.binary | midiStream.in};
    if (!midiStream.is_open())
    {
        std::cerr << "cannot open file: " << argv[1];
        return -1;
    }
    std::vector<unsigned char> midiFileBuffer(std::istreambuf_iterator<char>(midiStream), {});
    
    midi_file_t *midi_file = midi_file_new();
    int read = midi_file_unmarshal(midi_file, midiFileBuffer.data(), midiFileBuffer.size());
    if(read < 0)
    {
        // Free resources if error
        midi_file_free(midi_file);
        return read;
    }

    // Get total track count from SMF
    const uint16_t trackCount = midi_file_get_tracks_count(midi_file);
    for (uint32_t i = 0; i < trackCount; i++)
    {
        // Iterate over tracks
        mtrk_t *track = midi_file_get_track(midi_file, i);
        // Get event count for corresponding track
        uint32_t events_count = mtrk_get_events_count(track);
        for(uint32_t j = 0; j < events_count; j++)
        {
            // Iterate over events
            midi_event_smf_t event = mtrk_get_event(track, j);

            // Display note event details if it's found
            if(event.message.status == MIDI_STATUS_NOTE_OFF || event.message.status == MIDI_STATUS_NOTE_ON)
            {
                midi_note_t nt = event.event.note;
                std::cout << "Note is ON: " << nt.on;
                std::cout << " channel: " << nt.channel;
                std::cout << " pitch: " << nt.pitch;
                std::cout << " (freq: " << std::to_string(pitch_to_freq(nt.pitch));
                std::cout << " note: " << pitch_to_name(nt.pitch) << ")";
                std::cout << " velocity: " << nt.velocity;
                std::cout << std::endl;
            }
        }
    }

    // Free resources
    midi_file_free(midi_file);
    return 0;
}

```

## Building
```
mkdir build && cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release
cmake --build . -j4
```

## Testing
```
cmake --build . -j4 && ctest --output-on-failure --gtest_catch_exceptions=0
```

## Memory leak testing
```
leaks --atExit -- ./tests/midi-tests
```

## Code coverage
TBD
