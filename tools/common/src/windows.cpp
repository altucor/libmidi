

#include "common/driver.hpp"

#include <windows.h>
#include <mmsystem.h>

#include <array>
#include <vector>

#pragma comment(lib, "winmm.lib")

namespace midi
{

    class MidiWindowsBuffer
    {
    public:
        MidiWindowsBuffer(const HMIDIIN& port, const std::size_t size = 8192)
            : m_port(port)
        {
            std::memset(&m_header, 0x00, sizeof(m_header));
            m_buffer.resize(size);

            m_header.lpData = reinterpret_cast<LPSTR>(m_buffer.data());
            m_header.dwBufferLength = m_buffer.size();
            m_header.dwFlags = 0;

            // TODO: Implement error handling
            midiInPrepareHeader(m_port, &m_header, sizeof(m_header));
            midiInAddBuffer(m_port, &m_header, sizeof(m_header));
        }

        ~MidiWindowsBuffer()
        {
            midiInUnprepareHeader(m_port, &m_header, sizeof(m_header));
        }

    private:
        const HMIDIIN& m_port = nullptr;
        MIDIHDR m_header = {0};
        std::vector<uint8_t> m_buffer = {0};
    };

    struct PortInput::Impl
    {
        ~Impl()
        {
            midiInStop(m_port);
            midiInClose(m_port);
            m_port = nullptr;
        }

        static void midiInputCallback(
            HMIDIIN hMidiIn,
            UINT wMsg,
            DWORD_PTR dwInstance,
            DWORD_PTR dwParam1,
            DWORD_PTR dwParam2)
        {

            auto ctx = reinterpret_cast<PortInput::Impl*>(dwInstance);
            if (!ctx || !ctx->m_midiCb || !ctx->m_midiCtx)
            {
                return;
            }

            switch (wMsg)
            {
                // https://learn.microsoft.com/en-us/windows/win32/multimedia/mim-data
                case MIM_DATA:
                {
                    std::array<uint8_t, 3> data;
                    data[0] = dwParam1 & 0xFF;
                    data[1] = (dwParam1 >> 8) & 0xFF;
                    data[2] = (dwParam1 >> 16) & 0xFF;

                    ctx->m_midiCb(ctx->m_midiCtx, data.data(), data.size());
                    break;
                }

                // https://learn.microsoft.com/en-us/windows/win32/multimedia/mim-longdata
                case MIM_LONGDATA:
                {
                    if (!dwParam1)
                    {
                        break;
                    }

                    const MIDIHDR* midiHeader = reinterpret_cast<MIDIHDR*>(dwParam1);
                    if (midiHeader->dwBytesRecorded <= 0)
                    {
                        break;
                    }

                    std::vector<uint8_t> data(
                        reinterpret_cast<uint8_t*>(midiHeader->lpData),
                        reinterpret_cast<uint8_t*>(midiHeader->lpData) + midiHeader->dwBytesRecorded);

                    ctx->m_midiCb(ctx->m_midiCtx, data.data(), data.size());

                    break;
                }

                case MIM_OPEN:
                case MIM_CLOSE:
                case MIM_ERROR:
                case MIM_LONGERROR:
                default:
                {
                    break;
                }
            }

            //
        }

        HMIDIIN m_port = nullptr;
        std::vector<MidiWindowsBuffer> m_windowsBuffers = {};

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

        if (portIndex >= midiInGetNumDevs())
        {
            return port;
        }

        port->m_impl->m_midiCb = callback;
        port->m_impl->m_midiCtx = ctx;

        midiInOpen(
            &port->m_impl->m_port,
            portIndex,
            reinterpret_cast<DWORD_PTR>(&PortInput::Impl::midiInputCallback),
            reinterpret_cast<DWORD_PTR>(port->m_impl.get()),
            CALLBACK_FUNCTION);

        // Register buffers so windows can use them for SYSEX messages
        for (std::size_t i = 0; i < 10; i++)
        {
            port->m_impl->m_windowsBuffers.emplace_back(port->m_impl->m_port, 8192);
        }

        MMRESULT ret = midiInStart(port->m_impl->m_port);

        return port;
    }

    struct PortOutput::Impl
    {
        ~Impl()
        {
            //
        }
    };

    PortOutput::PortOutput()
        : m_impl(std::make_unique<Impl>())
    {
    }

    PortOutput::~PortOutput() = default;

    std::unique_ptr<PortOutput> PortOutput::create(const std::size_t portIndex)
    {
        auto port = std::unique_ptr<PortOutput>(new PortOutput());

        return port;
    }

    int PortOutput::sendData(const std::span<const std::byte>& data)
    {
        return -1;
    }
}
