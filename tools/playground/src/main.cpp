#include <print>

#include "libmidi/file.h"

#include <memory>
#include <fstream>
#include <vector>
#include <string>

void midiHandleSystemMetaEvent(const midi_event_t* event)
{

    auto textPrint = [](const midi_event_t* event, const std::string name)
    {
        std::print(
            "Event predelay: {:04d} System Meta {:s} text: \"{:s}\"\n",
            event->predelay,
            name,
            std::string(event->meta.text.data, event->meta.text.size));
    };

    switch (event->message_meta)
    {
        case MIDI_META_EVENT_SEQUENCE_NUMBER:
        {
            std::print(
                "Event predelay: {:04d} System Meta sequence number: {:d}\n",
                event->predelay,
                event->meta.sequence_number.val);
            break;
        }
        case MIDI_META_EVENT_TEXT:
        {
            textPrint(event, "Generic");
            break;
        }
        case MIDI_META_EVENT_COPYRIGHT:
        {
            textPrint(event, "Copyright");
            break;
        }
        case MIDI_META_EVENT_TRACK_NAME:
        {
            textPrint(event, "Track name");
            break;
        }
        case MIDI_META_EVENT_INSTRUMENT_NAME:
        {
            textPrint(event, "Instrument name");
            break;
        }
        case MIDI_META_EVENT_LYRIC_TEXT:
        {
            textPrint(event, "Lyric");
            break;
        }
        case MIDI_META_EVENT_TEXT_MARKER:
        {
            textPrint(event, "Marker");
            break;
        }
        case MIDI_META_EVENT_CUE_POINT:
        {
            textPrint(event, "Cue point");
            break;
        }
        case MIDI_META_EVENT_PROGRAM_PATCH_NAME:
        {
            textPrint(event, "Program patch name");
            break;
        }
        case MIDI_META_EVENT_DEVICE_PORT_NAME:
        {
            textPrint(event, "Device port name");
            break;
        }
        case MIDI_META_EVENT_MIDI_CHANNEL_PREFIX:
        {
            std::print(
                "Event predelay: {:04d} System Meta Midi chanel prefix: {:d}\n",
                event->predelay,
                event->meta.channel_prefix.val);
            break;
        }
        case MIDI_META_EVENT_MIDI_PORT:
        {
            std::print("Event predelay: {:04d} System Meta Midi port: {:d}\n", event->predelay, event->meta.port.val);
            break;
        }
        case MIDI_META_EVENT_TRACK_END:
        {
            std::print("Event predelay: {:04d} System Meta TRACK_END\n", event->predelay);
            break;
        }
        case MIDI_META_EVENT_M_LIVE_TAG:
        {
            std::print("Event predelay: {:04d} System Meta UNSUPPORTED MIDI_META_EVENT_M_LIVE_TAG\n", event->predelay);
            break;
        }
        case MIDI_META_EVENT_TEMPO:
        {
            std::print("Event predelay: {:04d} System Meta Tempo: {:d}\n", event->predelay, event->meta.tempo.val);
            break;
        }
        case MIDI_META_EVENT_SMPTE_OFFSET:
        {
            auto fpsToFloat = [](const smpte_fps_t fps)
            {
                switch (fps)
                {
                    case SMPTE_FPS_24: return 24.0f;
                    case SMPTE_FPS_25: return 25.0f;
                    case SMPTE_FPS_29_97: return 29.97f;
                    case SMPTE_FPS_30: return 30.0f;
                    default: return 0.0f;
                }
            };

            std::print(
                "Event predelay: {:04d} System Meta SMPTE Offset: fps: {:.02f}, hours: {:d}, minutes: {:d}, seconds: "
                "{:d}, frames: {:d}, sub-frames: {:d}\n",
                event->predelay,
                fpsToFloat(event->meta.smpte_offset.hours.fps),
                event->meta.smpte_offset.hours.hours,
                event->meta.smpte_offset.minutes,
                event->meta.smpte_offset.seconds,
                event->meta.smpte_offset.frames,
                event->meta.smpte_offset.subframes);

            break;
        }
        case MIDI_META_EVENT_TIME_SIGNATURE:
        {
            std::print(
                "Event predelay: {:04d} System Meta Time signature: numerator: {:d}, denominator: {:d}, "
                "ticks_per_click: "
                "{:d}, thirty_second_notes_per_crotchet: {:d}\n",
                event->predelay,
                event->meta.time_signature.numerator,
                event->meta.time_signature.denominator,
                event->meta.time_signature.ticks_per_click,
                event->meta.time_signature.thirty_second_notes_per_crotchet);
            break;
        }
        case MIDI_META_EVENT_KEY_SIGNATURE:
        {
            std::print(
                "Event predelay: {:04d} System Meta UNSUPPORTED MIDI_META_EVENT_KEY_SIGNATURE\n", event->predelay);
            break;
        }
        case MIDI_META_EVENT_PROPRIETARY_EVENT:
        {
            std::print(
                "Event predelay: {:04d} System Meta UNSUPPORTED MIDI_META_EVENT_PROPRIETARY_EVENT\n", event->predelay);
            break;
        }

        default:
        {
            std::print("Event System Meta: UNKNOWN: 0x{:02X}\n", static_cast<uint8_t>(event->message_meta));
        }
    }
}

void midiHandleSystemEvent(const midi_event_t* event)
{
    if (!event)
    {
        return;
    }

    switch (event->message.system)
    {
        case MIDI_STATUS_SYSTEM_COMMON_SYSEX_START:
        {
            std::print(
                "Event predelay: {:04d} System UNSUPPORTED MIDI_STATUS_SYSTEM_COMMON_SYSEX_START\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_MTC_QUARTER_FRAME:
        {
            std::print(
                "Event predelay: {:04d} System UNSUPPORTED MIDI_STATUS_SYSTEM_COMMON_MTC_QUARTER_FRAME\n",
                event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_SONG_POSITION:
        {
            std::print(
                "Event predelay: {:04d} System UNSUPPORTED MIDI_STATUS_SYSTEM_COMMON_SONG_POSITION\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_SONG_SELECT:
        {
            std::print(
                "Event predelay: {:04d} System UNSUPPORTED MIDI_STATUS_SYSTEM_COMMON_SONG_SELECT\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_RESERVED_4:
        {
            std::print(
                "Event predelay: {:04d} System UNSUPPORTED MIDI_STATUS_SYSTEM_COMMON_RESERVED_4\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_UNOFFICIAL_BUS_SELECT:
        {
            std::print(
                "Event predelay: {:04d} System UNSUPPORTED MIDI_STATUS_SYSTEM_COMMON_UNOFFICIAL_BUS_SELECT\n",
                event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_TUNE_REQUEST:
        {
            std::print(
                "Event predelay: {:04d} System UNSUPPORTED MIDI_STATUS_SYSTEM_COMMON_TUNE_REQUEST\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_SYSEX_END:
        {
            // nothing to do here should not be seen
            break;
        }

        case MIDI_STATUS_SYSTEM_REALTIME_TIMING_TICK:
        {
            std::print(
                "Event predelay: {:04d} System UNSUPPORTED MIDI_STATUS_SYSTEM_REALTIME_TIMING_TICK\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_REALTIME_RESERVED_9:
        {
            std::print(
                "Event predelay: {:04d} System UNSUPPORTED MIDI_STATUS_SYSTEM_REALTIME_RESERVED_9\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_REALTIME_SONG_START:
        {
            std::print(
                "Event predelay: {:04d} System UNSUPPORTED MIDI_STATUS_SYSTEM_REALTIME_SONG_START\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_REALTIME_SONG_CONTINUE:
        {
            std::print(
                "Event predelay: {:04d} System UNSUPPORTED MIDI_STATUS_SYSTEM_REALTIME_SONG_CONTINUE\n",
                event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_REALTIME_SONG_STOP:
        {
            std::print(
                "Event predelay: {:04d} System UNSUPPORTED MIDI_STATUS_SYSTEM_REALTIME_SONG_STOP\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_REALTIME_RESERVED_13:
        {
            std::print(
                "Event predelay: {:04d} System UNSUPPORTED MIDI_STATUS_SYSTEM_REALTIME_RESERVED_13\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_REALTIME_ACTIVE_SENSING:
        {
            std::print(
                "Event predelay: {:04d} System UNSUPPORTED MIDI_STATUS_SYSTEM_REALTIME_ACTIVE_SENSING\n",
                event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_META:
        {
            midiHandleSystemMetaEvent(event);
            break;
        }

        default: break;
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
                "Event predelay: {:04d} Standard Note: {:s} ch: {:d}, pitch: {:d}, velocity: {:d}\n",
                event->predelay,
                (event->standard.note.on ? "ON " : "OFF"),
                event->standard.note.channel,
                event->standard.note.pitch,
                event->standard.note.velocity);
            break;
        }
        case MIDI_STATUS_KEY_PRESSURE:
        {
            std::print(
                "Event predelay: {:04d} Standard Key Pressure: ch {:d}, pitch: {:d}, pressure: {:d}\n",
                event->predelay,
                event->standard.key_pressure.channel,
                event->standard.key_pressure.pitch,
                event->standard.key_pressure.pressure);
            break;
        }
        case MIDI_STATUS_CONTROLLER_CHANGE:
        {
            std::print(
                "Event predelay: {:04d} Standard Controller Change: ch {:d}, control: {:d}, value: {:d}\n",
                event->predelay,
                event->standard.control.channel,
                event->standard.control.control,
                event->standard.control.value);
            break;
        }
        case MIDI_STATUS_PROGRAM_CHANGE:
        {
            std::print(
                "Event predelay: {:04d} Standard Program Change: ch {:d}, program: {:d}\n",
                event->predelay,
                event->standard.program_change.channel,
                event->standard.program_change.program);
            break;
        }
        case MIDI_STATUS_CHANNEL_PRESSURE:
        {
            std::print(
                "Event predelay: {:04d} Standard Channel Pressure: ch {:d}, pressure: {:d}\n",
                event->predelay,
                event->standard.channel_pressure.channel,
                event->standard.channel_pressure.pressure);
            break;
        }
        case MIDI_STATUS_PITCH_BEND:
        {
            std::print(
                "Event predelay: {:04d} Standard Pitchbend: ch {:d}, value: {:d}\n",
                event->predelay,
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
        // return result;
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
