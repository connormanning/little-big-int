#include "little-big-int.hpp"
#include "gtest/gtest.h"

namespace
{
    const BigUint zero;
    const BigUint one(1);

    const BigUint maxTrivial((BigUint(1) << bitsPerBlock) - 1);
    const BigUint minNontrivial((BigUint(1) << bitsPerBlock));

    const std::string twoToThe64("18446744073709551616");
    const std::string twoToThe64MinusOne("18446744073709551615");
    const std::string twoToThe128("340282366920938463463374607431768211456");
    const std::string twoToThe128MinusOne(
            "340282366920938463463374607431768211455");

    const std::string twoToThe64TimesTwo("36893488147419103232");
    const std::string twoToThe64TimesTwoMinusOne("36893488147419103231");
}

TEST(LittleBigInt, ValueConstructor)
{
    EXPECT_EQ(zero.getSimple(), 0);

    EXPECT_EQ(BigUint(0).getSimple(), 0);
    EXPECT_EQ(one.getSimple(), 1);
    EXPECT_EQ(
            BigUint(1ULL << (bitsPerBlock - 1)),
            1ULL << (bitsPerBlock - 1));
    EXPECT_EQ(BigUint(blockMax), blockMax);
    EXPECT_EQ(BigUint(-1ULL), blockMax);
}

TEST(LittleBigInt, StringConstructorTrivial)
{
    EXPECT_EQ(BigUint("0").getSimple(), 0);
    EXPECT_EQ(BigUint("1").getSimple(), 1);
    EXPECT_EQ(
            BigUint(std::to_string(1ULL << (bitsPerBlock - 1))),
            1ULL << (bitsPerBlock - 1));
    EXPECT_EQ(BigUint(std::to_string(blockMax)), blockMax);
    EXPECT_EQ(BigUint(std::to_string(-1ULL)), blockMax);
}

TEST(LittleBigInt, StringConstructorComplex)
{
    EXPECT_EQ(BigUint(twoToThe64).str(), twoToThe64);
    EXPECT_EQ((BigUint(twoToThe64) - 1).str(), twoToThe64MinusOne);

    EXPECT_EQ(BigUint(twoToThe128).str(), twoToThe128);
    EXPECT_EQ((BigUint(twoToThe128) - 1).str(), twoToThe128MinusOne);
}

TEST(LittleBigInt, Zero)
{
    EXPECT_TRUE(zero.zero());
    EXPECT_TRUE(BigUint(0).zero());

    BigUint a(2);
    EXPECT_FALSE(a.zero());
    a = 0;
    EXPECT_TRUE(a.zero());

    BigUint b(twoToThe128);
    EXPECT_FALSE(b.zero());
    b = 0;
    EXPECT_TRUE(b.zero());

    EXPECT_FALSE((zero + 1).zero());
    EXPECT_FALSE(BigUint(1).zero());
    EXPECT_FALSE(BigUint(blockMax).zero());
    EXPECT_FALSE(BigUint(twoToThe64).zero());
    EXPECT_FALSE(BigUint(twoToThe128).zero());
}

TEST(LittleBigInt, Trivial)
{
    EXPECT_TRUE(zero.trivial());
    EXPECT_TRUE(BigUint(1).trivial());

    EXPECT_TRUE(maxTrivial.trivial());
    EXPECT_FALSE(minNontrivial.trivial());
}

TEST(LittleBigInt, BlockSize)
{
    EXPECT_EQ(zero.blockSize(), 1);
    EXPECT_EQ(one.blockSize(), 1);

    EXPECT_EQ(maxTrivial.blockSize(), 1);
    EXPECT_EQ(minNontrivial.blockSize(), 2);
    EXPECT_EQ((minNontrivial - 1).blockSize(), 1);
}

TEST(LittleBigInt, PlusEquals)
{
    BigUint z;
    EXPECT_TRUE(z.zero());
    EXPECT_TRUE(z.trivial());

    z += 0;
    EXPECT_TRUE(z.zero());
    EXPECT_TRUE(z.trivial());

    z += 1;
    EXPECT_FALSE(z.zero());
    EXPECT_TRUE(z.trivial());
    EXPECT_EQ(z, one);

    z += BigUint(twoToThe64MinusOne);
    EXPECT_FALSE(z.zero());
    EXPECT_EQ(z.str(), twoToThe64);

    z += BigUint(twoToThe64MinusOne);
    EXPECT_FALSE(z.zero());
    EXPECT_EQ(z.str(), twoToThe64TimesTwoMinusOne);

    z += 1;
    EXPECT_FALSE(z.zero());
    EXPECT_EQ(z.str(), twoToThe64TimesTwo);

    z += 0;
    EXPECT_FALSE(z.zero());
    EXPECT_EQ(z.str(), twoToThe64TimesTwo);
}

TEST(LittleBigInt, MinusEquals)
{
    BigUint z(twoToThe64TimesTwo);
    EXPECT_FALSE(z.zero());
    EXPECT_FALSE(z.trivial());
    EXPECT_EQ(z.str(), twoToThe64TimesTwo);

    z -= 0;
    EXPECT_FALSE(z.zero());
    EXPECT_EQ(z.str(), twoToThe64TimesTwo);

    z -= 1;
    EXPECT_FALSE(z.zero());
    EXPECT_EQ(z.str(), twoToThe64TimesTwoMinusOne);

    z -= BigUint(twoToThe64MinusOne);
    EXPECT_FALSE(z.zero());
    EXPECT_EQ(z.str(), twoToThe64);

    z -= 1;
    EXPECT_FALSE(z.zero());
    EXPECT_EQ(z.str(), twoToThe64MinusOne);

    z -= BigUint(twoToThe64MinusOne);
    EXPECT_TRUE(z.zero());
    EXPECT_EQ(z.str(), "0");
    EXPECT_EQ(z.getSimple(), 0);

    EXPECT_THROW(z -= 1, std::underflow_error);
    EXPECT_THROW(
            BigUint(twoToThe64MinusOne) - BigUint(twoToThe64),
            std::underflow_error);
    EXPECT_THROW(
            BigUint(twoToThe64MinusOne) - BigUint(twoToThe128),
            std::underflow_error);
    EXPECT_THROW(
            BigUint(twoToThe64) - BigUint(twoToThe128),
            std::underflow_error);
    EXPECT_THROW(
            BigUint(twoToThe64) - BigUint(twoToThe128MinusOne),
            std::underflow_error);

    EXPECT_EQ(zero - zero, 0);
    EXPECT_EQ(one - one, 0);
    EXPECT_EQ(BigUint(twoToThe64) - BigUint(twoToThe64), 0);
    EXPECT_EQ(BigUint(twoToThe64MinusOne) - BigUint(twoToThe64MinusOne), 0);
    EXPECT_EQ(BigUint(twoToThe128) - BigUint(twoToThe128), 0);
    EXPECT_EQ(BigUint(twoToThe128MinusOne) - BigUint(twoToThe128MinusOne), 0);
}

TEST(LittleBigInt, TimesEquals)
{
    bool done(false);
    EXPECT_EQ(done, true);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

