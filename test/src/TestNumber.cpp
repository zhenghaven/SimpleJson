#include <gtest/gtest.h>

#include <SimpleJson/Number.hpp>

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
using namespace SimpleJson;
#else
using namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE;
#endif

namespace SimpleJson_Test
{
	extern size_t g_numOfTestFile;
}

GTEST_TEST(TestNumber, CountTestFile)
{
	++SimpleJson_Test::g_numOfTestFile;
}

GTEST_TEST(TestNumber, ConstructorAndAssignment)
{
	Number<int32_t> num1;
	EXPECT_EQ(num1.GetVal(), 0);

	Number<int32_t> num2(12345);
	EXPECT_EQ(num2.GetVal(), 12345);

	Number<int32_t> num3(num2);
	EXPECT_EQ(num3.GetVal(), 12345);

	Number<int32_t> num4(std::move(num3));
	EXPECT_EQ(num4.GetVal(), 12345);

	Number<int32_t> num5 = 12345;
	EXPECT_EQ(num5.GetVal(), 12345);

	Number<int32_t> num6 = num5;
	EXPECT_EQ(num6.GetVal(), 12345);

	Number<int32_t> num7 = std::move(num6);
	EXPECT_EQ(num7.GetVal(), 12345);

	num2 = 67890;
	EXPECT_EQ(num2.GetVal(), 67890);

	num3 = num2;
	EXPECT_EQ(num3.GetVal(), 67890);

	num4 = std::move(num3);
	EXPECT_EQ(num4.GetVal(), 67890);
}

GTEST_TEST(TestNumber, CompareNumber)
{
	const Number<int32_t> num1 = 12345;
	const Number<int32_t> num2 = 67890;
	EXPECT_EQ(num1 == num2, num1.GetVal() == num2.GetVal());
	EXPECT_EQ(num1 != num2, num1.GetVal() != num2.GetVal());
	EXPECT_EQ(num1 >= num2, num1.GetVal() >= num2.GetVal());
	EXPECT_EQ(num1 <= num2, num1.GetVal() <= num2.GetVal());
	EXPECT_EQ(num1 >  num2, num1.GetVal() >  num2.GetVal());
	EXPECT_EQ(num1 <  num2, num1.GetVal() <  num2.GetVal());

	const Number<int32_t> num3 = 12345;
	EXPECT_EQ(num1 == num3, num1.GetVal() == num3.GetVal());
	EXPECT_EQ(num1 != num3, num1.GetVal() != num3.GetVal());
	EXPECT_EQ(num1 >= num3, num1.GetVal() >= num3.GetVal());
	EXPECT_EQ(num1 <= num3, num1.GetVal() <= num3.GetVal());
	EXPECT_EQ(num1 >  num3, num1.GetVal() >  num3.GetVal());
	EXPECT_EQ(num1 <  num3, num1.GetVal() <  num3.GetVal());
}

GTEST_TEST(TestNumber, CompareValue)
{
	{
		const Number<int32_t> num1 = 12345;
		const int32_t num2 = 67890;
		EXPECT_EQ(num1 == num2, num1.GetVal() == num2);
		EXPECT_EQ(num1 != num2, num1.GetVal() != num2);
		EXPECT_EQ(num1 >= num2, num1.GetVal() >= num2);
		EXPECT_EQ(num1 <= num2, num1.GetVal() <= num2);
		EXPECT_EQ(num1 >  num2, num1.GetVal() >  num2);
		EXPECT_EQ(num1 <  num2, num1.GetVal() <  num2);

		const int32_t num3 = 12345;
		EXPECT_EQ(num1 == num3, num1.GetVal() == num3);
		EXPECT_EQ(num1 != num3, num1.GetVal() != num3);
		EXPECT_EQ(num1 >= num3, num1.GetVal() >= num3);
		EXPECT_EQ(num1 <= num3, num1.GetVal() <= num3);
		EXPECT_EQ(num1 >  num3, num1.GetVal() >  num3);
		EXPECT_EQ(num1 <  num3, num1.GetVal() <  num3);
	}
	{
		const int32_t num1 = 12345;
		const Number<int32_t> num2 = 67890;
		EXPECT_EQ(num1 == num2, num1 == num2.GetVal());
		EXPECT_EQ(num1 != num2, num1 != num2.GetVal());
		EXPECT_EQ(num1 >= num2, num1 >= num2.GetVal());
		EXPECT_EQ(num1 <= num2, num1 <= num2.GetVal());
		EXPECT_EQ(num1 >  num2, num1 >  num2.GetVal());
		EXPECT_EQ(num1 <  num2, num1 <  num2.GetVal());

		const Number<int32_t> num3 = 12345;
		EXPECT_EQ(num1 == num3, num1 == num3.GetVal());
		EXPECT_EQ(num1 != num3, num1 != num3.GetVal());
		EXPECT_EQ(num1 >= num3, num1 >= num3.GetVal());
		EXPECT_EQ(num1 <= num3, num1 <= num3.GetVal());
		EXPECT_EQ(num1 >  num3, num1 >  num3.GetVal());
		EXPECT_EQ(num1 <  num3, num1 <  num3.GetVal());
	}
}

GTEST_TEST(TestNumber, ComputeNumber)
{
	const Number<int32_t> num1 = 67890;
	const Number<int32_t> num2 = 12345;
	EXPECT_EQ(num1 + num2, num1.GetVal() + num2.GetVal());
	EXPECT_EQ(num1 - num2, num1.GetVal() - num2.GetVal());
	EXPECT_EQ(num1 * num2, num1.GetVal() * num2.GetVal());
	EXPECT_EQ(num1 / num2, num1.GetVal() / num2.GetVal());
	EXPECT_EQ(num1 % num2, num1.GetVal() % num2.GetVal());

	const auto num3 = num1.GetVal();

	EXPECT_EQ(-num1, -num3);
	EXPECT_EQ(num1, num3);
}

GTEST_TEST(TestNumber, ComputeSelfNumber)
{
	Number<int32_t> num1 = 678;
	const Number<int32_t> num2 = 123;

	auto num3 = num1.GetVal();
	const auto num4 = num2.GetVal();

	EXPECT_EQ(num1 += num2, num3 += num4);
	EXPECT_EQ(num1, num3);
	EXPECT_EQ(num2, num4);

	EXPECT_EQ(num1 -= num2, num3 -= num4);
	EXPECT_EQ(num1, num3);
	EXPECT_EQ(num2, num4);

	EXPECT_EQ(num1 *= num2, num3 *= num4);
	EXPECT_EQ(num1, num3);
	EXPECT_EQ(num2, num4);

	EXPECT_EQ(num1 /= num2, num3 /= num4);
	EXPECT_EQ(num1, num3);
	EXPECT_EQ(num2, num4);

	EXPECT_EQ(num1 %= num2, num3 %= num4);
	EXPECT_EQ(num1, num3);
	EXPECT_EQ(num2, num4);

	EXPECT_EQ(num1++, num3++);
	EXPECT_EQ(num1, num3);
	EXPECT_EQ(++num1, ++num3);
	EXPECT_EQ(num1, num3);

	EXPECT_EQ(num1--, num3--);
	EXPECT_EQ(num1, num3);
	EXPECT_EQ(--num1, --num3);
	EXPECT_EQ(num1, num3);
}

GTEST_TEST(TestNumber, ComputeValue)
{
	{
		const Number<int32_t> num1 = 67890;
		const int32_t num2 = 12345;
		EXPECT_EQ(num1 + num2, num1.GetVal() + num2);
		EXPECT_EQ(num1 - num2, num1.GetVal() - num2);
		EXPECT_EQ(num1 * num2, num1.GetVal() * num2);
		EXPECT_EQ(num1 / num2, num1.GetVal() / num2);
		EXPECT_EQ(num1 % num2, num1.GetVal() % num2);
	}
	{
		const int32_t num1 = 67890;
		const Number<int32_t> num2 = 12345;
		EXPECT_EQ(num1 + num2, num1 + num2.GetVal());
		EXPECT_EQ(num1 - num2, num1 - num2.GetVal());
		EXPECT_EQ(num1 * num2, num1 * num2.GetVal());
		EXPECT_EQ(num1 / num2, num1 / num2.GetVal());
		EXPECT_EQ(num1 % num2, num1 % num2.GetVal());
	}
}

GTEST_TEST(TestNumber, ComputeSelfValue)
{
	Number<int32_t> num1 = 678;
	const int32_t num2 = 123;

	auto num3 = num1.GetVal();
	const auto num4 = num2;

	EXPECT_EQ(num1 += num2, num3 += num4);
	EXPECT_EQ(num1, num3);
	EXPECT_EQ(num2, num4);

	EXPECT_EQ(num1 -= num2, num3 -= num4);
	EXPECT_EQ(num1, num3);
	EXPECT_EQ(num2, num4);

	EXPECT_EQ(num1 *= num2, num3 *= num4);
	EXPECT_EQ(num1, num3);
	EXPECT_EQ(num2, num4);

	EXPECT_EQ(num1 /= num2, num3 /= num4);
	EXPECT_EQ(num1, num3);
	EXPECT_EQ(num2, num4);

	EXPECT_EQ(num1 %= num2, num3 %= num4);
	EXPECT_EQ(num1, num3);
	EXPECT_EQ(num2, num4);
}
