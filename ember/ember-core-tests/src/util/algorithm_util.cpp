#include "util/algorithm_util.h"
#include "util/object_selection.h"

using namespace ember;

TEST(algorithm_util, erase_if) {
	std::vector<int> numbers{1, 1, 3, 5, 8, 4, 2};
	const auto is_one = [](int number) { return number == 1; };
	const auto is_even = [](int number) { return number % 2 == 0; };
	EXPECT_EQ(numbers.size(), 7);
	util::erase_if(numbers, is_one);
	EXPECT_EQ(numbers.size(), 5);
	util::erase_if(numbers, is_even);
	EXPECT_EQ(numbers.size(), 2);
}

TEST(algorithm_util, erase) {
	std::vector<int> numbers{1, 1, 3, 5, 8, 4, 2};
	EXPECT_EQ(numbers.size(), 7);
	util::erase(numbers, 1);
	EXPECT_EQ(numbers.size(), 5);
}

TEST(algorithm_util, find_if) {
	std::vector<int> numbers{1, 1, 3, 5, 8, 4, 2};
	const auto is_one = [](int number) { return number == 1; };
	const auto is_zero = [](int number) { return number == 0; };
	EXPECT_NE(util::find_if(numbers, is_one), numbers.end());
	EXPECT_EQ(util::find_if(numbers, is_zero), numbers.end());
}

TEST(algorithm_util, for_each) {
	std::vector<int> numbers{1, 1, 3, 5, 8, 4, 2};
	const auto increment = [](auto& number) { ++number; };
	EXPECT_EQ(std::accumulate(numbers.begin(), numbers.end(), 0), 24);
	util::for_each(numbers, increment);
	EXPECT_EQ(std::accumulate(numbers.begin(), numbers.end(), 0), 31);
}