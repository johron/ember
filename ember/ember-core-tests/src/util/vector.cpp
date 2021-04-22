#include <type_traits>

using namespace ember;

TEST(vector, construct_default) {
	constexpr util::vector2i vec2;
	constexpr util::vector3i vec3;
}

TEST(vector, construct_from_args) {
	constexpr util::vector2i vec{1, 2};
	EXPECT_EQ(vec.x, 1);
	EXPECT_EQ(vec.y, 2);
}

TEST(vector, construct_from_copy) {
	constexpr util::vector2i vec_1{1, 2};
	constexpr util::vector2f vec_2(vec_1);
	static_assert(vec_1 == vec_2);
}

TEST(vector, size) {
	constexpr util::vector2i vec2{1, 2};
	static_assert(vec2.size() == 2, "incorrect vector size");

	constexpr util::vector3i vec3{1, 2, 3};
	static_assert(vec3.size() == 3, "incorrect vector size");
}

TEST(vector, iterator_access) {
	constexpr util::vector2i vec{1, 2};
	int expected_value = 1;
	for (const auto& element : vec) {
		EXPECT_EQ(expected_value, element);
		expected_value++;
	}
}

TEST(vector, subscript_operator) {
	constexpr util::vector2i vec{1, 2};
	static_assert(vec[0] == 1, "subscript operator failed");
	static_assert(vec[1] == 2, "subscript operator failed");
}

TEST(vector, comparison_operator) {
	constexpr util::vector2i vec_1{1, 2};
	constexpr util::vector2i vec_2{2, 3};
	static_assert(vec_1 != vec_2, "vector comparison failed");
	static_assert(!(vec_1 == vec_2), "vector comparison failed");
	static_assert(vec_1 == util::vector2i(1, 2), "vector comparison failed");
}

TEST(vector, comparison_different_types) {
	constexpr util::vector2i vec_1{1.f, 2.f};
	constexpr util::vector2f vec_2{1, 2};
	static_assert(vec_1 == vec_2, "vector comparison with different typed failed");
}

TEST(vector, addition) {
	constexpr util::vector2i vec_1{1, 2};
	constexpr util::vector2i vec_2{3, 4};
	constexpr auto result = vec_1 + vec_2;
	static_assert(result == util::vector2i{4, 6});
}

TEST(vector, addition_composite) {
	util::vector2i vec_1{1, 2};
	vec_1 += util::vector2i{2, 3};
	EXPECT_EQ(vec_1, util::vector2i(3, 5));
}

TEST(vector, subtraction) {
	constexpr util::vector2i vec_1{1, 2};
	constexpr util::vector2i vec_2{3, 4};
	constexpr auto result = vec_1 - vec_2;
	static_assert(result == util::vector2i{-2, -2});
}

TEST(vector, subtraction_composite) {
	util::vector2i vec_1{1, 2};
	vec_1 -= util::vector2i{2, 3};
	EXPECT_EQ(vec_1, util::vector2i(-1, -1));
}

TEST(vector, scalar_multiplication) {
	constexpr util::vector2i vec_1{1, 2};
	constexpr auto result = static_cast<util::vector2f>(vec_1) * 2.5f;
	static_assert(std::is_same_v<decltype(result), const util::vector2f>, "multiplication return value");
	static_assert(result == util::vector2f{2.5f, 5.0f});
}

TEST(vector, scalar_multiplication_composite) {
	util::vector2i vec{1, 2};
	vec *= 2;
	static_assert(std::is_same_v<decltype(vec), util::vector2i>, "multiplication return value");
	EXPECT_EQ(vec, util::vector2i(2, 4));
}

TEST(vector, scalar_division) {
	constexpr util::vector2i vec{2, 5};
	constexpr auto result = static_cast<util::vector2f>(vec) / 2.f;
	static_assert(std::is_same_v<decltype(result), const util::vector2f>, "division return value");
	static_assert(result == util::vector2f(1, 2.5f));
}

TEST(vector, scalar_division_composite) {
	util::vector2i vec{2, 5};
	vec /= 2;
	EXPECT_EQ(vec, util::vector2i(1, 2));
}