#include <gtest/gtest.h>

#include "events/meta/tempo.h"

#include <array>

TEST(tempo, midi_tempo_unmarshal_120)
{
    std::array<uint8_t, 3> src = {0x07, 0x90, 0xFC};
    midi_tempo_t ctx = {0};
    uint32_t iterator = 0;

    EXPECT_EQ(midi_tempo_unmarshal(&ctx, src.data(), src.size()), 3);
    EXPECT_EQ(ctx.val, 120);
}

TEST(tempo, midi_tempo_unmarshal_160)
{
    std::array<uint8_t, 3> src = {0x05, 0xB8, 0xD0};
    midi_tempo_t ctx = {0};
    uint32_t iterator = 0;

    EXPECT_EQ(midi_tempo_unmarshal(&ctx, src.data(), src.size()), 3);
    EXPECT_EQ(ctx.val, 160);
}

TEST(tempo, midi_tempo_unmarshal_90)
{
    std::array<uint8_t, 3> src = {0x0A, 0x2C, 0x2A};
    midi_tempo_t ctx = {0};
    uint32_t iterator = 0;

    EXPECT_EQ(midi_tempo_unmarshal(&ctx, src.data(), src.size()), 3);
    EXPECT_EQ(ctx.val, 90);
}