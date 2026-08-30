#pragma once

#include <cstdint>

#include <stdbool.h>
#include <vector>
#include <memory>
#include <span>

namespace midi
{

    typedef void(midi_feed_cb_f)(void* /* ctx */, const uint8_t /* byte */);
    typedef void(midi_input_cb_f)(void* /* ctx */, const uint8_t* /* data */, const uint32_t /* size */);

    class PortInput
    {
    public:
        PortInput(const PortInput& other) = delete;
        PortInput& operator=(const PortInput& other) = delete;
        PortInput(const PortInput&& rhs) = delete;
        PortInput& operator=(PortInput&& rhs) = delete;

        ~PortInput();

        static std::unique_ptr<PortInput> create(
            const std::size_t portIndex,
            midi_input_cb_f* callback,
            void* ctx = nullptr);

    private:
        explicit PortInput();

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

    // ------------------------------------------------

    class PortOutput
    {
    public:
        PortOutput(const PortOutput& other) = delete;
        PortOutput& operator=(const PortOutput& other) = delete;
        PortOutput(const PortOutput&& rhs) = delete;
        PortOutput& operator=(const PortOutput&& other) = delete;

        ~PortOutput();
        static std::unique_ptr<PortOutput> create(const std::size_t portIndex);
        int sendData(const std::span<const std::byte>& data);
        int sendData(const uint8_t* data, const uint32_t size)
        {
            return sendData(
                std::vector<std::byte>(
                    reinterpret_cast<const std::byte*>(data), reinterpret_cast<const std::byte*>(data + size)));
        }

    private:
        explicit PortOutput();

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

}
