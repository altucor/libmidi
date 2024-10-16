#include <gtest/gtest.h>

#include "events/channel_pressure.h"

#include <array>

TEST(channel_pressure, midi_channel_pressure_unmarshal)
{
    std::array<uint8_t, 1> src = {0x57};
    midi_channel_pressure_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {0};
    cmd.status = MIDI_STATUS_CHANNEL_PRESSURE;
    cmd.channel = 0;
    EXPECT_EQ(midi_channel_pressure_unmarshal(&ctx, cmd, src.data(), src.size()), 1);
    EXPECT_EQ(ctx.channel, 0);
    EXPECT_EQ(ctx.pressure, 0x57);
}

TEST(channel_pressure, midi_channel_pressure_channel)
{
    std::array<uint8_t, 1> src = {0x57};
    midi_channel_pressure_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {0};
    cmd.status = MIDI_STATUS_CHANNEL_PRESSURE;
    cmd.channel = 6;
    EXPECT_EQ(midi_channel_pressure_unmarshal(&ctx, cmd, src.data(), src.size()), 1);
    EXPECT_EQ(ctx.channel, 6);
    EXPECT_EQ(ctx.pressure, 0x57);
}

TEST(channel_pressure, midi_channel_pressure_invalid_cmd)
{
    std::array<uint8_t, 1> src = {0x57};
    midi_channel_pressure_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {0};
    cmd.status = MIDI_STATUS_NOTE_ON;
    cmd.channel = 6;
    EXPECT_EQ(midi_channel_pressure_unmarshal(&ctx, cmd, src.data(), src.size()), MIDI_ERROR_STATUS_INVALID);
    EXPECT_EQ(ctx.channel, 0);
    EXPECT_EQ(ctx.pressure, 0);
}

TEST(channel_pressure, midi_channel_pressure_not_data)
{
    std::array<uint8_t, 1> src = {0x97};
    midi_channel_pressure_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {0};
    cmd.status = MIDI_STATUS_CHANNEL_PRESSURE;
    cmd.channel = 6;
    EXPECT_EQ(midi_channel_pressure_unmarshal(&ctx, cmd, src.data(), src.size()), MIDI_ERROR_NOT_DATA);
    EXPECT_EQ(ctx.channel, 6);
    EXPECT_EQ(ctx.pressure, 0);
}
