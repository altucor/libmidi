#pragma once

#include "libmidi/file.h"

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
            auto typeToString = [](const m_live_tag_type_e type)
            {
                switch (type)
                {
                    case MIDI_M_LIVE_TAG_TYPE_GENRE: return "Genre";
                    case MIDI_M_LIVE_TAG_TYPE_ARTIST: return "Artist";
                    case MIDI_M_LIVE_TAG_TYPE_COMPOSER: return "Composer";
                    case MIDI_M_LIVE_TAG_TYPE_DURATION: return "Duration";
                    case MIDI_M_LIVE_TAG_TYPE_TEMPO: return "Tempo";

                    default: return "<UNKNOWN>";
                }
            };

            std::print(
                "Event predelay: {:04d} System Meta M-Live TAG: type: {:s}, text[{:d}]: {:s}\n",
                event->predelay,
                typeToString(event->meta.m_live_tag.type),
                event->meta.m_live_tag.size,
                std::string(reinterpret_cast<char*>(event->meta.m_live_tag.data), event->meta.m_live_tag.size));

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
            const static std::array<std::string_view, 15> kMajorScale = {
                "Cb major",
                "Gb major",
                "Db major",
                "Ab major",
                "Eb major",
                "Bb major",
                "F major",
                "C major",
                "G major",
                "D major",
                "A major",
                "E major",
                "B major",
                "F# major",
                "C# major"};

            const static std::array<std::string_view, 15> kMinorScale = {
                "Ab minor",
                "Eb minor",
                "Bb minor",
                "F minor",
                "C minor",
                "G minor",
                "D minor",
                "A minor",
                "E minor",
                "B minor",
                "F# minor",
                "C# minor",
                "G# minor",
                "D# minor",
                "A# minor"};

            std::print(
                "Event predelay: {:04d} System Meta Key Signature: sharps/flats: {:d}, scale: {:s}, humand readable: "
                "\"{:s}\"\n",
                event->predelay,
                event->meta.key_signature.sharps_flats,
                (event->meta.key_signature.scale == MIDI_SCALE_MINOR ? "Minor" : "Major"),
                (event->meta.key_signature.scale == MIDI_SCALE_MINOR
                     ? kMinorScale.at(event->meta.key_signature.sharps_flats + 7)
                     : kMajorScale.at(event->meta.key_signature.sharps_flats + 7)));
            break;
        }
        case MIDI_META_EVENT_PROPRIETARY_EVENT:
        {
            std::print(
                "Event predelay: {:04d} System Meta Proprietary: data[{:d}]: ",
                event->predelay,
                event->meta.proprietary.size);

            for (uint32_t i = 0; i < event->meta.proprietary.size; i++)
            {
                std::print("0x{:02X} ", event->meta.proprietary.data[i]);
            }

            std::print("\n");

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
        case MIDI_STATUS_SYSTEM_COMMON_SYSEX_END:
        {
            std::print(
                "Event predelay: {:04d} System SysEx: Manufacturer [0x{:X}] data[{:d}]: ",
                event->predelay,
                static_cast<uint8_t>(event->system.sysex.vendor),
                event->system.sysex.size);

            for (uint32_t i = 0; i < event->system.sysex.size; i++)
            {
                std::print("0x{:02X} ", event->system.sysex.data[i]);
            }

            std::print("\n");

            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_MTC_QUARTER_FRAME:
        {
            auto pieceToText = [](const mtc_piece_e piece)
            {
                switch (piece)
                {
                    case MTC_PIECE_FRAMES_LSN: return "FRAMES LSN";
                    case MTC_PIECE_FRAMES_MSN: return "FRAMES MSN";
                    case MTC_PIECE_SECONDS_LSN: return "SECONDS LSN";
                    case MTC_PIECE_SECONDS_MSN: return "SECONDS MSN";
                    case MTC_PIECE_MINUTES_LSN: return "MINUTES LSN";
                    case MTC_PIECE_MINUTES_MSN: return "MINUTES MSN";
                    case MTC_PIECE_HOURS_LSN: return "HOURS LSN";
                    case MTC_PIECE_HOURS_MSN: return "HOURS MSN";

                    default: return "<UNKNOWN>";
                }
            };

            std::print(
                "Event predelay: {:04d} System MTC Quarter frame: piece: {:s}, data: {:d}\n",
                event->predelay,
                pieceToText(event->system.mtc_quarter_frame.value.piece),
                event->system.mtc_quarter_frame.value.data);

            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_SONG_POSITION:
        {
            std::print(
                "Event predelay: {:04d} System Song position: {:d}\n",
                event->predelay,
                event->system.song_position.value);

            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_SONG_SELECT:
        {
            std::print(
                "Event predelay: {:04d} System Song select: {:d}\n",
                event->predelay,
                event->system.song_select.value.val);

            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_RESERVED_4:
        {
            std::print("Event predelay: {:04d} System MIDI_STATUS_SYSTEM_COMMON_RESERVED_4\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_UNOFFICIAL_BUS_SELECT:
        {
            std::print(
                "Event predelay: {:04d} System MIDI_STATUS_SYSTEM_COMMON_UNOFFICIAL_BUS_SELECT\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_COMMON_TUNE_REQUEST:
        {
            std::print("Event predelay: {:04d} System MIDI_STATUS_SYSTEM_COMMON_TUNE_REQUEST\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_REALTIME_TIMING_TICK:
        {
            std::print("Event predelay: {:04d} System Realtime Timing Tick\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_REALTIME_RESERVED_9:
        {
            std::print("Event predelay: {:04d} System MIDI_STATUS_SYSTEM_REALTIME_RESERVED_9\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_REALTIME_SONG_START:
        {
            std::print("Event predelay: {:04d} System Song START\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_REALTIME_SONG_CONTINUE:
        {
            std::print("Event predelay: {:04d} System Song CONTINUE\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_REALTIME_SONG_STOP:
        {
            std::print("Event predelay: {:04d} System Song STOP\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_REALTIME_RESERVED_13:
        {
            std::print("Event predelay: {:04d} System MIDI_STATUS_SYSTEM_REALTIME_RESERVED_13\n", event->predelay);
            break;
        }

        case MIDI_STATUS_SYSTEM_REALTIME_ACTIVE_SENSING:
        {
            std::print("Event predelay: {:04d} System Active Sensing\n", event->predelay);
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

void midiHandleEvent(const midi_event_t* event)
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
