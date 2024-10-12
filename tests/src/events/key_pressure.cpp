#include <gtest/gtest.h>

#include "events/key_pressure.h"

#include <array>

TEST(key_pressure, midi_key_pressure_unmarshal)
{
    std::array<uint8_t, 2> src = {0x34, 0x76};
    midi_key_pressure_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {.status = MIDI_STATUS_KEY_PRESSURE, .subCmd = 0};
    EXPECT_EQ(midi_key_pressure_unmarshal(&ctx, cmd, src.data(), src.size()), 2);
    EXPECT_EQ(ctx.channel, 0);
    EXPECT_EQ(ctx.pitch, 0x34);
    EXPECT_EQ(ctx.pressure, 0x76);
}

TEST(key_pressure, midi_key_pressure_unmarshal_channel)
{
    std::array<uint8_t, 2> src = {0x34, 0x76};
    midi_key_pressure_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {.status = MIDI_STATUS_KEY_PRESSURE, .subCmd = 6};
    EXPECT_EQ(midi_key_pressure_unmarshal(&ctx, cmd, src.data(), src.size()), 2);
    EXPECT_EQ(ctx.channel, 6);
    EXPECT_EQ(ctx.pitch, 0x34);
    EXPECT_EQ(ctx.pressure, 0x76);
}

TEST(key_pressure, midi_key_pressure_unmarshal_invalid)
{
    std::array<uint8_t, 2> src = {0x34, 0x76};
    midi_key_pressure_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {.status = MIDI_STATUS_NOTE_OFF, .subCmd = 6};
    EXPECT_EQ(midi_key_pressure_unmarshal(&ctx, cmd, src.data(), src.size()), -1);
    EXPECT_EQ(ctx.channel, 0);
    EXPECT_EQ(ctx.pitch, 0);
    EXPECT_EQ(ctx.pressure, 0);
}
