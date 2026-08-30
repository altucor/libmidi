#include <print>

#include "libmidi/file.h"
#include "common/eventPrint.hpp"

#include <memory>
#include <fstream>
#include <vector>
#include <string>

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
