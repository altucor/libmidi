#include <gtest/gtest.h>

#include "midi_input_device.h"

#include <array>

void my_handle_event(mtrk_t *ctx, midi_cmd_t msg, uint8_t message_meta, midi_event_t event)
{
}

TEST(midi_input_device, midi_input_device_full_check)
{
    using unique_midi_input_device_t = std::unique_ptr<midi_input_device_t, decltype(&midi_input_device_free)>;
    unique_midi_input_device_t input_device_ctx(midi_input_device_new(true), &midi_input_device_free);

    midi_device_callback_data_t cb;
    cb.handle = nullptr;
    cb.event = (midi_cb_event_f *)&my_handle_event;
    midi_input_device_set_listener(input_device_ctx.get(), &cb);

    constexpr std::array<uint8_t, 46> data = {0x00, 0xFF, 0x51, 0x03, 0x07, 0x90, 0xFC, 0x00, 0xFF, 0x58, 0x04, 0x04, 0x02, 0x18, 0x08, 0x00,
                                              0xFF, 0x2F, 0x00, 0x4D, 0x54, 0x72, 0x6B, 0x00, 0x00, 0x02, 0x4B, 0x00, 0xFF, 0x03, 0x07, 0x33,
                                              0x78, 0x4F, 0x73, 0x63, 0x20, 0x31, 0x00, 0x90, 0x3C, 0x64, 0x60, 0x80, 0x3C, 0x40};
    EXPECT_EQ(mthd_unmarshal(mthd_ctx.get(), data.data(), data.size()), data.size());

    EXPECT_TRUE(0 == std::memcmp(mthd_ctx->mthd, mthd_header_reference, MTHD_MARKER_SIZE));
    EXPECT_EQ(mthd_ctx->length, 6);
    EXPECT_EQ(mthd_ctx->format, MTHD_FORMAT_MULTI_TRACK);
    EXPECT_EQ(mthd_ctx->track_count, 2);
    EXPECT_EQ(mthd_ctx->ppqn, 96);
}