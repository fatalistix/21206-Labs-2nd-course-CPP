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
    {
        BigInt num(std::vector<uchar>{255, 250});
        num.addRadix();
        EXPECT_EQ(std::vector<uchar>({255, 250, 255}), num.getArray());
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

TEST(Assignments, CopyAssignment)
{
    BigInt a;
    EXPECT_EQ(-ZERO, a =  ZERO);
    EXPECT_EQ( ONE,  a =  ONE);
    EXPECT_EQ(-ONE,  a = -ONE);
    {
        BigInt b;
        EXPECT_EQ(b, a = b);
    }
    {
        BigInt b(120);
        EXPECT_EQ(b, a = b);
    }
    {
        BigInt b(255);
        EXPECT_EQ(b, a = b);
    }
    {
        BigInt b(256);
        EXPECT_EQ(b, a = b);
    }
    {
        BigInt b(370);
        EXPECT_EQ(b, a = b);
    }
    {
        BigInt b(-120);
        EXPECT_EQ(b, a = b);
    }
    {
        BigInt b(-255);
        EXPECT_EQ(b, a = b);
    }
    {
        BigInt b(-256);
        EXPECT_EQ(b, a = b);
    }
    {
        BigInt b(-370);
        EXPECT_EQ(b, a = b);
    }
}

TEST(Assignments, MoveAssignment)
{
    BigInt a;
    EXPECT_EQ(-ZERO,     a = std::move(BigInt( 0)));
    EXPECT_EQ( ONE,      a = std::move(BigInt( 1)));
    EXPECT_EQ(-ONE,      a = std::move(BigInt(-1)));
    EXPECT_EQ( BigInt(), a = std::move(BigInt(  )));
    {
        BigInt b(120);
        EXPECT_EQ(BigInt(120), a = std::move(b));
    }
    {
        BigInt b(255);
        EXPECT_EQ(BigInt(255), a = std::move(b));
    }
    {
        BigInt b(256);
        EXPECT_EQ(BigInt(256), a = std::move(b));
    }
    {
        BigInt b(370);
        EXPECT_EQ(BigInt(370), a = std::move(b));
    }
    {
        BigInt b(-120);
        EXPECT_EQ(BigInt(-120), a = std::move(b));
    }
    {
        BigInt b(-255);
        EXPECT_EQ(BigInt(-255), a = std::move(b));
    }
    {
        BigInt b(-256);
        EXPECT_EQ(BigInt(-256), a = std::move(b));
    }
    {
        BigInt b(-370);
        EXPECT_EQ(BigInt(-370), a = std::move(b));
    }
}

TEST(BitsOperators, Invert)
{
    EXPECT_EQ(~ZERO, BigInt(~0));

    EXPECT_EQ(~ONE,                      BigInt(~ 1));
    EXPECT_EQ(~BigInt(108),              BigInt(~ 108));
    EXPECT_EQ(~BigInt(255),              BigInt(~ 255));
    EXPECT_EQ(~BigInt(256),              BigInt(~ 256));
    EXPECT_EQ(~BigInt(300),              BigInt(~ 300));
    EXPECT_EQ(~BigInt(INT16_MAX),        BigInt(~ INT16_MAX));
    EXPECT_EQ(~BigInt(UINT16_MAX),       BigInt(~ UINT16_MAX));
    EXPECT_EQ(~BigInt(UINT16_MAX + 1),   BigInt(~(UINT16_MAX + 1)));
    EXPECT_EQ(~BigInt(INT32_MAX - 100),  BigInt(~(INT32_MAX - 100)));

    EXPECT_EQ(~BigInt(-1),               BigInt(~(-1)));
    EXPECT_EQ(~BigInt(-108),             BigInt(~(-108)));
    EXPECT_EQ(~BigInt(-255),             BigInt(~(-255)));
    EXPECT_EQ(~BigInt(-256),             BigInt(~(-256)));
    EXPECT_EQ(~BigInt(-300),             BigInt(~(-300)));
    EXPECT_EQ(~BigInt(-INT16_MAX),       BigInt(~(-INT16_MAX)));
    EXPECT_EQ(~BigInt(-UINT16_MAX),      BigInt(~(-UINT16_MAX)));
    EXPECT_EQ(~BigInt(-UINT16_MAX - 1),  BigInt(~(-UINT16_MAX - 1)));
    EXPECT_EQ(~BigInt(-INT32_MAX + 100), BigInt(~(-INT32_MAX + 100)));
}

TEST(UnaryOperators, Minus)
{
    EXPECT_EQ(-ZERO, ZERO);

    EXPECT_EQ(-ONE,                     BigInt(-1));
    EXPECT_EQ(-BigInt(250),             BigInt(-250));
    EXPECT_EQ(-BigInt(255),             BigInt(-255));
    EXPECT_EQ(-BigInt(256),             BigInt(-256));
    EXPECT_EQ(-BigInt(1000),            BigInt(-1000));
    EXPECT_EQ(-BigInt(UINT16_MAX),      BigInt(-UINT16_MAX));
    EXPECT_EQ(-BigInt(UINT16_MAX + 1),  BigInt(-(UINT16_MAX + 1)));

    EXPECT_EQ(-BigInt(-1),              ONE);
    EXPECT_EQ(-BigInt(-250),            BigInt(250));
    EXPECT_EQ(-BigInt(-255),            BigInt(255));
    EXPECT_EQ(-BigInt(-256),            BigInt(256));
    EXPECT_EQ(-BigInt(-1000),           BigInt(1000));
    EXPECT_EQ(-BigInt(-UINT16_MAX),     BigInt(-(-UINT16_MAX)));
    EXPECT_EQ(-BigInt(-UINT16_MAX - 1), BigInt(-(-UINT16_MAX - 1)));
}

TEST(Constructors, CopyConstructor)
{
    EXPECT_EQ(BigInt(),     BigInt(BigInt()));
    EXPECT_EQ(ZERO,         BigInt(-ZERO));

    EXPECT_EQ(ONE,          BigInt(ONE));
    EXPECT_EQ(BigInt(120),  BigInt(BigInt(120)));
    EXPECT_EQ(BigInt(255),  BigInt(BigInt(255)));
    EXPECT_EQ(BigInt(256),  BigInt(BigInt(256)));
    EXPECT_EQ(BigInt(370),  BigInt(BigInt(370)));

    EXPECT_EQ(-ONE,         BigInt(-ONE));
    EXPECT_EQ(BigInt(-120), BigInt(BigInt(-120)));
    EXPECT_EQ(BigInt(-255), BigInt(BigInt(-255)));
    EXPECT_EQ(BigInt(-256), BigInt(BigInt(-256)));
    EXPECT_EQ(BigInt(-370), BigInt(BigInt(-370)));
}

TEST(MathOperators, Mul)
{
    {
        EXPECT_EQ(ZERO *  ZERO,           ZERO);
        EXPECT_EQ(ZERO *  ONE,            ZERO);
        EXPECT_EQ(ZERO *  BigInt(100),    ZERO);
        EXPECT_EQ(ZERO *  BigInt(255),    ZERO);
        EXPECT_EQ(ZERO *  BigInt(256),    ZERO);
        EXPECT_EQ(ZERO *  BigInt(1000),   ZERO);
        EXPECT_EQ(ZERO *  BigInt(300000), ZERO);

        EXPECT_EQ(ZERO * -ZERO,           ZERO);
        EXPECT_EQ(ZERO * -ONE,            ZERO);
        EXPECT_EQ(ZERO * -BigInt(100),    ZERO);
        EXPECT_EQ(ZERO * -BigInt(255),    ZERO);
        EXPECT_EQ(ZERO * -BigInt(256),    ZERO);
        EXPECT_EQ(ZERO * -BigInt(1000),   ZERO);
        EXPECT_EQ(ZERO * -BigInt(300000), ZERO);
    }
    {
        EXPECT_EQ( ONE *  ZERO,            ZERO);
        EXPECT_EQ( ONE *  ONE,             ONE);
        EXPECT_EQ( ONE *  BigInt(100),     BigInt(100));
        EXPECT_EQ( ONE *  BigInt(255),     BigInt(255));
        EXPECT_EQ( ONE *  BigInt(256),     BigInt(256));
        EXPECT_EQ( ONE *  BigInt(1000),    BigInt(1000));
        EXPECT_EQ( ONE *  BigInt(300000),  BigInt(300000));

        EXPECT_EQ( ONE * -ZERO,            ZERO);
        EXPECT_EQ( ONE * -ONE,            -ONE);
        EXPECT_EQ( ONE * -BigInt(100),    -BigInt(100));
        EXPECT_EQ( ONE * -BigInt(255),    -BigInt(255));
        EXPECT_EQ( ONE * -BigInt(256),    -BigInt(256));
        EXPECT_EQ( ONE * -BigInt(1000),   -BigInt(1000));
        EXPECT_EQ( ONE * -BigInt(300000), -BigInt(300000));

        EXPECT_EQ(-ONE *  ZERO,           -ZERO);
        EXPECT_EQ(-ONE *  ONE,            -ONE);
        EXPECT_EQ(-ONE *  BigInt(100),    -BigInt(100));
        EXPECT_EQ(-ONE *  BigInt(255),    -BigInt(255));
        EXPECT_EQ(-ONE *  BigInt(256),    -BigInt(256));
        EXPECT_EQ(-ONE *  BigInt(1000),   -BigInt(1000));
        EXPECT_EQ(-ONE *  BigInt(300000), -BigInt(300000));

        EXPECT_EQ(-ONE * -ZERO,            ZERO);
        EXPECT_EQ(-ONE * -ONE,             ONE);
        EXPECT_EQ(-ONE * -BigInt(100),     BigInt(100));
        EXPECT_EQ(-ONE * -BigInt(255),     BigInt(255));
        EXPECT_EQ(-ONE * -BigInt(256),     BigInt(256));
        EXPECT_EQ(-ONE * -BigInt(1000),    BigInt(1000));
        EXPECT_EQ(-ONE * -BigInt(300000),  BigInt(300000));
    }
    {
        EXPECT_EQ( BigInt(100) *  ZERO,            ZERO);
        EXPECT_EQ( BigInt(100) *  ONE,             BigInt(100));
        EXPECT_EQ( BigInt(100) *  BigInt(37),      BigInt(3700));
        EXPECT_EQ( BigInt(100) *  BigInt(100),     BigInt(10000));
        EXPECT_EQ( BigInt(100) *  BigInt(255),     BigInt(25500));
        EXPECT_EQ( BigInt(100) *  BigInt(256),     BigInt(25600));
        EXPECT_EQ( BigInt(100) *  BigInt(1000),    BigInt(100000));
        EXPECT_EQ( BigInt(100) *  BigInt(300000),  BigInt(30000000));

        EXPECT_EQ( BigInt(100) * -ZERO,            ZERO);
        EXPECT_EQ( BigInt(100) * -ONE,            -BigInt(100));
        EXPECT_EQ( BigInt(100) * -BigInt(37),     -BigInt(3700));
        EXPECT_EQ( BigInt(100) * -BigInt(100),    -BigInt(10000));
        EXPECT_EQ( BigInt(100) * -BigInt(255),    -BigInt(25500));
        EXPECT_EQ( BigInt(100) * -BigInt(256),    -BigInt(25600));
        EXPECT_EQ( BigInt(100) * -BigInt(1000),   -BigInt(100000));
        EXPECT_EQ( BigInt(100) * -BigInt(300000), -BigInt(30000000));

        EXPECT_EQ(-BigInt(100) *  ZERO,            ZERO);
        EXPECT_EQ(-BigInt(100) *  ONE,            -BigInt(100));
        EXPECT_EQ(-BigInt(100) *  BigInt(37),     -BigInt(3700));
        EXPECT_EQ(-BigInt(100) *  BigInt(100),    -BigInt(10000));
        EXPECT_EQ(-BigInt(100) *  BigInt(255),    -BigInt(25500));
        EXPECT_EQ(-BigInt(100) *  BigInt(256),    -BigInt(25600));
        EXPECT_EQ(-BigInt(100) *  BigInt(1000),   -BigInt(100000));
        EXPECT_EQ(-BigInt(100) *  BigInt(300000), -BigInt(30000000));

        EXPECT_EQ(-BigInt(100) * -ZERO,            ZERO);
        EXPECT_EQ(-BigInt(100) * -ONE,             BigInt(100));
        EXPECT_EQ(-BigInt(100) * -BigInt(37),      BigInt(3700));
        EXPECT_EQ(-BigInt(100) * -BigInt(100),     BigInt(10000));
        EXPECT_EQ(-BigInt(100) * -BigInt(255),     BigInt(25500));
        EXPECT_EQ(-BigInt(100) * -BigInt(256),     BigInt(25600));
        EXPECT_EQ(-BigInt(100) * -BigInt(1000),    BigInt(100000));
        EXPECT_EQ(-BigInt(100) * -BigInt(300000),  BigInt(30000000));
    }
    {
        EXPECT_EQ( BigInt(1000) *  ZERO,            ZERO);
        EXPECT_EQ( BigInt(1000) *  ONE,             BigInt(1000));
        EXPECT_EQ( BigInt(1000) *  BigInt(100),     BigInt(100000));
        EXPECT_EQ( BigInt(1000) *  BigInt(255),     BigInt(255000));
        EXPECT_EQ( BigInt(1000) *  BigInt(256),     BigInt(256000));
        EXPECT_EQ( BigInt(1000) *  BigInt(786),     BigInt(786000));
        EXPECT_EQ( BigInt(1000) *  BigInt(1000),    BigInt(1000000));
        EXPECT_EQ( BigInt(1000) *  BigInt(5678),    BigInt(5678000));
        EXPECT_EQ( BigInt(1000) *  BigInt(300000),  BigInt(300000000));

        EXPECT_EQ(-BigInt(1000) *  ZERO,            ZERO);
        EXPECT_EQ(-BigInt(1000) *  ONE,            -BigInt(1000));
        EXPECT_EQ(-BigInt(1000) *  BigInt(100),    -BigInt(100000));
        EXPECT_EQ(-BigInt(1000) *  BigInt(255),    -BigInt(255000));
        EXPECT_EQ(-BigInt(1000) *  BigInt(256),    -BigInt(256000));
        EXPECT_EQ(-BigInt(1000) *  BigInt(786),    -BigInt(786000));
        EXPECT_EQ(-BigInt(1000) *  BigInt(1000),   -BigInt(1000000));
        EXPECT_EQ(-BigInt(1000) *  BigInt(5678),   -BigInt(5678000));
        EXPECT_EQ(-BigInt(1000) *  BigInt(300000), -BigInt(300000000));

        EXPECT_EQ( BigInt(1000) * -ZERO,            ZERO);
        EXPECT_EQ( BigInt(1000) * -ONE,            -BigInt(1000));
        EXPECT_EQ( BigInt(1000) * -BigInt(100),    -BigInt(100000));
        EXPECT_EQ( BigInt(1000) * -BigInt(255),    -BigInt(255000));
        EXPECT_EQ( BigInt(1000) * -BigInt(256),    -BigInt(256000));
        EXPECT_EQ( BigInt(1000) * -BigInt(786),    -BigInt(786000));
        EXPECT_EQ( BigInt(1000) * -BigInt(1000),   -BigInt(1000000));
        EXPECT_EQ( BigInt(1000) * -BigInt(5678),   -BigInt(5678000));
        EXPECT_EQ( BigInt(1000) * -BigInt(300000), -BigInt(300000000));

        EXPECT_EQ(-BigInt(1000) * -ZERO,            ZERO);
        EXPECT_EQ(-BigInt(1000) * -ONE,             BigInt(1000));
        EXPECT_EQ(-BigInt(1000) * -BigInt(100),     BigInt(100000));
        EXPECT_EQ(-BigInt(1000) * -BigInt(255),     BigInt(255000));
        EXPECT_EQ(-BigInt(1000) * -BigInt(256),     BigInt(256000));
        EXPECT_EQ(-BigInt(1000) * -BigInt(786),     BigInt(786000));
        EXPECT_EQ(-BigInt(1000) * -BigInt(1000),    BigInt(1000000));
        EXPECT_EQ(-BigInt(1000) * -BigInt(5678),    BigInt(5678000));
        EXPECT_EQ(-BigInt(1000) * -BigInt(300000),  BigInt(300000000));
    }
}

TEST(MathOperators, Add)
{
    {
        EXPECT_EQ(ZERO +  ZERO,           ZERO);
        EXPECT_EQ(ZERO +  ONE,            ONE);
        EXPECT_EQ(ZERO +  BigInt(100),    BigInt(100));
        EXPECT_EQ(ZERO +  BigInt(255),    BigInt(255));
        EXPECT_EQ(ZERO +  BigInt(256),    BigInt(256));
        EXPECT_EQ(ZERO +  BigInt(1000),   BigInt(1000));
        EXPECT_EQ(ZERO +  BigInt(300000), BigInt(300000));

        EXPECT_EQ(ZERO + -ZERO,           ZERO);
        EXPECT_EQ(ZERO + -ONE,            -ONE);
        EXPECT_EQ(ZERO + -BigInt(100),    -BigInt(100));
        EXPECT_EQ(ZERO + -BigInt(255),    -BigInt(255));
        EXPECT_EQ(ZERO + -BigInt(256),    -BigInt(256));
        EXPECT_EQ(ZERO + -BigInt(1000),   -BigInt(1000));
        EXPECT_EQ(ZERO + -BigInt(300000), -BigInt(300000));
    }
    {
        EXPECT_EQ( ONE +  ZERO,            ONE);
        EXPECT_EQ( ONE +  ONE,             BigInt(2));
        EXPECT_EQ( ONE +  BigInt(100),     BigInt(101));
        EXPECT_EQ( ONE +  BigInt(255),     BigInt(256));
        EXPECT_EQ( ONE +  BigInt(256),     BigInt(257));
        EXPECT_EQ( ONE +  BigInt(1000),    BigInt(1001));
        EXPECT_EQ( ONE +  BigInt(300000),  BigInt(300001));

        EXPECT_EQ( ONE + -ZERO,            ONE);
        EXPECT_EQ( ONE + -ONE,            -ZERO);
        EXPECT_EQ( ONE + -BigInt(100),    -BigInt(99));
        EXPECT_EQ( ONE + -BigInt(255),    -BigInt(254));
        EXPECT_EQ( ONE + -BigInt(256),    -BigInt(255));
        EXPECT_EQ( ONE + -BigInt(1000),   -BigInt(999));
        EXPECT_EQ( ONE + -BigInt(300000), -BigInt(299999));

        EXPECT_EQ(-ONE +  ZERO,           -ONE);
        EXPECT_EQ(-ONE +  ONE,             ZERO);
        EXPECT_EQ(-ONE +  BigInt(100),     BigInt(99));
        EXPECT_EQ(-ONE +  BigInt(255),     BigInt(254));
        EXPECT_EQ(-ONE +  BigInt(256),     BigInt(255));
        EXPECT_EQ(-ONE +  BigInt(1000),    BigInt(999));
        EXPECT_EQ(-ONE +  BigInt(300000),  BigInt(299999));

        EXPECT_EQ(-ONE + -ZERO,           -ONE);
        EXPECT_EQ(-ONE + -ONE,            -BigInt(2));
        EXPECT_EQ(-ONE + -BigInt(100),    -BigInt(101));
        EXPECT_EQ(-ONE + -BigInt(255),    -BigInt(256));
        EXPECT_EQ(-ONE + -BigInt(256),    -BigInt(257));
        EXPECT_EQ(-ONE + -BigInt(1000),   -BigInt(1001));
        EXPECT_EQ(-ONE + -BigInt(300000), -BigInt(300001));
    }
    {
        EXPECT_EQ( BigInt(100) +  ZERO,            BigInt(100));
        EXPECT_EQ( BigInt(100) +  ONE,             BigInt(101));
        EXPECT_EQ( BigInt(100) +  BigInt(37),      BigInt(137));
        EXPECT_EQ( BigInt(100) +  BigInt(100),     BigInt(200));
        EXPECT_EQ( BigInt(100) +  BigInt(255),     BigInt(355));
        EXPECT_EQ( BigInt(100) +  BigInt(256),     BigInt(356));
        EXPECT_EQ( BigInt(100) +  BigInt(1000),    BigInt(1100));
        EXPECT_EQ( BigInt(100) +  BigInt(300000),  BigInt(300100));

        EXPECT_EQ( BigInt(100) + -ZERO,            BigInt(100));
        EXPECT_EQ( BigInt(100) + -ONE,             BigInt(99));
        EXPECT_EQ( BigInt(100) + -BigInt(37),      BigInt(63));
        EXPECT_EQ( BigInt(100) + -BigInt(100),     ZERO);
        EXPECT_EQ( BigInt(100) + -BigInt(255),    -BigInt(155));
        EXPECT_EQ( BigInt(100) + -BigInt(256),    -BigInt(156));
        EXPECT_EQ( BigInt(100) + -BigInt(1000),   -BigInt(900));
        EXPECT_EQ( BigInt(100) + -BigInt(300000), -BigInt(299900));

        EXPECT_EQ(-BigInt(100) +  ZERO,           -BigInt(100));
        EXPECT_EQ(-BigInt(100) +  ONE,            -BigInt(99));
        EXPECT_EQ(-BigInt(100) +  BigInt(37),     -BigInt(63));
        EXPECT_EQ(-BigInt(100) +  BigInt(100),     ZERO);
        EXPECT_EQ(-BigInt(100) +  BigInt(255),     BigInt(155));
        EXPECT_EQ(-BigInt(100) +  BigInt(256),     BigInt(156));
        EXPECT_EQ(-BigInt(100) +  BigInt(1000),    BigInt(900));
        EXPECT_EQ(-BigInt(100) +  BigInt(300000),  BigInt(299900));

        EXPECT_EQ(-BigInt(100) + -ZERO,           -BigInt(100));
        EXPECT_EQ(-BigInt(100) + -ONE,            -BigInt(101));
        EXPECT_EQ(-BigInt(100) + -BigInt(37),     -BigInt(137));
        EXPECT_EQ(-BigInt(100) + -BigInt(100),    -BigInt(200));
        EXPECT_EQ(-BigInt(100) + -BigInt(255),    -BigInt(355));
        EXPECT_EQ(-BigInt(100) + -BigInt(256),    -BigInt(356));
        EXPECT_EQ(-BigInt(100) + -BigInt(1000),   -BigInt(1100));
        EXPECT_EQ(-BigInt(100) + -BigInt(300000), -BigInt(300100));
    }
    {
        EXPECT_EQ( BigInt(1000) +  ZERO,            BigInt(1000));
        EXPECT_EQ( BigInt(1000) +  ONE,             BigInt(1001));
        EXPECT_EQ( BigInt(1000) +  BigInt(100),     BigInt(1100));
        EXPECT_EQ( BigInt(1000) +  BigInt(255),     BigInt(1255));
        EXPECT_EQ( BigInt(1000) +  BigInt(256),     BigInt(1256));
        EXPECT_EQ( BigInt(1000) +  BigInt(786),     BigInt(1786));
        EXPECT_EQ( BigInt(1000) +  BigInt(1000),    BigInt(2000));
        EXPECT_EQ( BigInt(1000) +  BigInt(5678),    BigInt(6678));
        EXPECT_EQ( BigInt(1000) +  BigInt(300000),  BigInt(301000));

        EXPECT_EQ(-BigInt(1000) +  ZERO,           -BigInt(1000));
        EXPECT_EQ(-BigInt(1000) +  ONE,            -BigInt(999));
        EXPECT_EQ(-BigInt(1000) +  BigInt(100),    -BigInt(900));
        EXPECT_EQ(-BigInt(1000) +  BigInt(255),    -BigInt(745));
        EXPECT_EQ(-BigInt(1000) +  BigInt(256),    -BigInt(744));
        EXPECT_EQ(-BigInt(1000) +  BigInt(786),    -BigInt(214));
        EXPECT_EQ(-BigInt(1000) +  BigInt(1000),    ZERO);
        EXPECT_EQ(-BigInt(1000) +  BigInt(5678),    BigInt(4678));
        EXPECT_EQ(-BigInt(1000) +  BigInt(300000),  BigInt(299000));

        EXPECT_EQ( BigInt(1000) + -ZERO,            BigInt(1000));
        EXPECT_EQ( BigInt(1000) + -ONE,             BigInt(999));
        EXPECT_EQ( BigInt(1000) + -BigInt(100),     BigInt(900));
        EXPECT_EQ( BigInt(1000) + -BigInt(255),     BigInt(745));
        EXPECT_EQ( BigInt(1000) + -BigInt(256),     BigInt(744));
        EXPECT_EQ( BigInt(1000) + -BigInt(786),     BigInt(214));
        EXPECT_EQ( BigInt(1000) + -BigInt(1000),    ZERO);
        EXPECT_EQ( BigInt(1000) + -BigInt(5678),   -BigInt(4678));
        EXPECT_EQ( BigInt(1000) + -BigInt(300000), -BigInt(299000));

        EXPECT_EQ(-BigInt(1000) + -ZERO,           -BigInt(1000));
        EXPECT_EQ(-BigInt(1000) + -ONE,            -BigInt(1001));
        EXPECT_EQ(-BigInt(1000) + -BigInt(100),    -BigInt(1100));
        EXPECT_EQ(-BigInt(1000) + -BigInt(255),    -BigInt(1255));
        EXPECT_EQ(-BigInt(1000) + -BigInt(256),    -BigInt(1256));
        EXPECT_EQ(-BigInt(1000) + -BigInt(786),    -BigInt(1786));
        EXPECT_EQ(-BigInt(1000) + -BigInt(1000),   -BigInt(2000));
        EXPECT_EQ(-BigInt(1000) + -BigInt(5678),   -BigInt(6678));
        EXPECT_EQ(-BigInt(1000) + -BigInt(300000), -BigInt(301000));
    }
}

TEST(Increments, PrefixIncrement)
{
    EXPECT_EQ(++BigInt(20), BigInt(21));
    EXPECT_EQ(++BigInt(-1), ZERO);
    EXPECT_EQ(++BigInt(-256), BigInt(-255));
    EXPECT_EQ(++BigInt(255),  BigInt(256));
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

TEST(Operators, Div)
{
    EXPECT_THROW(BigInt("-123192872398473298") / ZERO, std::invalid_argument);
    {
        EXPECT_EQ(ZERO / ONE,             ZERO);
        EXPECT_EQ(ZERO / DEC,             ZERO);
        EXPECT_EQ(ZERO / BigInt(250),     ZERO);
        EXPECT_EQ(ZERO / BigInt(1000),    ZERO);
        EXPECT_EQ(ZERO / BigInt(300000),  ZERO);

        EXPECT_EQ(ZERO / -ONE,            ZERO);
        EXPECT_EQ(ZERO / -DEC,            ZERO);
        EXPECT_EQ(ZERO / BigInt(-250),    ZERO);
        EXPECT_EQ(ZERO / BigInt(-1000),   ZERO);
        EXPECT_EQ(ZERO / BigInt(-300000), ZERO);
    }
    {
        EXPECT_EQ(DEC             / ONE,   DEC);
        EXPECT_EQ(BigInt(250)     / ONE,   BigInt(250));
        EXPECT_EQ(BigInt(1000)    / ONE,   BigInt(1000));
        EXPECT_EQ(BigInt(300000)  / ONE,   BigInt(300000));

        EXPECT_EQ(-DEC            / ONE,  -DEC);
        EXPECT_EQ(-BigInt(250)    / ONE,  -BigInt(250));
        EXPECT_EQ(-BigInt(1000)   / ONE,  -BigInt(1000));
        EXPECT_EQ(-BigInt(300000) / ONE,  -BigInt(300000));

        EXPECT_EQ(DEC             / -ONE, -DEC);
        EXPECT_EQ(BigInt(250)     / -ONE, -BigInt(250));
        EXPECT_EQ(BigInt(1000)    / -ONE, -BigInt(1000));
        EXPECT_EQ(BigInt(300000)  / -ONE, -BigInt(300000));

        EXPECT_EQ(-DEC            / -ONE,  DEC);
        EXPECT_EQ(-BigInt(250)    / -ONE,  BigInt(250));
        EXPECT_EQ(-BigInt(1000)   / -ONE,  BigInt(1000));
        EXPECT_EQ(-BigInt(300000) / -ONE,  BigInt(300000));
    }
    {
        EXPECT_EQ(BigInt(135)  / BigInt(13),       BigInt(10));
        EXPECT_EQ(BigInt(135)  / BigInt(135),      ONE);
        EXPECT_EQ(BigInt(135)  / BigInt(250),      ZERO);
        EXPECT_EQ(BigInt(135)  / BigInt(1000),     ZERO);
        EXPECT_EQ(BigInt(135)  / BigInt(300000),   ZERO);

        EXPECT_EQ(BigInt(-135) / BigInt(13),      -BigInt(10));
        EXPECT_EQ(BigInt(-135) / BigInt(135),     -ONE);
        EXPECT_EQ(BigInt(-135) / BigInt(250),     -ZERO);
        EXPECT_EQ(BigInt(-135) / BigInt(1000),    -ZERO);
        EXPECT_EQ(BigInt(-135) / BigInt(300000),  -ZERO);

        EXPECT_EQ(BigInt(135)  / BigInt(-13),     -BigInt(10));
        EXPECT_EQ(BigInt(135)  / BigInt(-135),    -ONE);
        EXPECT_EQ(BigInt(135)  / BigInt(-250),    -ZERO);
        EXPECT_EQ(BigInt(135)  / BigInt(-1000),   -ZERO);
        EXPECT_EQ(BigInt(135)  / BigInt(-300000), -ZERO);

        EXPECT_EQ(BigInt(-135) / BigInt(-13),      BigInt(10));
        EXPECT_EQ(BigInt(-135) / BigInt(-135),     ONE);
        EXPECT_EQ(BigInt(-135) / BigInt(-250),     ZERO);
        EXPECT_EQ(BigInt(-135) / BigInt(-1000),    ZERO);
        EXPECT_EQ(BigInt(-135) / BigInt(-300000),  ZERO);
    }
    {
        EXPECT_EQ(BigInt(1000)  / BigInt(13), BigInt(76));
        EXPECT_EQ(BigInt(1000)  / BigInt(487), BigInt(2));
        EXPECT_EQ(BigInt(1000)  / BigInt(1000), ONE);
        EXPECT_EQ(BigInt(1000)  / BigInt(5678), ZERO);
        EXPECT_EQ(BigInt(1000)  / BigInt(300000), ZERO);

        EXPECT_EQ(BigInt(-1000) / BigInt(13), -BigInt(76));
        EXPECT_EQ(BigInt(-1000) / BigInt(487), -BigInt(2));
        EXPECT_EQ(BigInt(-1000) / BigInt(1000), -ONE);
        EXPECT_EQ(BigInt(-1000) / BigInt(5678), -ZERO);
        EXPECT_EQ(BigInt(-1000) / BigInt(300000), -ZERO);

        EXPECT_EQ(BigInt(1000)  / BigInt(-13), -BigInt(76));
        EXPECT_EQ(BigInt(1000)  / BigInt(-487), -BigInt(2));
        EXPECT_EQ(BigInt(1000)  / BigInt(-1000), -ONE);
        EXPECT_EQ(BigInt(1000)  / BigInt(-5678), -ZERO);
        EXPECT_EQ(BigInt(1000)  / BigInt(-300000), -ZERO);

        EXPECT_EQ(BigInt(-1000) / BigInt(-13), BigInt(76));
        EXPECT_EQ(BigInt(-1000) / BigInt(-487), BigInt(2));
        EXPECT_EQ(BigInt(-1000) / BigInt(-1000), ONE);
        EXPECT_EQ(BigInt(-1000) / BigInt(-5678), ZERO);
        EXPECT_EQ(BigInt(-1000) / BigInt(-300000), ZERO);
    }
}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
