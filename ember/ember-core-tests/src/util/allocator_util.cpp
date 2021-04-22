#include "util/allocator_util.h"

struct test_object {
	test_object(int number, std::string string)
		: m_number(number)
		, m_string(string) {}
	int m_number;
	std::string m_string;
};

using namespace ember;

TEST(allocator_util, unique_allocator) {
	util::unique_allocator<test_object> allocator;
	const auto obj = allocator(1, "hello");
	static_assert(std::is_same_v<decltype(obj), const std::unique_ptr<test_object>>);
	EXPECT_EQ(obj->m_number, 1);
	EXPECT_EQ(obj->m_string, "hello");
}

TEST(allocator_util, heap_allocator) {
	util::heap_allocator<test_object> allocator;
	const auto obj = allocator(1, "hello");
	static_assert(std::is_same_v<decltype(obj), test_object* const>);
	EXPECT_EQ(obj->m_number, 1);
	EXPECT_EQ(obj->m_string, "hello");
}

TEST(allocator_util, stack_allocator) {
	util::stack_allocator<test_object> allocator;
	const auto obj = allocator(1, "hello");
	static_assert(std::is_same_v<decltype(obj), const test_object>);
	EXPECT_EQ(obj.m_number, 1);
	EXPECT_EQ(obj.m_string, "hello");
}