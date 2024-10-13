#include <gtest/gtest.h>

#include "mthd.h"

#include <array>

TEST(mthd, mthd_full_check)
{
    using unique_mthd_t = std::unique_ptr<mthd_t, decltype(&mthd_free)>;
    unique_mthd_t mthd_ctx(mthd_new(), &mthd_free);

    constexpr std::array<uint8_t, 14> data = {0x4D, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06, 0x00, 0x01, 0x00, 0x02, 0x00, 0x60};
    EXPECT_EQ(mthd_unmarshal(mthd_ctx.get(), data.data(), data.size()), data.size());

    EXPECT_TRUE(0 == std::memcmp(mthd_ctx->mthd, mthd_header_reference, MTHD_MARKER_SIZE));
    EXPECT_EQ(mthd_ctx->length, 6);
    EXPECT_EQ(mthd_ctx->format, MTHD_FORMAT_MULTI_TRACK);
    EXPECT_EQ(mthd_ctx->track_count, 2);
    EXPECT_EQ(mthd_ctx->ppqn, 96);
}

TEST(mthd, mthd_not_enough_data)
{
    using unique_mthd_t = std::unique_ptr<mthd_t, decltype(&mthd_free)>;
    unique_mthd_t mthd_ctx(mthd_new(), &mthd_free);

    constexpr std::array<uint8_t, 13> data = {0x4D, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06, 0x00, 0x01, 0x00, 0x02, 0x00};
    EXPECT_EQ(mthd_unmarshal(mthd_ctx.get(), data.data(), data.size()), MIDI_ERROR_NOT_ENOUGH_DATA);

    EXPECT_STREQ(mthd_ctx->mthd, "");
    EXPECT_EQ(mthd_ctx->length, 0);
    EXPECT_EQ(mthd_ctx->format, MTHD_FORMAT_SINGLE_TRACK);
    EXPECT_EQ(mthd_ctx->track_count, 0);
    EXPECT_EQ(mthd_ctx->ppqn, 0);
}

TEST(mthd, mthd_invalid_mthd_marker)
{
    using unique_mthd_t = std::unique_ptr<mthd_t, decltype(&mthd_free)>;
    unique_mthd_t mthd_ctx(mthd_new(), &mthd_free);

    constexpr std::array<uint8_t, 14> data = {0x4D, 0x53, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06, 0x00, 0x01, 0x00, 0x02, 0x00, 0x60};
    EXPECT_EQ(mthd_unmarshal(mthd_ctx.get(), data.data(), data.size()), MIDI_ERROR_INVALID_MTHD_MARKER);

    EXPECT_EQ(std::memcmp(mthd_ctx->mthd, mthd_header_reference, MTHD_MARKER_SIZE), -1);
    EXPECT_EQ(mthd_ctx->length, 0);
    EXPECT_EQ(mthd_ctx->format, MTHD_FORMAT_SINGLE_TRACK);
    EXPECT_EQ(mthd_ctx->track_count, 0);
    EXPECT_EQ(mthd_ctx->ppqn, 0);
}

TEST(mthd, mthd_invalid_mthd_format)
{
    using unique_mthd_t = std::unique_ptr<mthd_t, decltype(&mthd_free)>;
    unique_mthd_t mthd_ctx(mthd_new(), &mthd_free);

    constexpr std::array<uint8_t, 14> data = {0x4D, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06, 0x00, 0x05, 0x00, 0x02, 0x00, 0x60};
    EXPECT_EQ(mthd_unmarshal(mthd_ctx.get(), data.data(), data.size()), MIDI_ERROR_INVALID_MTHD_FORMAT);

    EXPECT_TRUE(0 == std::memcmp(mthd_ctx->mthd, mthd_header_reference, MTHD_MARKER_SIZE));
    EXPECT_EQ(mthd_ctx->length, 6);
    EXPECT_EQ(mthd_ctx->format, 5);
    EXPECT_EQ(mthd_ctx->track_count, 0);
    EXPECT_EQ(mthd_ctx->ppqn, 0);
}
