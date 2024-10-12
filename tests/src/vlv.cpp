#include <gtest/gtest.h>

#include "protocol.h"
#include "vlv.h"

#include <array>

TEST(vlv, vlv_feed_1)
{
    std::array<uint8_t, 4> src = {0xAA, 0xAA, 0xAA, 0x2A};
    vlv_t ctx = {0};
    uint32_t iterator = 0;
    bool res = false;

    res = vlv_feed(&ctx, src.at(0));
    EXPECT_FALSE(res);

    res = vlv_feed(&ctx, src.at(1));
    EXPECT_FALSE(res);

    res = vlv_feed(&ctx, src.at(2));
    EXPECT_FALSE(res);

    res = vlv_feed(&ctx, src.at(3));
    EXPECT_TRUE(res);

    EXPECT_EQ(ctx.val, 0x54A952A);
}

TEST(vlv, vlv_feed_2)
{
    std::vector<uint8_t> src = {0x7F, 0x7F, 0x7F, 0x7F};
    vlv_t ctx = {0};

    EXPECT_TRUE(vlv_feed(&ctx, src.at(0)));
    EXPECT_EQ(ctx.val, 0x7F);
}

TEST(vlv, vlv_feed_3)
{
    std::vector<uint8_t> src = {0xFF, 0xFF, 0xFF, 0x7F};
    vlv_t ctx = {0};
    uint32_t iterator = 0;
    bool res = false;

    res = vlv_feed(&ctx, src.at(0));
    EXPECT_FALSE(res);

    res = vlv_feed(&ctx, src.at(1));
    EXPECT_FALSE(res);

    res = vlv_feed(&ctx, src.at(2));
    EXPECT_FALSE(res);

    res = vlv_feed(&ctx, src.at(3));
    EXPECT_TRUE(res);

    EXPECT_EQ(ctx.val, 0xFFFFFFF);
}
