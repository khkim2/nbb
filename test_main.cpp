#include <stdio.h>
#include <gtest/gtest.h>

TEST(Main, HelloGoogleTest)
{
	EXPECT_EQ(1 + 1, 2);
}

#include <numcpp/array.h>

namespace np = numcpp;

TEST(ArrayType, DeclareEmptyArray)
{
	np::array_t<int> a1;

	ASSERT_TRUE(a1.empty());
	ASSERT_EQ(a1.size(), 0);
	ASSERT_EQ(a1.raw_ptr(), nullptr);
}

TEST(ArrayType, DeclareArrayWithSize)
{
	np::array_t<int> a1(5);

	ASSERT_FALSE(a1.empty());
	ASSERT_EQ(a1.size(), 5);
	ASSERT_NE(a1.raw_ptr(), nullptr);
}

// TEST(ArrayType, AccessElements)
// {
// 	np::array_t<int> a1(5);

// 	a1.at(0) = 2;
// 	a1.at(1) = 3;
// 	a1.at(2) = 1;
// 	a1.at(3) = 5;
// 	a1.at(4) = 7;
// }

int main(int argc, char **argv)
{
	puts("Hello Tests!");

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}