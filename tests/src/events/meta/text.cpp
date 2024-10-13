#include <gtest/gtest.h>

#include "events/meta/text.h"

TEST(text, midi_text_event_unmarshal)
{
    std::string src = "Hello World123!@";
    midi_text_event_t ctx = {0};
    uint32_t iterator = 0;

    EXPECT_EQ(midi_text_event_unmarshal(&ctx, reinterpret_cast<uint8_t *>(src.data()), src.size()), src.size());
    EXPECT_STREQ(ctx.val, src.c_str());
    free(ctx.val);
}
