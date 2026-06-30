#include <print>
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <fstream>
#include <chrono>

#include "libmidi/midi_input_device.h"
#include "libmidi/file.h"

#include "common/eventPrint.hpp"
#include "common/driver.hpp"

/*
    1. Should be able to decode SMF files and print all found events
    2. Should be able to accept byte sequence and decode it in series of midi events
    3. Should be able connect to MIDI input port and listen to all events and print them live
    4. Add ability to use UART/COM/TTY for receiving ???
    5. Add ability to act as proxy dumper by opening IN/OUT and listening for events
*/

int g_lastRetValue = 0;

enum Mode
{
    MODE_FILE,
    MODE_PORT,
    MODE_DATA,
    MODE_SERIAL
};

struct KeyValue
{
    KeyValue(const std::string& data, const std::size_t delimiterPos)
    {
        // 2 - skip double dash "--"
        key = data.substr(2, delimiterPos - 2);
        value = data.substr(delimiterPos + 1);
    }

    std::string key;
    std::string value;
};

class Config
{
public:
    explicit Config(int argc, char* argv[])
    {
        for (std::size_t i = 1; i < argc; i++)
        {
            // Minimal reference
            // "--k=v"

            auto argument = std::string(argv[i]);
            static const std::size_t kMinimumKvSize = 5;
            if (argument.size() < kMinimumKvSize)
            {
                continue;
            }

            if (argument.at(0) != '-' && argument.at(1) != '-')
            {
                continue;
            }

            auto delimiterIndex = argument.find('=');

            // delimiter cannot be earlier than at index 3
            if (delimiterIndex < 3 || delimiterIndex == std::string::npos)
            {
                continue;
            }

            auto kv = KeyValue(argument, delimiterIndex);

            if (kv.key == "file")
            {
                mode = MODE_FILE;
                filePath = kv.value;
            }
            else if (kv.key == "port")
            {
                mode = MODE_PORT;
                port = kv.value;
                // OS/Platform specific specify midi port for listening ???
            }
            else if (kv.key == "data")
            {
                mode = MODE_DATA;
                data = kv.value;
            }
            else if (kv.key == "smf" && (kv.value == "1" || kv.value == "true"))
            {
                smfData = true;
            }
            else
            {
                g_lastRetValue = -1;
            }
        }
    }

    Mode mode = MODE_FILE;
    bool smfData = false;
    std::string port = "";
    std::filesystem::path filePath = "";
    std::string data = "";
};

int handleFile(const std::filesystem::path& path)
{
    auto file = std::ifstream(path.string(), std::ios::binary);
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
        std::print("[midi-decoder] Failed 0x{:x}\n", result);
        return result;
    }

    const auto mthd = midi_file_get_mthd(midiFile.get());
    std::print(
        "[midi-decoder] MThd: header: \"{:s}\", length: {:d}, format: {:d}, tracks: {:d}, ppqn: {:d}\n",
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
            "[midi-decoder] Track: {:d} MTrk: header: \"{:s}\", size: {:d}, events: {:d}\n",
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

    return 0;
}

std::vector<std::byte> stringToBytes(const std::string& text)
{
    std::vector<std::byte> out = {};

    if (!text.size() || text.size() % 2 != 0)
    {
        std::print("[midi-decoder] Invalid data format, bytes are corrupted\n");
        return out;
    }

    for (std::size_t i = 0; i < text.size();)
    {
        const std::string textByte = {text.data() + i, 2};
        const std::byte byte = static_cast<std::byte>(std::stoul(textByte, nullptr, 16));
        out.push_back(byte);
        i += 2;
    }

    return out;
}

void removeAll(std::string& data, const std::string& key)
{
    bool shouldReplace = true;

    while (shouldReplace)
    {
        const auto pos = data.find(key);

        if (pos == std::string::npos)
        {
            shouldReplace = false;
            continue;
        }

        data.replace(pos, key.size(), "");
    }
}

void errorHandler(void* ctx, const midi_error_e err)
{
    g_lastRetValue = err;

    auto midiErrorToString = [](const midi_error_e err)
    {
        switch (err)
        {
            case MIDI_ERROR_OK: return "MIDI_ERROR_OK";
            case MIDI_ERROR_GENERAL: return "MIDI_ERROR_GENERAL";
            case MIDI_ERROR_INVALID_POINTER: return "MIDI_ERROR_INVALID_POINTER";
            case MIDI_ERROR_NOT_ENOUGH_DATA: return "MIDI_ERROR_NOT_ENOUGH_DATA";
            case MIDI_ERROR_NOT_DATA: return "MIDI_ERROR_NOT_DATA";
            case MIDI_ERROR_NOT_NEW_MESSAGE: return "MIDI_ERROR_NOT_NEW_MESSAGE";
            case MIDI_ERROR_STATUS_INVALID: return "MIDI_ERROR_STATUS_INVALID";
            case MIDI_ERROR_MTHD_INVALID_MARKER: return "MIDI_ERROR_MTHD_INVALID_MARKER";
            case MIDI_ERROR_MTHD_INVALID_FORMAT: return "MIDI_ERROR_MTHD_INVALID_FORMAT";
            case MIDI_ERROR_MTRK_INVALID_MARKER: return "MIDI_ERROR_MTRK_INVALID_MARKER";
            case MIDI_ERROR_MTRK_NOT_FULLY_PROCESSED: return "MIDI_ERROR_MTRK_NOT_FULLY_PROCESSED";
            case MIDI_ERROR_UNEXPECTED_STATUS: return "MIDI_ERROR_UNEXPECTED_STATUS";
            case MIDI_ERROR_UNEXPECTED_SYSTEM_RESERVED: return "MIDI_ERROR_UNEXPECTED_SYSTEM_RESERVED";

            default: return "<UNKNOWN>";
        }
    };

    std::print("[midi-decoder] Error: 0x{:04X} - {:s}\n", static_cast<uint32_t>(err), midiErrorToString(err));
}

void eventHandler(void* ctx, const midi_event_t* event)
{
    std::print("[midi-decoder] ");
    midiHandleEvent(event);
}

void eventHandlerRealtime(void* ctx, const midi_event_t* event)
{
    std::print("[{:%Y-%m-%d %H:%M:%S}] [midi-decoder] ", std::chrono::system_clock::now());
    midiHandleEvent(event);
}

int handleData(std::string& data, const bool smf)
{
    /*
        Possible formats:
        1) 0xAA 0xBB 0xCC 0xDD
        2) 0xAA, 0xBB, 0xCC, 0xDD
        3) AA BB CC DD
        4) AABBCCDD

        Decoder should only convert this type of string to bytes: AABBCCDD
        All other formats shoud be converted to: AABBCCDD
    */

    if (!data.size())
    {
        return -1;
    }

    removeAll(data, " ");
    removeAll(data, "0x");
    removeAll(data, ",");

    auto bytes = stringToBytes(data);

    std::print("[midi-decoder] Data[{:d}]: ", bytes.size());

    for (const auto& b : bytes)
    {
        std::print("0x{:02X} ", static_cast<int>(b));
    }

    std::print("\n");

    midi_input_device_t* device = midi_input_device_new(smf);
    if (!device)
    {
        return -1;
    }

    midi_device_callback_data_t cb = {0};
    cb.handle = nullptr;
    cb.error = &errorHandler;
    cb.event = &eventHandler;

    midi_input_device_set_listener(device, &cb);

    for (const auto& b : bytes)
    {
        midi_input_device_feed(device, static_cast<uint8_t>(b));
    }

    midi_input_device_free(device);

    return 0;
}

int handlePort(const std::string& portText)
{
    if (!portText.size())
    {
        std::print("[midi-decoder] MIDI port not specified\n");
        return -1;
    }

    const std::size_t port = std::stoul(portText, nullptr, 10);

    MacOsMidiDriver driver;

    if (!driver.getSourcesCount())
    {
        std::print("[midi-decoder] No available MIDI source ports\n");
        return -1;
    }

    if (port >= driver.getSourcesCount())
    {
        std::print("[midi-decoder] MIDI port number is out of range\n");
        return -1;
    }

    midi_input_device_t* device = midi_input_device_new(false);
    if (!device)
    {
        return -1;
    }

    midi_device_callback_data_t cb = {0};
    cb.handle = nullptr;
    cb.error = &errorHandler;
    cb.event = &eventHandlerRealtime;

    midi_input_device_set_listener(device, &cb);
    driver.registerInput(port, (midi_feed_cb_f*)&midi_input_device_feed, device);

    while (true)
    {
        // wait here??
    }

    midi_input_device_free(device);

    return 0;
}

int help(const std::string& appName)
{
    std::print(
        "Usage: {:s} <args>\n"
        "Argument format is only: <--key=value>\n"
        "Available keys:\n"
        "--file=<file-path> // Path to SMF(Standard Midi File)\n\n"
        "--port=<port-number> // Specify midi input port number\n\n"
        "--data=<hex byte array> // Supported formats:\n"
        "   \"AABBCCDD\"\n"
        "   \"AA BB CC DD\"\n"
        "   \"0xAA 0xBB 0xCC 0xDD\"\n"
        "   \"0xAA, 0xBB, 0xCC, 0xDD\"\n\n"
        "--smf=<1 or true> // Indicate in data mode that byte array is from SMF and can contain VLV pre-delays\n"
        "\n",
        std::filesystem::path(appName).filename().string());

    return 0;
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        return help(argv[0]);
    }

    Config cfg(argc, argv);

    if (g_lastRetValue != 0)
    {
        return g_lastRetValue;
    }

    switch (cfg.mode)
    {
        default:
        case MODE_FILE:
        {
            std::print("[midi-decoder] Mode: File\n");
            return handleFile(cfg.filePath);
        }

        case MODE_PORT:
        {
            std::print("[midi-decoder] Mode: Port\n");
            return handlePort(cfg.port);
        }

        case MODE_DATA:
        {
            std::print("[midi-decoder] Mode: Data\n");
            auto ret = handleData(cfg.data, cfg.smfData);
            if (g_lastRetValue != 0)
            {
                return g_lastRetValue;
            }
            else
            {
                return ret;
            }
        }
    }

    return 0;
}
