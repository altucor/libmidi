#pragma once

#include <CoreMIDI/CoreMIDI.h>
#include <cstdint>

#include <stdbool.h>
#include <vector>

typedef void(midi_feed_cb_f)(void* /* ctx */, const uint8_t /* byte */);

class MidiPortInput
{
public:
    MidiPortInput(const MidiPortInput& other) = delete;
    MidiPortInput& operator=(const MidiPortInput& other) = delete;

    MidiPortInput(const MIDIClientRef& client)
        : m_client(client)
    {
    }

    ~MidiPortInput()
    {
        MIDIEndpointDispose(m_source);
        MIDIPortDispose(m_inputPort);
    }

    int registerClient(const std::size_t portIndex, midi_feed_cb_f* callback, void* ctx = nullptr)
    {
        m_midiCb = callback;
        m_midiCtx = ctx;

        // Get the first MIDI input source
        ItemCount sourceCount = MIDIGetNumberOfSources();
        if (sourceCount == 0)
        {
            fprintf(stderr, "No MIDI sources found.\n");
            return -1;
        }

        // Create a MIDI input port
        OSStatus ret = MIDIInputPortCreate(
            m_client, CFSTR("Input Port"), (MIDIReadProc)&midiInputCallback, (void*)this, &m_inputPort);

        m_source = MIDIGetSource(portIndex);
        // Connect the input port to the MIDI source
        ret = MIDIPortConnectSource(m_inputPort, m_source, NULL);

        return ret;
    }

private:
    static void midiInputCallback(const MIDIPacketList* packetList, void* obj, void* srcConnRefCon)
    {
        if (!obj)
        {
            return;
        }

        auto ctx = reinterpret_cast<MidiPortInput*>(obj);

        for (int i = 0; i < packetList->numPackets; ++i)
        {
            MIDIPacket packet = packetList->packet[i];
            for (int j = 0; j < packet.length; ++j)
            {
                ctx->m_midiCb(ctx->m_midiCtx, packet.data[j]);
            }
        }
    }

private:
    const MIDIClientRef& m_client = 0;
    MIDIPortRef m_inputPort = 0;
    MIDIEndpointRef m_source = 0;

    midi_feed_cb_f* m_midiCb = NULL;
    void* m_midiCtx = NULL;
};

// ------------

class MacOsMidiDriver
{
public:
    MacOsMidiDriver()
    {
        MIDIClientCreate(CFSTR("MIDI Driver Input"), NULL, NULL, &m_client);
    };

    ~MacOsMidiDriver()
    {
        MIDIClientDispose(m_client);
    }

    std::size_t getSourcesCount()
    {
        return MIDIGetNumberOfSources();
    }

    int registerInput(const std::size_t portIndex, midi_feed_cb_f* callback, void* ctx = nullptr)
    {
        auto port = std::make_unique<MidiPortInput>(m_client);
        port->registerClient(portIndex, callback, ctx);
        m_ports.push_back(std::move(port));

        std::print("DBG: {:d}\n", m_ports.size());

        return 0;
    }

private:
    MIDIClientRef m_client = 0;
    std::vector<std::unique_ptr<MidiPortInput>> m_ports = {};
};
