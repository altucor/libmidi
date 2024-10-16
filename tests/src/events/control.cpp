#include <gtest/gtest.h>

#include "events/control.h"

#include <array>

TEST(control, midi_control_unmarshal)
{
    std::array<uint8_t, 2> src = {0x57, 0x45};
    midi_control_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {0};
    cmd.status = MIDI_STATUS_CONTROLLER_CHANGE;
    cmd.channel = 0;
    EXPECT_EQ(midi_control_unmarshal(&ctx, cmd, src.data(), src.size()), 2);
    EXPECT_EQ(ctx.channel, 0);
    EXPECT_EQ(ctx.control, 0x57);
    EXPECT_EQ(ctx.value, 0x45);
}

TEST(control, midi_control_unmarshal_channel)
{
    std::array<uint8_t, 2> src = {0x57, 0x45};
    midi_control_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {0};
    cmd.status = MIDI_STATUS_CONTROLLER_CHANGE;
    cmd.channel = 4;
    EXPECT_EQ(midi_control_unmarshal(&ctx, cmd, src.data(), src.size()), 2);
    EXPECT_EQ(ctx.channel, 4);
    EXPECT_EQ(ctx.control, 0x57);
    EXPECT_EQ(ctx.value, 0x45);
}

TEST(control, midi_control_unmarshal_invalid)
{
    std::array<uint8_t, 2> src = {0x57, 0x45};
    midi_control_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {0};
    cmd.status = MIDI_STATUS_NOTE_OFF;
    cmd.channel = 4;
    EXPECT_EQ(midi_control_unmarshal(&ctx, cmd, src.data(), src.size()), MIDI_ERROR_STATUS_INVALID);
    EXPECT_EQ(ctx.channel, 0);
    EXPECT_EQ(ctx.control, 0);
    EXPECT_EQ(ctx.value, 0);
}

TEST(control, midi_control_unmarshal_not_data)
{
    std::array<uint8_t, 2> src = {0x57, 0xF5};
    midi_control_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {0};
    cmd.status = MIDI_STATUS_CONTROLLER_CHANGE;
    cmd.channel = 4;
    EXPECT_EQ(midi_control_unmarshal(&ctx, cmd, src.data(), src.size()), MIDI_ERROR_NOT_DATA);
    EXPECT_EQ(ctx.channel, 4);
    EXPECT_EQ(ctx.control, 0x57);
    EXPECT_EQ(ctx.value, 0);
}
