#include <gtest/gtest.h>

#include "protocol.h"
#include "vlv.h"

#include <array>

TEST(vlv, vlv_feed_1)
{
    std::array<uint8_t, 4> src = {0xAA, 0xAA, 0xAA, 0x2A};
    vlv_t ctx = {0};
    bool res = false;

    res = vlv_feed(&ctx, src.at(0));
    EXPECT_FALSE(res);

    res = vlv_feed(&ctx, src.at(1));
    EXPECT_FALSE(res);

    res = vlv_feed(&ctx, src.at(2));
    EXPECT_FALSE(res);

    res = vlv_feed(&ctx, src.at(3));
    EXPECT_TRUE(res);

    EXPECT_EQ(vlv_get_value(&ctx), 0x54A952A);
}

TEST(vlv, vlv_feed_2)
{
    std::vector<uint8_t> src = {0x7F, 0x7F, 0x7F, 0x7F};
    vlv_t ctx = {0};

    EXPECT_TRUE(vlv_feed(&ctx, src.at(0)));
    EXPECT_EQ(vlv_get_value(&ctx), 0x7F);
}

TEST(vlv, vlv_feed_3)
{
    std::vector<uint8_t> src = {0xFF, 0xFF, 0xFF, 0x7F};
    vlv_t ctx = {0};
    bool res = false;

    res = vlv_feed(&ctx, src.at(0));
    EXPECT_FALSE(res);

    res = vlv_feed(&ctx, src.at(1));
    EXPECT_FALSE(res);

    res = vlv_feed(&ctx, src.at(2));
    EXPECT_FALSE(res);

    res = vlv_feed(&ctx, src.at(3));
    EXPECT_TRUE(res);

    EXPECT_EQ(vlv_get_value(&ctx), 0xFFFFFFF);
}

TEST(vlv, vlv_feed_4)
{
    vlv_t ctx = {0};
    bool res = false;

    res = vlv_feed(&ctx, 0x7F);
    EXPECT_TRUE(res);
    EXPECT_EQ(vlv_get_value(&ctx), 0x7F);

    vlv_reset(&ctx);
    res = vlv_feed(&ctx, 0x81);
    EXPECT_FALSE(res);
    res = vlv_feed(&ctx, 0x7F);
    EXPECT_TRUE(res);
    EXPECT_EQ(vlv_get_value(&ctx), 0xFF);

    vlv_reset(&ctx);
    res = vlv_feed(&ctx, 0x82);
    EXPECT_FALSE(res);
    res = vlv_feed(&ctx, 0x80);
    EXPECT_FALSE(res);
    res = vlv_feed(&ctx, 0x00);
    EXPECT_TRUE(res);
    EXPECT_EQ(vlv_get_value(&ctx), 0x8000);
}

TEST(vlv, vlv_fetch_1)
{
    std::array<uint8_t, 4> expected = {0xAA, 0xAA, 0xAA, 0x2A};
    vlv_t ctx = {0};

    vlv_set_value(&ctx, 0x54A952A);

    EXPECT_TRUE(vlv_can_fetch(&ctx));
    EXPECT_EQ(vlv_fetch(&ctx), expected.at(0));

    EXPECT_TRUE(vlv_can_fetch(&ctx));
    EXPECT_EQ(vlv_fetch(&ctx), expected.at(1));

    EXPECT_TRUE(vlv_can_fetch(&ctx));
    EXPECT_EQ(vlv_fetch(&ctx), expected.at(2));

    EXPECT_TRUE(vlv_can_fetch(&ctx));
    EXPECT_EQ(vlv_fetch(&ctx), expected.at(3));

    EXPECT_FALSE(vlv_can_fetch(&ctx));
}

TEST(vlv, vlv_fetch_2)
{
    vlv_t ctx = {0};

    vlv_set_value(&ctx, 0x00);
    EXPECT_FALSE(vlv_can_fetch(&ctx));
    EXPECT_EQ(vlv_fetch(&ctx), 0x00);
    EXPECT_FALSE(vlv_can_fetch(&ctx));

    vlv_reset(&ctx);
    vlv_set_value(&ctx, 0x7F);
    EXPECT_TRUE(vlv_can_fetch(&ctx));
    EXPECT_EQ(vlv_fetch(&ctx), 0x7F);
    EXPECT_FALSE(vlv_can_fetch(&ctx));

    vlv_reset(&ctx);
    vlv_set_value(&ctx, 0xFF);
    EXPECT_TRUE(vlv_can_fetch(&ctx));
    EXPECT_EQ(vlv_fetch(&ctx), 0x81);
    EXPECT_TRUE(vlv_can_fetch(&ctx));
    EXPECT_EQ(vlv_fetch(&ctx), 0x7F);
    EXPECT_FALSE(vlv_can_fetch(&ctx));

    vlv_reset(&ctx);
    vlv_set_value(&ctx, 0x8000);
    EXPECT_TRUE(vlv_can_fetch(&ctx));
    EXPECT_EQ(vlv_fetch(&ctx), 0x82);
    EXPECT_TRUE(vlv_can_fetch(&ctx));
    EXPECT_EQ(vlv_fetch(&ctx), 0x80);
    EXPECT_TRUE(vlv_can_fetch(&ctx));
    EXPECT_EQ(vlv_fetch(&ctx), 0x00);
    EXPECT_FALSE(vlv_can_fetch(&ctx));
}

TEST(vlv, vlv_feed_and_fetch)
{
    std::array<uint8_t, 2> src = {0xAA, 0x2A};
    vlv_t ctx = {0};

    bool res = false;

    res = vlv_feed(&ctx, src.at(0));
    EXPECT_FALSE(res);

    res = vlv_feed(&ctx, src.at(1));
    EXPECT_TRUE(res);

    EXPECT_EQ(vlv_get_value(&ctx), 0x152A);

    vlv_reset(&ctx);
    vlv_set_value(&ctx, 0x152A);

    EXPECT_TRUE(vlv_can_fetch(&ctx));
    EXPECT_EQ(vlv_fetch(&ctx), src.at(0));

    EXPECT_TRUE(vlv_can_fetch(&ctx));
    EXPECT_EQ(vlv_fetch(&ctx), src.at(1));

    EXPECT_FALSE(vlv_can_fetch(&ctx));
}
