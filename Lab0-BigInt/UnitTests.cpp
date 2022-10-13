#include "BigInt.h"
#include "gtest/gtest.h"

using namespace LongMath;

TEST(Constructors, DefaultConstructor)
{
    BigInt x;
    EXPECT_TRUE (x.getArray().empty());
    EXPECT_FALSE(x.lessZero());
}

TEST(PrivateMethods, AddRadix)
{
    {
        BigInt num(std::vector<uchar>{0});
        num.addRadix();
        EXPECT_EQ(std::vector<uchar>({0, 0}), num.getArray());
    }
    {
        BigInt num(std::vector<uchar>{255, 0});
        num.addRadix();
        EXPECT_EQ(std::vector<uchar>({255, 0, 0}), num.getArray());
    }
    {
        BigInt num(std::vector<uchar>{0, 1});
        num.addRadix();
        EXPECT_EQ(std::vector<uchar>({0, 1, 0}), num.getArray());
    }
    {
        BigInt num(std::vector<uchar>{1, 1});
        num.addRadix();
        EXPECT_EQ(std::vector<uchar>({1, 1, 0}), num.getArray());
    }
    {
        BigInt num(std::vector<uchar>{255});
        num.addRadix();
        EXPECT_EQ(std::vector<uchar>({255, 255}), num.getArray());
    }
    {
        BigInt num(std::vector<uchar>{0, 250});
        num.addRadix();
        EXPECT_EQ(std::vector<uchar>({0, 250, 255}), num.getArray());
    }
}

TEST(PrivateMethods, PurgeRadix)
{
    {
        BigInt num(std::vector<uchar>{0});
        num.purgeRadix();
        EXPECT_EQ(std::vector<uchar>({0}), num.getArray());
    }
    {
        BigInt num(std::vector<uchar>{100});
        num.purgeRadix();
        EXPECT_EQ(std::vector<uchar>({100}), num.getArray());
    }
    {
        BigInt num(std::vector<uchar>{240});
        num.purgeRadix();
        EXPECT_EQ(std::vector<uchar>({240}), num.getArray());
    }
    {
        BigInt num(std::vector<uchar>{100, 100});
        num.purgeRadix();
        EXPECT_EQ(std::vector<uchar>({100, 100}), num.getArray());
    }
    {
        BigInt num(std::vector<uchar>{0, 100});
        num.purgeRadix();
        EXPECT_EQ(std::vector<uchar>({0, 100}), num.getArray());
    }
    {
        BigInt num(std::vector<uchar>{100, 250});
        num.purgeRadix();
        EXPECT_EQ(std::vector<uchar>({100, 250}), num.getArray());
    }
    {
        BigInt num(std::vector<uchar>{0, 250});
        num.purgeRadix();
        EXPECT_EQ(std::vector<uchar>({0, 250}), num.getArray());
    }
    {
        BigInt num(std::vector<uchar>{100, 0});
        num.purgeRadix();
        EXPECT_EQ(std::vector<uchar>({100}), num.getArray());
    }
    {
        BigInt num(std::vector<uchar>{100, 0, 0, 0, 0});
        num.purgeRadix();
        EXPECT_EQ(std::vector<uchar>({100}), num.getArray());
    }
    {
        BigInt num(std::vector<uchar>{100, 255});
        num.purgeRadix();
        EXPECT_EQ(std::vector<uchar>({100}), num.getArray());
    }
    {
        BigInt num(std::vector<uchar>{100, 255, 255, 255, 255});
        num.purgeRadix();
        EXPECT_EQ(std::vector<uchar>({100}), num.getArray());
    }
}

TEST(Constructors, IntConstructor)
{
    for (size_t i = 1; i < UINT32_MAX; i <<= 1)
    {
        for (size_t k = 0; k < 4; k++)
        {
            int compare = static_cast<int>(i + k - 1);
            {
                std::vector<uchar> num = BigInt(compare).getArray();
                unsigned int buf = 0;
                for (size_t j = 0; j < sizeof(int); j++) {
                    buf = buf * 256 + (sizeof(int) - j - 1 < num.size() ? num[sizeof(int) - 1 - j] : 0);
                }
                EXPECT_EQ(buf, compare);
            }
            compare = -compare;
            {
                if (compare)
                {
                    std::vector<uchar> num = BigInt(compare).getArray();
                    unsigned int buf = 0;
                    for (size_t j = 0; j < sizeof(int); j++) {
                        buf = buf * 256 + (sizeof(int) - j - 1 < num.size() ? num[sizeof(int) - 1 - j] : UINT8_MAX);
                    }
                    EXPECT_EQ(buf, compare);
                }
            }
        }
    }
}

TEST(OperatorsWithAssignment, CopyAssign)
{
    BigInt a;
    {
        BigInt b;
        EXPECT_EQ(b, a = b);
    }
    EXPECT_EQ(ONE, a = ONE);
    EXPECT_EQ(-ZERO, a = ZERO);
    {
        BigInt b(120);
        EXPECT_EQ(b, a = b);
    }
    {
        BigInt b(-120);
        EXPECT_EQ(b, a = b);
    }
    {
        BigInt b(370);
        EXPECT_EQ(b, a = b);
    }
    BigInt b(-370);
    EXPECT_EQ(b, a = b);
}

TEST(OperatorsWithAssignment, MoveAssign)
{
    BigInt a;
    {
        BigInt b;
        EXPECT_EQ(b, a = std::move(BigInt()));
    }
    EXPECT_EQ(ONE, a = std::move(BigInt(1)));
    EXPECT_EQ(ZERO, a = std::move(-ZERO));
    {
        BigInt b(120);
        EXPECT_EQ(BigInt(120), a = std::move(b));
    }
    {
        BigInt b(-120);
        EXPECT_EQ(BigInt(-120), a = std::move(b));
    }
    {
        BigInt b(370);
        EXPECT_EQ(BigInt(370), a = std::move(b));
    }
    BigInt b(-370);
    EXPECT_EQ(BigInt(-370), a = std::move(b));
}

TEST(OperatorsWithAssignment, Mul)
{
    {
        for (size_t i = 1; i < UINT32_MAX; i<<=1)
        {
            for (size_t k = 0; k < 4; k++) {
                int compare = static_cast<int>(i + k - 1);
                BigInt b(compare);
                b *= ZERO;
                EXPECT_EQ(ZERO, b);
            }
        }
    }
    {
        for (size_t i = 1; i < UINT32_MAX; i<<=1)
        {
            for (size_t k = 0; k < 4; k++) {
                int compare = static_cast<int>(i + k - 1);
                BigInt b(compare);
                BigInt buf(compare);
                b *= ONE;
                EXPECT_EQ(buf, b);
            }
        }
    }
    {
        {
            BigInt a(13);
            BigInt b(15);
            b *= a;
            EXPECT_EQ(b, BigInt(13*15));
        }
        {
            BigInt a(13);
            BigInt b(47);
            b *= a;
            EXPECT_EQ(b, BigInt(13 * 47));
        }
        {
            BigInt a(13);
            BigInt b(276);
            b *= a;
            EXPECT_EQ(b, BigInt(13 * 276));
        }
        {
            BigInt a(276);
            BigInt b(13);
            b *= a;
            EXPECT_EQ(b, BigInt(13 * 276));
        }
        {
            BigInt a(UINT16_MAX + 10);
            BigInt b(200);
            b *= a;
            EXPECT_EQ(b, BigInt(200 * (UINT16_MAX + 10)));
        }
        {
            BigInt a(200);
            BigInt b(UINT16_MAX + 10);
            b *= a;
            EXPECT_EQ(b, BigInt(200 * (UINT16_MAX + 10)));
        }
        {
            BigInt a(300);
            BigInt b(UINT16_MAX + 10);
            b *= a;
            EXPECT_EQ(b, BigInt(300 * (UINT16_MAX + 10)));
        }
        {
            BigInt a(UINT16_MAX + 10);
            BigInt b(300);
            b *= a;
            EXPECT_EQ(b, BigInt(300 * (UINT16_MAX + 10)));
        }
    }
    {
        {
            BigInt a(-13);
            BigInt b(15);
            b *= a;
            EXPECT_EQ(b, BigInt(-13 * 15));
        }
        {
            BigInt a(-13);
            BigInt b(47);
            b *= a;
            EXPECT_EQ(b, BigInt(-13 * 47));
        }
        {
            BigInt a(-13);
            BigInt b(276);
            b *= a;
            EXPECT_EQ(b, BigInt(-13 * 276));
        }
        {
            BigInt a(-276);
            BigInt b(13);
            b *= a;
            EXPECT_EQ(b, BigInt(-13 * 276));
        }
        {
            BigInt a(-UINT16_MAX - 10);
            BigInt b(200);
            b *= a;
            EXPECT_EQ(b, BigInt(-200 * (UINT16_MAX + 10)));
        }
        {
            BigInt a(-200);
            BigInt b(UINT16_MAX + 10);
            b *= a;
            EXPECT_EQ(b, BigInt(-200 * (UINT16_MAX + 10)));
        }
        {
            BigInt a(-300);
            BigInt b(UINT16_MAX + 10);
            b *= a;
            EXPECT_EQ(b, BigInt(-300 * (UINT16_MAX + 10)));
        }
        {
            BigInt a(-UINT16_MAX - 10);
            BigInt b(300);
            b *= a;
            EXPECT_EQ(b, BigInt(-300 * (UINT16_MAX + 10)));
        }
    }
}

TEST(OperatorsWithAssignment, Add)

{
    {
        for (size_t i = 1; i < UINT32_MAX; i<<=1)
        {
            for (size_t k = 0; k < 4; k++) {
                int compare = static_cast<int>(i + k - 1);
                BigInt b(compare);
                b += ZERO;
                EXPECT_EQ(b, b);
            }
        }
    }
    {
        {
            BigInt a(13);
            BigInt b(15);
            b += a;
            EXPECT_EQ(b, BigInt(13+15));
        }
        {
            BigInt a(130);
            BigInt b(178);
            b += a;
            EXPECT_EQ(b, BigInt(130 + 178));
        }
        {
            BigInt a(13);
            BigInt b(276);
            b += a;
            EXPECT_EQ(b, BigInt(13 + 276));
        }
        {
            BigInt a(276);
            BigInt b(13);
            b += a;
            EXPECT_EQ(b, BigInt(13 + 276));
        }
        {
            BigInt a(UINT16_MAX + 10);
            BigInt b(200);
            b += a;
            EXPECT_EQ(b, BigInt(200 + (UINT16_MAX + 10)));
        }
        {
            BigInt a(200);
            BigInt b(UINT16_MAX + 10);
            b += a;
            EXPECT_EQ(b, BigInt(200 + (UINT16_MAX + 10)));
        }
        {
            BigInt a(300);
            BigInt b(UINT16_MAX + 10);
            b += a;
            EXPECT_EQ(b, BigInt(300 + (UINT16_MAX + 10)));
        }
        {
            BigInt a(UINT16_MAX + 10);
            BigInt b(300);
            b += a;
            EXPECT_EQ(b, BigInt(300 + (UINT16_MAX + 10)));
        }
    }
    {
        {
            BigInt a(-13);
            BigInt b(15);
            b += a;
            EXPECT_EQ(b, BigInt(-13 + 15));
        }
        {
            BigInt a(15);
            BigInt b(-13);
            b += a;
            EXPECT_EQ(b, BigInt(-13 + 15));
        }
        {
            BigInt a(-47);
            BigInt b(13);
            b += a;
            EXPECT_EQ(b, BigInt(13 - 47));
        }
        {
            BigInt a(13);
            BigInt b(-47);
            b += a;
            EXPECT_EQ(b, BigInt(13 - 47));
        }
        {
            BigInt a(-50);
            BigInt b(276);
            b += a;
            EXPECT_EQ(b, BigInt(-50 + 276));
        }
        {
            BigInt a(276);
            BigInt b(-50);
            b += a;
            EXPECT_EQ(b, BigInt(-50 + 276));
        }
        {
            BigInt a(-276);
            BigInt b(50);
            b += a;
            EXPECT_EQ(b, BigInt(50 - 276));
        }
        {
            BigInt a(50);
            BigInt b(-276);
            b += a;
            EXPECT_EQ(b, BigInt(50 - 276));
        }
        {
            BigInt a(-UINT16_MAX - 10);
            BigInt b(200);
            b += a;
            EXPECT_EQ(b, BigInt(200 - (UINT16_MAX + 10)));
        }
        {
            BigInt a(200);
            BigInt b(-UINT16_MAX - 10);
            b += a;
            EXPECT_EQ(b, BigInt(200 - (UINT16_MAX + 10)));
        }
        {
            BigInt a(-200);
            BigInt b(UINT16_MAX + 10);
            b += a;
            EXPECT_EQ(b, BigInt(-200 + (UINT16_MAX + 10)));
        }
        {
            BigInt a(UINT16_MAX + 10);
            BigInt b(-200);
            b += a;
            EXPECT_EQ(b, BigInt(-200 + (UINT16_MAX + 10)));
        }
        {
            BigInt a(-300);
            BigInt b(UINT16_MAX + 10);
            b += a;
            EXPECT_EQ(b, BigInt(-300 + (UINT16_MAX + 10)));
        }
        {
            BigInt a(-UINT16_MAX - 10);
            BigInt b(300);
            b += a;
            EXPECT_EQ(b, BigInt(300 - (UINT16_MAX + 10)));
        }
    }
}

TEST(BitsOperators, Invert)
{
    EXPECT_EQ(~ZERO, BigInt(~0));
    EXPECT_EQ(~ONE,  BigInt(~1));
    EXPECT_EQ(~BigInt(108), BigInt(~108));
    EXPECT_EQ(~BigInt(255), BigInt(~255));
    EXPECT_EQ(~BigInt(300), BigInt(~300));
    EXPECT_EQ(~BigInt(INT16_MAX),       BigInt(~INT16_MAX));
    EXPECT_EQ(~BigInt(INT32_MAX - 100), BigInt(~(INT32_MAX - 100)));

    EXPECT_EQ(~BigInt(-1),  BigInt(~(-1)));
    EXPECT_EQ(~BigInt(-108), BigInt(~(-108)));
    EXPECT_EQ(~BigInt(-255), BigInt(~(-255)));
    EXPECT_EQ(~BigInt(-300), BigInt(~(-300)));
    EXPECT_EQ(~BigInt(-INT16_MAX),       BigInt(~(-INT16_MAX)));
    EXPECT_EQ(~BigInt(-INT32_MAX + 100), BigInt(~(-INT32_MAX + 100)));
}

TEST(Increments, PrefixIncrement)
{
    EXPECT_EQ(++BigInt(20), BigInt(21));
    EXPECT_EQ(++BigInt(-1), ZERO);
    EXPECT_EQ(++BigInt(-256), BigInt(-255));
    EXPECT_EQ(++BigInt(255),  BigInt(256));
}

TEST(UnaryOperators, Minus)
{
    EXPECT_EQ(-ZERO, ZERO);
    EXPECT_EQ(-ONE, BigInt(-1));
    EXPECT_EQ(-BigInt(-1), ONE);
    EXPECT_EQ(-BigInt(250), BigInt(-250));
    EXPECT_EQ(-BigInt(-250), BigInt(250));
    EXPECT_EQ(-BigInt(-1000), BigInt(1000));
    EXPECT_EQ(-BigInt(1000), BigInt(-1000));
}

TEST(Constructors, StringConstructor)
{
    EXPECT_EQ(BigInt("0"), BigInt(0));
    EXPECT_EQ(BigInt("00000000000"), BigInt(0));
    EXPECT_EQ(BigInt("-000000000000000000"), BigInt(0));
    EXPECT_EQ(BigInt("+0000000000"), BigInt("0000000000"));
    EXPECT_EQ(BigInt("-00000000"), BigInt("000000000000"));

    EXPECT_EQ(BigInt("1"), BigInt(1));
    EXPECT_EQ(BigInt("0000000000001"), BigInt(1));
    EXPECT_EQ(BigInt("-0000000000001"), BigInt(-1));
    EXPECT_EQ(BigInt("+000000001"), BigInt(1));

    EXPECT_EQ(BigInt("000255"),   BigInt(UINT8_MAX));
    EXPECT_EQ(BigInt("-0000128"), BigInt(INT8_MIN));
    EXPECT_EQ(BigInt(std::to_string(INT32_MAX)), BigInt(INT32_MAX));
    EXPECT_EQ(BigInt(std::to_string(INT32_MIN)), BigInt(INT32_MIN));
    {
        BigInt buf(std::vector<uchar>({155, 255, 255, 255, 255, 255, 255, 255, 0}));
        buf.purgeRadix();
        EXPECT_EQ(BigInt(std::to_string(UINT64_MAX - 100)), buf);
    }

    EXPECT_THROW(BigInt("-"), std::invalid_argument);
    EXPECT_THROW(BigInt("+"), std::invalid_argument);

    EXPECT_THROW(BigInt("--"), std::invalid_argument);
    EXPECT_THROW(BigInt("-+"), std::invalid_argument);
    EXPECT_THROW(BigInt("+-"), std::invalid_argument);
    EXPECT_THROW(BigInt("++"), std::invalid_argument);

    EXPECT_THROW(BigInt("a"),        std::invalid_argument);
    EXPECT_THROW(BigInt("-a"),       std::invalid_argument);
    EXPECT_THROW(BigInt("+a"),       std::invalid_argument);
    EXPECT_THROW(BigInt("-100a"),    std::invalid_argument);
    EXPECT_THROW(BigInt("+100a"),    std::invalid_argument);
    EXPECT_THROW(BigInt("100a"),     std::invalid_argument);
    EXPECT_THROW(BigInt("300a311"),  std::invalid_argument);
    EXPECT_THROW(BigInt("-300a311"), std::invalid_argument);
    EXPECT_THROW(BigInt("+300a311"), std::invalid_argument);
    EXPECT_THROW(BigInt("a499"),     std::invalid_argument);
    EXPECT_THROW(BigInt("-a499"),    std::invalid_argument);
    EXPECT_THROW(BigInt("+a487"),    std::invalid_argument);

    EXPECT_THROW(BigInt("!"),        std::invalid_argument);
    EXPECT_THROW(BigInt("-!"),       std::invalid_argument);
    EXPECT_THROW(BigInt("+!"),       std::invalid_argument);
    EXPECT_THROW(BigInt("-100!"),    std::invalid_argument);
    EXPECT_THROW(BigInt("+100!"),    std::invalid_argument);
    EXPECT_THROW(BigInt("100!"),     std::invalid_argument);
    EXPECT_THROW(BigInt("300!311"),  std::invalid_argument);
    EXPECT_THROW(BigInt("-300!311"), std::invalid_argument);
    EXPECT_THROW(BigInt("+300!311"), std::invalid_argument);
    EXPECT_THROW(BigInt("!499"),     std::invalid_argument);
    EXPECT_THROW(BigInt("-!499"),    std::invalid_argument);
    EXPECT_THROW(BigInt("+!487"),    std::invalid_argument);
}

TEST(Constructors, CopyConstructor)
{
    EXPECT_EQ(BigInt(), BigInt(BigInt()));
    EXPECT_EQ(ONE, BigInt(ONE));
    EXPECT_EQ(ZERO, BigInt(-ZERO));
    EXPECT_EQ(BigInt(120), BigInt(BigInt(120)));
    EXPECT_EQ(BigInt(-120), BigInt(BigInt(-120)));
    EXPECT_EQ(BigInt(370), BigInt(BigInt(370)));
    EXPECT_EQ(BigInt(-370), BigInt(BigInt(-370)));
}

TEST(Constructors, MoveConstructor)
{
    EXPECT_EQ(BigInt(), BigInt(std::move(BigInt())));
    EXPECT_EQ(ONE, BigInt(std::move(BigInt(1))));
    EXPECT_EQ(ZERO, BigInt(std::move(-ZERO)));
    EXPECT_EQ(BigInt(120), BigInt(std::move(BigInt(120))));
    EXPECT_EQ(BigInt(-120), BigInt(std::move(BigInt(-120))));
    EXPECT_EQ(BigInt(370), BigInt(std::move(BigInt(370))));
    EXPECT_EQ(BigInt(-370), BigInt(std::move(BigInt(-370))));
}

TEST(Increments, PostfixIncrement)
{
    {
        BigInt a(20);
        EXPECT_EQ(a++, BigInt(20));
        EXPECT_EQ(a, BigInt(21));
    }
    {
        BigInt a(-1);
        EXPECT_EQ(a++, BigInt(-1));
        EXPECT_EQ(a, ZERO);
    }
    {
        BigInt a(-256);
        EXPECT_EQ(a++, BigInt(-256));
        EXPECT_EQ(a, BigInt(-255));
    }
    {
        BigInt a(255);
        EXPECT_EQ(a++, BigInt(255));
        EXPECT_EQ(a, BigInt(256));
    }

}

TEST(Decrements, PrefixDecrement)
{
    EXPECT_EQ(--BigInt(21), BigInt(20));
    EXPECT_EQ(--BigInt(1), ZERO);
    EXPECT_EQ(--BigInt(256), BigInt(255));
    EXPECT_EQ(--BigInt(-255),  BigInt(-256));
}

TEST(Increments, PostfixDecrement)
{
    {
        BigInt a(21);
        EXPECT_EQ(a--, BigInt(21));
        EXPECT_EQ(a, BigInt(20));
    }
    {
        BigInt a(ONE);
        EXPECT_EQ(a--, ONE);
        EXPECT_EQ(a, ZERO);
    }
    {
        BigInt a(256);
        EXPECT_EQ(a--, BigInt(256));
        EXPECT_EQ(a, BigInt(255));
    }
    {
        BigInt a(-255);
        EXPECT_EQ(a--, BigInt(-255));
        EXPECT_EQ(a, BigInt(-256));
    }

}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
