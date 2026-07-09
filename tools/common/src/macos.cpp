#include <CoreMIDI/CoreMIDI.h>

#include <vector>
#include <memory>
#include <span>

#include "common/driver.hpp"

namespace midi
{
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

    struct PortInput::Impl
    {
        ~Impl()
        {
            MIDIPortDisconnectSource(m_port, m_endpoint);
            MIDIEndpointDispose(m_endpoint);
            MIDIPortDispose(m_port);
        }

        static void midiInputCallback(const MIDIPacketList* packetList, void* obj, void* srcConnRefCon)
        {
            if (!obj)
            {
                return;
            }

            auto ctx = reinterpret_cast<PortInput::Impl*>(obj);
            if (!ctx || !ctx->m_midiCb || !ctx->m_midiCtx)
            {
                return;
            }

            for (int i = 0; i < packetList->numPackets; ++i)
            {
                MIDIPacket packet = packetList->packet[i];
                ctx->m_midiCb(ctx->m_midiCtx, packet.data, packet.length);
            }
        }

        MIDIPortRef m_port = 0;
        MIDIEndpointRef m_endpoint = 0;

        midi_input_cb_f* m_midiCb = nullptr;
        void* m_midiCtx = nullptr;
    };

    PortInput::PortInput()
        : m_impl(std::make_unique<Impl>())
    {
    }

    PortInput::~PortInput() = default;

    std::unique_ptr<PortInput> PortInput::create(const std::size_t portIndex, midi_input_cb_f* callback, void* ctx)
    {
        auto port = std::unique_ptr<PortInput>(new PortInput());

        const auto inputsCount = DriverMacOs::getInstance().getInputsCount();
        if (!inputsCount || portIndex >= inputsCount)
        {
            return port;
        }

        port->m_impl->m_midiCb = callback;
        port->m_impl->m_midiCtx = ctx;

        OSStatus ret = MIDIInputPortCreate(
            DriverMacOs::getInstance().getRef(),
            CFSTR("Input Port"),
            (MIDIReadProc)&PortInput::Impl::midiInputCallback,
            (void*)port->m_impl.get(),
            &port->m_impl->m_port);

        if (ret != 0)
        {
            return port;
        }

        port->m_impl.get()->m_endpoint = MIDIGetSource(portIndex);
        ret = MIDIPortConnectSource(port->m_impl->m_port, port->m_impl->m_endpoint, NULL);

        return port;
    }

    struct PortOutput::Impl
    {
        ~Impl()
        {
            MIDIEndpointDispose(m_endpoint);
            MIDIPortDispose(m_port);
        }

        MIDIPortRef m_port = 0;
        MIDIEndpointRef m_endpoint = 0;
    };

    PortOutput::PortOutput()
        : m_impl(std::make_unique<Impl>())
    {
    }

    PortOutput::~PortOutput() = default;

    std::unique_ptr<PortOutput> PortOutput::create(const std::size_t portIndex)
    {
        auto port = std::unique_ptr<PortOutput>(new PortOutput());

        const auto outputsCount = DriverMacOs::getInstance().getOutputsCount();
        if (!outputsCount || portIndex >= outputsCount)
        {
            return port;
        }

        OSStatus ret =
            MIDIOutputPortCreate(DriverMacOs::getInstance().getRef(), CFSTR("Output Port"), &port->m_impl->m_port);

        if (ret != 0)
        {
            return port;
        }

        port->m_impl->m_endpoint = MIDIGetDestination(portIndex);

        return port;
    }

    int PortOutput::sendData(const std::span<const std::byte>& data)
    {
        MIDIPacketList packetList;
        MIDIPacket* packet = MIDIPacketListInit(&packetList);

        packet = MIDIPacketListAdd(
            &packetList, sizeof(packetList), packet, 0, data.size(), reinterpret_cast<const Byte*>(data.data()));

        OSStatus ret = MIDISend(m_impl->m_port, m_impl->m_endpoint, &packetList);
        if (ret != 0)
        {
            return ret;
        }

        return data.size();
    }
};
