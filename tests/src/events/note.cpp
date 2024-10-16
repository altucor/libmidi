#include <gtest/gtest.h>

#include "events/note.h"

#include <array>

TEST(note, midi_note_unmarshal_on_0)
{
    std::array<uint8_t, 2> src = {0x30, 0x70};
    midi_note_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {.status = MIDI_STATUS_NOTE_ON, .channel = 0};
    EXPECT_EQ(midi_note_unmarshal(&ctx, cmd, src.data(), src.size()), 2);
    EXPECT_TRUE(ctx.on);
    EXPECT_EQ(ctx.channel, 0);
    EXPECT_EQ(ctx.pitch, 0x30);
    EXPECT_EQ(ctx.velocity, 0x70);
}

TEST(note, midi_note_unmarshal_off_5)
{
    std::array<uint8_t, 2> src = {0x56, 0x24};
    midi_note_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {.status = MIDI_STATUS_NOTE_OFF, .channel = 5};
    EXPECT_EQ(midi_note_unmarshal(&ctx, cmd, src.data(), src.size()), 2);
    EXPECT_FALSE(ctx.on);
    EXPECT_EQ(ctx.channel, 5);
    EXPECT_EQ(ctx.pitch, 0x56);
    EXPECT_EQ(ctx.velocity, 0x24);
}

TEST(note, midi_note_unmarshal_invalid_cmd)
{
    std::array<uint8_t, 2> src = {0x56, 0x24};
    midi_note_t ctx = {0};
    uint32_t iterator = 0;

    midi_cmd_t cmd = {.status = MIDI_STATUS_KEY_PRESSURE, .channel = 5};
    EXPECT_EQ(midi_note_unmarshal(&ctx, cmd, src.data(), src.size()), MIDI_ERROR_STATUS_INVALID);
    EXPECT_FALSE(ctx.on);
    EXPECT_EQ(ctx.channel, 0);
    EXPECT_EQ(ctx.pitch, 0);
    EXPECT_EQ(ctx.velocity, 0);
}
