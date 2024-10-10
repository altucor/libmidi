
#include <gtest/gtest.h>

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