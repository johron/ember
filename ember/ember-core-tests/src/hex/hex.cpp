#include "grid/hex_pos.h"
#include "grid/hex_util.h"

using namespace ember;
static const auto test_layout = hex::flat_layout(20);

TEST(hex_pos, construction) {
	constexpr hex::pos pos{1, 2};
	static_assert(pos.q() == 1);
	static_assert(pos.r() == 2);
	static_assert(pos.s() == -3);
}

TEST(hex_pos, equality) {
	constexpr hex::pos pos1{1, 1};
	constexpr hex::pos pos2{1, 1};
	static_assert(pos1 == pos2);
}

TEST(hex_pos, inequality) {
	constexpr hex::pos pos1{1, 1};
	constexpr hex::pos pos2{-1, 1};
	static_assert(pos1 != pos2);
}

TEST(hex_pos, addition) {
	constexpr hex::pos pos1{1, 1};
	constexpr hex::pos pos2{1, 1};
	constexpr auto sum = pos1 + pos2;
	static_assert(sum == hex::pos{2, 2});
}

TEST(hex_pos, subtraction) {
	constexpr hex::pos pos1{1, 1};
	constexpr hex::pos pos2{1, 1};
	constexpr auto sum = pos1 - pos2;
	static_assert(sum == hex::pos{0, 0});
}

TEST(hex_pos, distance) {
	constexpr hex::pos pos1{0, 0};
	constexpr hex::pos pos2(-3, 2);
	EXPECT_EQ(hex::distance(pos1, pos2), 3);
}
/*
TEST(hex_util, hex_to_pixel) {
	constexpr hex::pos pos1{0, 1};
	const auto pixel = hex::hex_to_pixel(test_layout, pos1);
	const auto origin = util::vector2i{0, 34};
	EXPECT_EQ(pixel, origin);
}
*/