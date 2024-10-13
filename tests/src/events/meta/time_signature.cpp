#include <gtest/gtest.h>

#include "events/meta/time_signature.h"

#include <array>

TEST(time_signature, midi_time_signature_unmarshal)
{
    std::array<uint8_t, 4> src = {0xAA, 0xBB, 0xCC, 0xDD};
    midi_time_signature_t ctx = {0};
    uint32_t iterator = 0;

    EXPECT_EQ(midi_time_signature_unmarshal(&ctx, src.data(), src.size()), 4);
    EXPECT_EQ(ctx.numerator, 0xAA);
    EXPECT_EQ(ctx.denominator, 0xBB);
    EXPECT_EQ(ctx.ticks_per_click, 0xCC);
    EXPECT_EQ(ctx.thirty_second_notes_per_crotchet, 0xDD);
}