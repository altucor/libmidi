#include <print>

#include "libmidi/file.h"

#include <memory>
#include <fstream>
#include <vector>
#include <string>

void midiHandleSystemEvent(midi_event_t* event)
{
    switch (event->message_meta)
    {
        case MIDI_META_EVENT_SEQUENCE_NUMBER:
        {
            break;
        }
        case MIDI_META_EVENT_TEXT:
        {
            break;
        }
        case MIDI_META_EVENT_COPYRIGHT:
        {
            break;
        }
        case MIDI_META_EVENT_TRACK_NAME:
        {
            break;
        }
        case MIDI_META_EVENT_INSTRUMENT_NAME:
        {
            break;
        }
        case MIDI_META_EVENT_LYRIC_TEXT:
        {
            break;
        }
        case MIDI_META_EVENT_TEXT_MARKER:
        {
            break;
        }
        case MIDI_META_EVENT_CUE_POINT:
        {
            break;
        }
        case MIDI_META_EVENT_PROGRAM_PATCH_NAME:
        {
            break;
        }
        case MIDI_META_EVENT_DEVICE_PORT_NAME:
        {
            break;
        }
        case MIDI_META_EVENT_MIDI_CHANNEL:
        {
            break;
        }
        case MIDI_META_EVENT_MIDI_PORT:
        {
            break;
        }
        case MIDI_META_EVENT_TRACK_END:
        {
            std::print("M");
            break;
        }
        case MIDI_META_EVENT_M_LIVE_TAG:
        {
            break;
        }
        case MIDI_META_EVENT_TEMPO:
        {
            break;
        }
        case MIDI_META_EVENT_SMPTE_OFFSET:
        {
            break;
        }
        case MIDI_META_EVENT_TIME_SIGNATURE:
        {
            break;
        }
        case MIDI_META_EVENT_KEY_SIGNATURE:
        {
            break;
        }
        case MIDI_META_EVENT_PROPRIETARY_EVENT:
        {
            break;
        }

        default:
        {
            std::print("Event Meta: UNKNOWN: 0x{:02X}\n", static_cast<uint8_t>(event->message_meta));
        }
    }
}

void midiHandleEvent(midi_event_t* event)
{
    switch (event->message.status)
    {
        case MIDI_STATUS_NOTE_OFF:
        case MIDI_STATUS_NOTE_ON:
        {
            std::print(
                "Event Standard Note: {:s} ch: {:d}, pitch: {:d}, velocity: {:d}\n",
                (event->standard.note.on ? "ON" : "OFF"),
                event->standard.note.channel,
                event->standard.note.pitch,
                event->standard.note.velocity);
            break;
        }
        case MIDI_STATUS_KEY_PRESSURE:
        {
            std::print(
                "Event Standard Key Pressure: ch {:d}, pitch: {:d}, pressure: {:d}\n",
                event->standard.key_pressure.channel,
                event->standard.key_pressure.pitch,
                event->standard.key_pressure.pressure);
            break;
        }
        case MIDI_STATUS_CONTROLLER_CHANGE:
        {
            std::print(
                "Event Standard Controller Change: ch {:d}, control: {:d}, value: {:d}\n",
                event->standard.control.channel,
                event->standard.control.control,
                event->standard.control.value);
            break;
        }
        case MIDI_STATUS_PROGRAM_CHANGE:
        {
            std::print(
                "Event Standard Program Change: ch {:d}, program: {:d}\n",
                event->standard.program_change.channel,
                event->standard.program_change.program);
            break;
        }
        case MIDI_STATUS_CHANNEL_PRESSURE:
        {
            std::print(
                "Event Standard Channel Pressure: ch {:d}, pressure: {:d}\n",
                event->standard.channel_pressure.channel,
                event->standard.channel_pressure.pressure);
            break;
        }
        case MIDI_STATUS_PITCH_BEND:
        {
            std::print(
                "Event Standard Pitchbend: ch {:d}, value: {:d}\n",
                event->standard.pitch.channel,
                event->standard.pitch.value);
            break;
        }
        case MIDI_STATUS_SYSTEM:
        {
            midiHandleSystemEvent(event);
            break;
        }

        default: break;
    }
}

int main(int argc, char* argv[])
{
    std::print("[playground] Started\n");

    if (argc < 2)
    {
        return 0;
    }

    std::string filePath = argv[1];
    auto file = std::ifstream(filePath, std::ios::binary);
    if (!file.is_open())
    {
        return -1;
    }

    file.seekg(0, std::ios::end);
    auto fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    auto midiFile = std::unique_ptr<midi_file_t, decltype(&midi_file_free)>(midi_file_new(), &midi_file_free);

    std::vector<uint8_t> data;
    data.resize(fileSize);
    file.read(reinterpret_cast<char*>(data.data()), data.size());
    const int result = midi_file_unmarshal(midiFile.get(), data.data(), data.size());
    if (result < 0)
    {
        std::print("[playground] Failed 0x{:x}\n", result);
        return result;
    }

    const auto mthd = midi_file_get_mthd(midiFile.get());
    std::print(
        "MThd: header: \"{:s}\", length: {:d}, format: {:d}, tracks: {:d}, ppqn: {:d}\n",
        std::string(reinterpret_cast<const char*>(&mthd.mthd), 4),
        mthd.length,
        mthd.format,
        mthd.track_count,
        mthd.ppqn);

    for (uint32_t i = 0; i < midi_file_get_tracks_count(midiFile.get()); i++)
    {
        mtrk_t* track = midi_file_get_track(midiFile.get(), i);
        if (!track)
        {
            return -1;
        }

        std::print(
            "Track: {:d} MTrk: header: \"{:s}\", size: {:d}, events: {:d}\n",
            i,
            std::string(reinterpret_cast<const char*>(&track->mtrk), 4),
            track->size,
            track->events_count);

        const auto eventsCount = mtrk_get_events_count(track);
        for (uint32_t j = 0; j < eventsCount; j++)
        {
            const auto event = mtrk_get_event(track, j);
            if (!event)
            {
                return -1;
            }

            midiHandleEvent(event);
        }
    }

    std::print("[playground] Finished\n");

    return 0;
}
