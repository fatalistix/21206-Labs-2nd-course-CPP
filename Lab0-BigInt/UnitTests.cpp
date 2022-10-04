#include "BigInt"
#include <gtest/gtest.h>

TEST(BigInt, Constructors)
{
    EXPECT_NO_THROW(BigInt("0"));
    EXPECT_NO_THROW(BigInt("-0"));
    EXPECT_NO_THROW(BigInt("255"));
    EXPECT_NO_THROW(BigInt("-255"));
    EXPECT_NO_THROW(BigInt("+255"));
    EXPECT_NO_THROW(BigInt("61"));
    EXPECT_NO_THROW(BigInt("-32"));

}