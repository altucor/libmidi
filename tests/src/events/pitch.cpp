#include <gtest/gtest.h>

#include "events/pitch.h"

#include <array>

TEST(pitch, midi_pitch_unmarshal)
{
    std::array<uint8_t, 2> src = {0x30, 0x70};
    midi_pitch_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {.status = MIDI_STATUS_PITCH_BEND, .subCmd = 0};
    EXPECT_EQ(midi_pitch_unmarshal(&ctx, cmd, src.data(), src.size()), 2);
    EXPECT_EQ(ctx.channel, 0);
    EXPECT_EQ(ctx.value, 0x7030);
}

TEST(pitch, midi_pitch_unmarshal_channel)
{
    std::array<uint8_t, 2> src = {0xAA, 0xBB};
    midi_pitch_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {.status = MIDI_STATUS_PITCH_BEND, .subCmd = 5};
    EXPECT_EQ(midi_pitch_unmarshal(&ctx, cmd, src.data(), src.size()), 2);
    EXPECT_EQ(ctx.channel, 5);
    EXPECT_EQ(ctx.value, 0xBBAA);
}

TEST(pitch, midi_pitch_unmarshal_invalid)
{
    std::array<uint8_t, 2> src = {0xAA, 0xBB};
    midi_pitch_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {.status = MIDI_STATUS_NOTE_ON, .subCmd = 5};
    EXPECT_EQ(midi_pitch_unmarshal(&ctx, cmd, src.data(), src.size()), -1);
    EXPECT_EQ(ctx.channel, 0);
    EXPECT_EQ(ctx.value, 0);
}

TEST(pitch, midi_pitch_unmarshal_val)
{
    std::array<uint8_t, 2> src = {0xAA, 0xBB};
    midi_pitch_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {.status = MIDI_STATUS_NOTE_ON, .subCmd = 5};
    EXPECT_EQ(midi_pitch_unmarshal(&ctx, cmd, src.data(), src.size()), -1);
    EXPECT_EQ(ctx.channel, 0);
    EXPECT_EQ(ctx.value, 0);
}
