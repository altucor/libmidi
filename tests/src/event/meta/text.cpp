#include <gtest/gtest.h>

#include "libmidi/event/meta/text.h"

#include <string_view>

TEST(text, midi_text_event_unmarshal)
{
    std::string src = "Hello World123!@";
    midi_text_event_t ctx = {0};
    uint32_t iterator = 0;

    EXPECT_EQ(midi_text_event_unmarshal(&ctx, reinterpret_cast<uint8_t*>(src.data()), src.size()), src.size());
    EXPECT_EQ(src.size(), ctx.size);
    EXPECT_EQ(std::string_view(ctx.data, ctx.size), src.c_str());
}
