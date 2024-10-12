#include <gtest/gtest.h>

#include "protocol.h"
#include "util.h"

#include <vector>

TEST(util, readu16bswap_1122)
{
    uint16_t src = 0x1122;
    uint32_t iterator = 0;
    uint16_t res = readu16bswap(reinterpret_cast<uint8_t *>(&src), &iterator);
    EXPECT_EQ(iterator, 2);
    EXPECT_EQ(res, 0x2211);
}

TEST(util, readu16bswap_1002)
{
    uint16_t src = 0x1002;
    uint32_t iterator = 0;
    uint16_t res = readu16bswap(reinterpret_cast<uint8_t *>(&src), &iterator);
    EXPECT_EQ(iterator, 2);
    EXPECT_EQ(res, 0x0210);
}

TEST(util, readu32_11223344)
{
    uint32_t src = 0x11223344;
    uint32_t iterator = 0;
    uint32_t res = readu32(reinterpret_cast<uint8_t *>(&src), &iterator);
    EXPECT_EQ(iterator, 4);
    EXPECT_EQ(res, 0x11223344);
}

TEST(util, readu32bswap_11223344)
{
    uint32_t src = 0x11223344;
    uint32_t iterator = 0;
    uint32_t res = readu32bswap(reinterpret_cast<uint8_t *>(&src), &iterator);
    EXPECT_EQ(iterator, 4);
    EXPECT_EQ(res, 0x44332211);
}

TEST(util, pulses_per_second)
{
    EXPECT_NEAR(pulses_per_second(96, 120), 5.208f, 0.001);
    EXPECT_NEAR(pulses_per_second(100, 140), 4.285f, 0.001);
    EXPECT_NEAR(pulses_per_second(30, 160), 12.5f, 0.001);
}

TEST(util, duration_to_ms)
{
    EXPECT_NEAR(duration_to_ms(48, 5.208f), 249.984f, 0.001);
    EXPECT_NEAR(duration_to_ms(100, 5.208f), 520.800f, 0.001);
    EXPECT_NEAR(duration_to_ms(11, 5.208f), 57.288f, 0.001);
}

TEST(util, pitch_to_freq)
{
    EXPECT_NEAR(pitch_to_freq(60), 261.63f, 0.001);
    EXPECT_NEAR(pitch_to_freq(69), 440.0f, 0.001);
    EXPECT_NEAR(pitch_to_freq(0), 8.18f, 0.001);
    EXPECT_NEAR(pitch_to_freq(119), 7902.13f, 0.001);
}

TEST(util, pitch_to_name)
{
    EXPECT_STREQ(pitch_to_name(60), "C5");
    EXPECT_STREQ(pitch_to_name(69), "A5");
    EXPECT_STREQ(pitch_to_name(0), "C0");
    EXPECT_STREQ(pitch_to_name(119), "B9");
}

TEST(util, is_meta_text_event)
{
    EXPECT_TRUE(is_meta_text_event(MIDI_META_EVENT_TEXT));
    EXPECT_TRUE(is_meta_text_event(MIDI_META_EVENT_COPYRIGHT));
    EXPECT_TRUE(is_meta_text_event(MIDI_META_EVENT_TRACK_NAME));
    EXPECT_TRUE(is_meta_text_event(MIDI_META_EVENT_INSTRUMENT_NAME));
    EXPECT_TRUE(is_meta_text_event(MIDI_META_EVENT_LYRIC_TEXT));
    EXPECT_TRUE(is_meta_text_event(MIDI_META_EVENT_TEXT_MARKER));
    EXPECT_TRUE(is_meta_text_event(MIDI_META_EVENT_CUE_POINT));
    EXPECT_TRUE(is_meta_text_event(MIDI_META_EVENT_PROGRAM_PATCH_NAME));
    EXPECT_TRUE(is_meta_text_event(MIDI_META_EVENT_DEVICE_PORT_NAME));
    EXPECT_FALSE(is_meta_text_event(MIDI_META_EVENT_SEQUENCE_NUMBER));
    EXPECT_FALSE(is_meta_text_event(MIDI_META_EVENT_MIDI_CHANNEL));
    EXPECT_FALSE(is_meta_text_event(MIDI_META_EVENT_MIDI_PORT));
    EXPECT_FALSE(is_meta_text_event(MIDI_META_EVENT_TRACK_END));
    EXPECT_FALSE(is_meta_text_event(MIDI_META_EVENT_TEMPO));
}
