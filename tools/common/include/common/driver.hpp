#pragma once

#include <CoreMIDI/CoreMIDI.h>
#include <cstdint>

#include <stdbool.h>
#include <vector>
#include <memory>
#include <span>

namespace midi
{

    typedef void(midi_feed_cb_f)(void* /* ctx */, const uint8_t /* byte */);
    typedef void(midi_input_cb_f)(void* /* ctx */, const uint8_t* /* data */, const uint32_t /* size */);

    class DriverMacOs
    {
    public:
        DriverMacOs(const DriverMacOs& other) = delete;
        void operator=(DriverMacOs const& other) = delete;

        ~DriverMacOs()
        {
            MIDIClientDispose(m_client);
        }

        static DriverMacOs& getInstance()
        {
            static DriverMacOs m_instance;
            return m_instance;
        }

        const std::size_t getInputsCount() noexcept
        {
            return MIDIGetNumberOfSources();
        }

        const std::size_t getOutputsCount() noexcept
        {
            return MIDIGetNumberOfDestinations();
        }

        const auto& getRef() noexcept
        {
            return m_client;
        }

    private:
        explicit DriverMacOs()
        {
            m_status = MIDIClientCreate(CFSTR("Driver libmidi MacOS"), NULL, NULL, &m_client);
        };

    private:
        OSStatus m_status = 0;
        MIDIClientRef m_client = 0;
    };

    class PortInput
    {
    public:
        PortInput(const PortInput& other) = delete;
        PortInput& operator=(const PortInput& other) = delete;
        PortInput(const PortInput&& rhs) = delete;
        PortInput& operator=(PortInput&& rhs) = delete;

        ~PortInput()
        {
            MIDIPortDisconnectSource(m_port, m_endpoint);
            MIDIEndpointDispose(m_endpoint);
            MIDIPortDispose(m_port);
        }

        static std::unique_ptr<PortInput> create(
            const std::size_t portIndex,
            midi_input_cb_f* callback,
            void* ctx = nullptr)
        {
            auto port = std::unique_ptr<PortInput>(new PortInput());

            const auto inputsCount = DriverMacOs::getInstance().getInputsCount();
            if (!inputsCount || portIndex >= inputsCount)
            {
                return port;
            }

            port->m_midiCb = callback;
            port->m_midiCtx = ctx;

            OSStatus ret = MIDIInputPortCreate(
                DriverMacOs::getInstance().getRef(),
                CFSTR("Input Port"),
                (MIDIReadProc)&PortInput::midiInputCallback,
                (void*)port.get(),
                &port->m_port);

            if (ret != 0)
            {
                return port;
            }

            port->m_endpoint = MIDIGetSource(portIndex);
            ret = MIDIPortConnectSource(port->m_port, port->m_endpoint, NULL);

            // MIDIEndpointRef destination = MIDIGetDestination(0);

            return port;
        }

    private:
        PortInput()
        {
        }

        static void midiInputCallback(const MIDIPacketList* packetList, void* obj, void* srcConnRefCon)
        {
            if (!obj)
            {
                return;
            }

            auto ctx = reinterpret_cast<PortInput*>(obj);
            if (!ctx->m_midiCb || !ctx->m_midiCtx)
            {
                return;
            }

            for (int i = 0; i < packetList->numPackets; ++i)
            {
                MIDIPacket packet = packetList->packet[i];
                ctx->m_midiCb(ctx->m_midiCtx, packet.data, packet.length);
            }
        }

    private:
        MIDIPortRef m_port = 0;
        MIDIEndpointRef m_endpoint = 0;

        midi_input_cb_f* m_midiCb = nullptr;
        void* m_midiCtx = nullptr;
    };

    // ------------------------------------------------

    class PortOutput
    {
    public:
        PortOutput(const PortOutput& other) = delete;
        PortOutput& operator=(const PortOutput& other) = delete;
        PortOutput(const PortOutput&& rhs) = delete;
        PortOutput& operator=(const PortOutput&& other) = delete;

        ~PortOutput()
        {
            MIDIEndpointDispose(m_endpoint);
            MIDIPortDispose(m_port);
        }

        static std::unique_ptr<PortOutput> create(const std::size_t portIndex)
        {
            auto port = std::unique_ptr<PortOutput>(new PortOutput());

            const auto outputsCount = DriverMacOs::getInstance().getOutputsCount();
            if (!outputsCount || portIndex >= outputsCount)
            {
                return port;
            }

            OSStatus ret =
                MIDIOutputPortCreate(DriverMacOs::getInstance().getRef(), CFSTR("Output Port"), &port->m_port);

            if (ret != 0)
            {
                return port;
            }

            port->m_endpoint = MIDIGetDestination(portIndex);

            return port;
        }

        int sendData(const std::span<const std::byte>& data)
        {
            MIDIPacketList packetList;
            MIDIPacket* packet = MIDIPacketListInit(&packetList);

            packet = MIDIPacketListAdd(
                &packetList, sizeof(packetList), packet, 0, data.size(), reinterpret_cast<const Byte*>(data.data()));

            OSStatus ret = MIDISend(m_port, m_endpoint, &packetList);
            if (ret != 0)
            {
                return ret;
            }

            return data.size();
        }

        int sendData(const uint8_t* data, const uint32_t size)
        {
            return sendData(
                std::vector<std::byte>(
                    reinterpret_cast<const std::byte*>(data), reinterpret_cast<const std::byte*>(data + size)));
        }

    private:
        PortOutput()
        {
        }

    private:
        MIDIPortRef m_port = 0;
        MIDIEndpointRef m_endpoint = 0;
    };

}
