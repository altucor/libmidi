#include <gtest/gtest.h>

#include "events/program_change.h"

#include <array>

TEST(program_change, midi_program_change_unmarshal_1)
{
    std::array<uint8_t, 1> src = {0x30};
    midi_program_change_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {0};
    cmd.status = MIDI_STATUS_PROGRAM_CHANGE;
    cmd.channel = 0;
    EXPECT_EQ(midi_program_change_unmarshal(&ctx, cmd, src.data(), src.size()), 1);
    EXPECT_EQ(ctx.channel, 0);
    EXPECT_EQ(ctx.program, 0x30);
}

TEST(program_change, midi_program_change_unmarshal_2)
{
    std::array<uint8_t, 1> src = {0x57};
    midi_program_change_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {0};
    cmd.status = MIDI_STATUS_PROGRAM_CHANGE;
    cmd.channel = 4;
    EXPECT_EQ(midi_program_change_unmarshal(&ctx, cmd, src.data(), src.size()), 1);
    EXPECT_EQ(ctx.channel, 4);
    EXPECT_EQ(ctx.program, 0x57);
}

TEST(program_change, midi_program_change_unmarshal_invalid)
{
    std::array<uint8_t, 1> src = {0x57};
    midi_program_change_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {0};
    cmd.status = MIDI_STATUS_NOTE_OFF;
    cmd.channel = 4;
    EXPECT_EQ(midi_program_change_unmarshal(&ctx, cmd, src.data(), src.size()), MIDI_ERROR_STATUS_INVALID);
    EXPECT_EQ(ctx.channel, 0);
    EXPECT_EQ(ctx.program, 0);
}
