#include <gtest/gtest.h>

#include "mtrk.h"

#include <array>

TEST(mtrk, mtrk_full_check)
{
    // TODO: need here "midi_input_device_t"
    // using unique_mtrk_t = std::unique_ptr<mtrk_t, decltype(&mtrk_free)>;
    // unique_mtrk_t mtrk_ctx(mtrk_new(), &mtrk_free);

    // constexpr std::array<uint8_t, 54> data = {0x4D, 0x54, 0x72, 0x6B, 0x00, 0x00, 0x00, 0x13, 0x00, 0xFF, 0x51, 0x03, 0x07, 0x90, 0xFC, 0x00, 0xFF, 0x58,
    //                                           0x04, 0x04, 0x02, 0x18, 0x08, 0x00, 0xFF, 0x2F, 0x00, 0x4D, 0x54, 0x72, 0x6B, 0x00, 0x00, 0x02, 0x4B, 0x00,
    //                                           0xFF, 0x03, 0x07, 0x33, 0x78, 0x4F, 0x73, 0x63, 0x20, 0x31, 0x00, 0x90, 0x3C, 0x64, 0x60, 0x80, 0x3C, 0x40};
    // EXPECT_EQ(mtrk_unmarshal(mthd_ctx.get(), data.data(), data.size()), data.size());

    // EXPECT_TRUE(0 == std::memcmp(mthd_ctx->mthd, mthd_header_reference, MTHD_MARKER_SIZE));
    // EXPECT_EQ(mthd_ctx->length, 6);
    // EXPECT_EQ(mthd_ctx->format, MTHD_FORMAT_MULTI_TRACK);
    // EXPECT_EQ(mthd_ctx->track_count, 2);
    // EXPECT_EQ(mthd_ctx->ppqn, 96);
}