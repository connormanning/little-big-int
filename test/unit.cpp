#include "little-big-int.hpp"
#include "gtest/gtest.h"

#include <map>

namespace
{
    const BigUint zero;
    const BigUint one(1);

    const BigUint maxTrivial((BigUint(1) << bitsPerBlock) - 1);
    const BigUint minNontrivial((BigUint(1) << bitsPerBlock));

    const Block toggle(2863311530); // 0b10 repeated 16 times (total 32 bits).

    const std::string twoToThe64("18446744073709551616");
    const std::string twoToThe64MinusOne("18446744073709551615");
    const std::string twoToThe128("340282366920938463463374607431768211456");
    const std::string twoToThe128MinusOne(
            "340282366920938463463374607431768211455");

    const std::string twoToThe64TimesTwo("36893488147419103232");
    const std::string twoToThe64TimesTwoMinusOne("36893488147419103231");

    const std::string bigPi(
            "3141592653589793238462643383279502884197169399375105820974944592"
            "3078164062862089986280348253421170679821480865132823066470938446"
            "09550582231725359408128481");

    const std::string bigE(
            "2718281828459045235360287471352662497757247093699959574966967627");

    const std::vector<uint64_t> testVector { 3928502 };
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
    EXPECT_EQ(BigUint(0).str(), "0");

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

    EXPECT_NO_THROW(maxTrivial.getSimple());
    EXPECT_THROW(minNontrivial.getSimple(), std::overflow_error);
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
    BigUint big;
    EXPECT_TRUE(big.zero());

    big += 0;
    EXPECT_TRUE(big.zero());

    big += 1;
    EXPECT_FALSE(big.zero());
    EXPECT_TRUE(big.trivial());
    EXPECT_EQ(big, one);

    big += BigUint(twoToThe64MinusOne);
    EXPECT_FALSE(big.zero());
    EXPECT_EQ(big.str(), twoToThe64);

    big += BigUint(twoToThe64MinusOne);
    EXPECT_FALSE(big.zero());
    EXPECT_EQ(big.str(), twoToThe64TimesTwoMinusOne);

    big += 1;
    EXPECT_FALSE(big.zero());
    EXPECT_EQ(big.str(), twoToThe64TimesTwo);

    big += 0;
    EXPECT_FALSE(big.zero());
    EXPECT_EQ(big.str(), twoToThe64TimesTwo);
}

TEST(LittleBigInt, MinusEquals)
{
    BigUint big(twoToThe64TimesTwo);
    EXPECT_FALSE(big.zero());
    EXPECT_FALSE(big.trivial());
    EXPECT_EQ(big.str(), twoToThe64TimesTwo);

    big -= 0;
    EXPECT_FALSE(big.zero());
    EXPECT_EQ(big.str(), twoToThe64TimesTwo);

    big -= 1;
    EXPECT_FALSE(big.zero());
    EXPECT_EQ(big.str(), twoToThe64TimesTwoMinusOne);

    big -= BigUint(twoToThe64MinusOne);
    EXPECT_FALSE(big.zero());
    EXPECT_EQ(big.str(), twoToThe64);

    big -= 1;
    EXPECT_FALSE(big.zero());
    EXPECT_EQ(big.str(), twoToThe64MinusOne);

    big -= BigUint(twoToThe64MinusOne);
    EXPECT_TRUE(big.zero());
    EXPECT_EQ(big.str(), "0");
    EXPECT_EQ(big.getSimple(), 0);

    EXPECT_THROW(big -= 1, std::underflow_error);
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
    BigUint big(2);

    big *= 1;
    EXPECT_EQ(big.getSimple(), 2);

    big *= 2;
    EXPECT_EQ(big.getSimple(), 4);

    big *= big;
    EXPECT_EQ(big.getSimple(), 16);

    big *= 0;
    EXPECT_EQ(big.getSimple(), 0);
    EXPECT_TRUE(big.zero());

    big = BigUint(twoToThe64MinusOne);
    EXPECT_EQ(big.str(), twoToThe64MinusOne);

    big *= big;
    EXPECT_FALSE(big.trivial());
    EXPECT_EQ(big.str(), "340282366920938463426481119284349108225");

    big *= 723489;
    EXPECT_EQ(big.str(), "246190549361262847965961398509914451960597025");

    big *= BigUint(twoToThe128MinusOne);
    EXPECT_EQ(
            big.str(),
            "837743028502166569017211202580842572065608"
            "65027745443300930865952827670618243921375");

    big *= 0;
    EXPECT_EQ(big.getSimple(), 0);
    EXPECT_TRUE(big.trivial());
    EXPECT_TRUE(big.zero());
}

TEST(LittleBigInt, DivisionEquals)
{
    BigUint big;

    big /= 1;
    EXPECT_EQ(big.getSimple(), 0);
    EXPECT_TRUE(big.zero());

    big /= BigUint(twoToThe128MinusOne);
    EXPECT_EQ(big.getSimple(), 0);
    EXPECT_TRUE(big.zero());

    EXPECT_THROW(big /= 0, std::invalid_argument);

    big = 1;

    EXPECT_EQ(big.getSimple(), 1);
    EXPECT_TRUE(big.trivial());

    EXPECT_THROW(big /= 0, std::invalid_argument);

    big /= 1;
    EXPECT_EQ(big.getSimple(), 1);
    EXPECT_TRUE(big.trivial());

    big /= 2;
    EXPECT_TRUE(big.zero());

    big = 1;

    big /= BigUint(twoToThe128MinusOne);
    EXPECT_TRUE(big.zero());

    big = BigUint(twoToThe128MinusOne);

    big /= 1;
    EXPECT_EQ(big, BigUint(twoToThe128MinusOne));

    big /= BigUint(twoToThe128);
    EXPECT_TRUE(big.zero());

    big = BigUint(bigE);

    big /= BigUint(twoToThe128MinusOne);
    EXPECT_EQ(big.str(), "7988312333241215178101817");

    big = BigUint(bigPi);
    big /= BigUint(bigE);
    EXPECT_EQ(
            big.str(),
            "11557273497909217179100931833126962991208510231644158204997065356"
            "35143084709932442722697945");

    big = BigUint(bigPi);
    big /= 1;
    EXPECT_EQ(big.str(), bigPi);

    big /= big + 1;
    EXPECT_TRUE(big.zero());
}

TEST(LittleBigInt, ModuloEquals)
{
    BigUint big;

    big %= 1;
    EXPECT_TRUE(big.zero());

    big %= BigUint(twoToThe128);
    EXPECT_TRUE(big.zero());

    EXPECT_THROW(big %= 0, std::invalid_argument);

    big = 1;

    EXPECT_THROW(big %= 0, std::invalid_argument);

    big %= 1;
    EXPECT_TRUE(big.zero());

    big = 1;
    big %= 2;
    EXPECT_EQ(big.getSimple(), 1);

    big %= BigUint(twoToThe128);
    EXPECT_EQ(big.getSimple(), 1);
    big %= BigUint(twoToThe128MinusOne);
    EXPECT_EQ(big.getSimple(), 1);

    big = BigUint(bigE);
    big %= BigUint(twoToThe128MinusOne);
    EXPECT_EQ(big.str(), "187862281444247134214799219766991253892");

    big = BigUint(bigE);
    big %= 54321;
    EXPECT_EQ(big.getSimple(), 17425);

    big = BigUint(bigPi);
    big %= BigUint(bigE);
    EXPECT_EQ(
            big.str(),
            "833506609181640258526740229346357683036844286048969278493701966");

    big = BigUint(bigPi);
    big %= (big + 1);
    EXPECT_EQ(big.str(), bigPi);

    big = BigUint(bigPi);
    big %= (big - 1);
    EXPECT_EQ(big.getSimple(), 1);
}

TEST(LittleBigInt, BitwiseAndEquals)
{
    BigUint big;

    big &= big;
    EXPECT_TRUE(big.zero());

    big &= 255;
    EXPECT_TRUE(big.zero());

    big &= BigUint(twoToThe128MinusOne);
    EXPECT_TRUE(big.zero());

    big = 0xFFFF;

    big &= 0xFFFF;
    EXPECT_EQ(big.getSimple(), 0xFFFF);

    big &= big;
    EXPECT_EQ(big.getSimple(), 0xFFFF);

    big &= 0;
    EXPECT_TRUE(big.zero());

    big = Block(-1);
    big &= BigUint(toggle);
    EXPECT_EQ(big.getSimple(), toggle);

    big = BigUint(twoToThe128MinusOne);
    big &= toggle;
    EXPECT_NO_THROW(big.getSimple());
    EXPECT_EQ(big.getSimple(), toggle);

    big = BigUint(twoToThe128MinusOne);
    big &= big;
    EXPECT_EQ(big.str(), twoToThe128MinusOne);

    big = toggle;
    big &= BigUint(twoToThe128MinusOne);
    EXPECT_NO_THROW(big.getSimple());
    EXPECT_EQ(big.getSimple(), toggle);

    big = BigUint(bigE);
    big &= 0;
    EXPECT_TRUE(big.zero());

    big = BigUint(bigE);
    big &= toggle;
    EXPECT_NO_THROW(big.getSimple());
    EXPECT_EQ(big.getSimple(), 2729052170);

    big = BigUint(bigE);
    big &= BigUint(bigPi);
    EXPECT_EQ(
            big.str(),
            "221098403465395518252809192553258077061683685751282349611417921");

    big = BigUint(bigPi);
    big &= big;
    EXPECT_EQ(big.str(), bigPi);

    big &= 1;
    EXPECT_NO_THROW(big.getSimple());
    EXPECT_EQ(big.getSimple(), 1);
}

TEST(LittleBigInt, BitwiseOrEquals)
{
    BigUint big;

    big |= 0;
    EXPECT_TRUE(big.zero());

    big |= big;
    EXPECT_TRUE(big.zero());

    big |= 1;
    EXPECT_EQ(big.getSimple(), 1);

    big = toggle;
    big |= (toggle >> 1);
    EXPECT_EQ(big.getSimple(), 4294967295);

    big |= 0;
    EXPECT_EQ(big.getSimple(), 4294967295);

    big |= BigUint(twoToThe128);
    EXPECT_EQ(big.str(), "340282366920938463463374607436063178751");

    big |= 0;
    EXPECT_EQ(big.str(), "340282366920938463463374607436063178751");

    big = BigUint(bigE);
    big |= toggle;
    EXPECT_EQ(
            big.str(),
            "2718281828459045235360287471352662497757247093699959575101226987");

    big = toggle;
    big |= BigUint(bigE);
    EXPECT_EQ(
            big.str(),
            "2718281828459045235360287471352662497757247093699959575101226987");

    big = BigUint(bigE);
    big |= BigUint(bigPi);
    EXPECT_EQ(
            big.str(),
            "3141592653589793238462643383279502884197169399375105820974944592"
            "3078164062862089986280348278393004929757978036207605854464982653"
            "05113990180402584763678187");
}

TEST(LittleBigInt, InterleaveRegression)
{
    std::size_t x(0);
    std::size_t y(7);
    BigUint expandX(0);
    BigUint expandY(0);

    for (std::size_t i(0); i < 64; ++i)
    {
        expandX |= BigUint((x >> i) & 1ULL) << (i * 2);
        expandY |= BigUint((y >> i) & 1ULL) << (i * 2);
    }

    BigUint result((expandY << 1) | expandX);

    ASSERT_TRUE(result.trivial());
    EXPECT_EQ(result.getSimple(), 42);
}

/*
TEST(LittleBigInt, BitShiftLeftEquals)
{
    // TODO
}

TEST(LittleBigInt, BitShiftRightEquals)
{
    // TODO
}

TEST(LittleBigInt, Plus)
{
    // TODO
}

TEST(LittleBigInt, Minus)
{
    // TODO
}

TEST(LittleBigInt, Times)
{
    // TODO
}

TEST(LittleBigInt, Division)
{
    // TODO
}

TEST(LittleBigInt, Modulo)
{
    // TODO
}

TEST(LittleBigInt, BitwiseAnd)
{
    // TODO
}

TEST(LittleBigInt, BigwiseOr)
{
    // TODO
}

TEST(LittleBigInt, BitShiftLeft)
{
    // TODO
}

TEST(LittleBigInt, BitShiftRight)
{
    // TODO
}

TEST(LittleBigInt, IncDec)
{
    // TODO
}

TEST(LittleBigInt, Comparisons)
{
    // TODO
}

TEST(LittleBigInt, Log2)
{
    // TODO
}
*/

TEST(LittleBigInt, Sqrt)
{
    EXPECT_EQ(sqrt(BigUint(68719476736)), 262144);
    EXPECT_EQ(
            sqrt(BigUint("22300745198530623141535718272648361505980416")),
            BigUint("4722366482869645213696"));
}

TEST(LittleBigInt, MapMovement)
{
    // This doesn't really test functionality, but tests that we survive the
    // issue here, relevant on Clang 7.3.0 and 7.3.1:
    //      https://openradar.appspot.com/25704040
    //
    // See https://github.com/connormanning/little-big-int/issues/1 for more
    // info.
    std::map<BigUint, uint64_t> from;
    const std::size_t v(3928502);
    from[v] = v;

    // If susceptible, we'll segfault here.
    auto to(std::move(from));

    EXPECT_EQ(from.size(), 0);
    EXPECT_EQ(to.size(), 1);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

